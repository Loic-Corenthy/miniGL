//===============================================================================================//
/*!
 *  \file      MultipassShadowMapTechnique.cpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#include "MultipassShadowMapTechnique.hpp"

#include "EngineCommon.hpp"
#include "GLUtils.hpp"

using std::map;
using std::vector;
using std::string;
using std::shared_ptr;
using std::static_pointer_cast;
using std::make_unique;
using std::make_shared;
using std::make_tuple;
using std::get;
using std::tuple;
using std::numeric_limits;
using miniGL::MultipassShadowMapTechnique;
using miniGL::RenderingTechniqueBase;
using miniGL::MeshAndTransform;
using miniGL::BaseLight;

MultipassShadowMapTechnique::MultipassShadowMapTechnique(void)
:RenderingTechniqueBase("MultipassShadowMapTechnique")
{
}

void MultipassShadowMapTechnique::init(unsigned int pPointLightCount, const tuple<unsigned int, unsigned int> & pFramebufferDimensions)
{
    mMultipassShadowMapFBO = make_unique<MultipassShadowMapFBO>();
    mMultipassShadowMapFBO->init(get<0>(pFramebufferDimensions), get<1>(pFramebufferDimensions));

    // Setup the shadow map render
    mMultipassShadowMapLighting = make_unique<MultipassShadowMapLighting>();
    mMultipassShadowMapLighting->init();
    // No need to call use because it is called at the end of the init method
    //    mMultipassShadowMapLighting->use();
    mMultipassShadowMapLighting->colorTextureUnit(COLOR_TEXTURE_UNIT_INDEX);
    mMultipassShadowMapLighting->shadowTextureUnit(SHADOW_TEXTURE_UNIT_INDEX);

    // Setup the shadow map first pass render
    mMultipassShadowMap = make_unique<MultipassShadowMap>();
    mMultipassShadowMap->init();
//    mMultipassShadowMap->use();

    // Setup opengl to use cube maps
    glEnable(GL_TEXTURE_CUBE_MAP); checkOpenGLState;
}

void MultipassShadowMapTechnique::render(const map<string, MeshAndTransform> & pMeshes, const vector<shared_ptr<BaseLight>> & pLights)
{
    // Find the meshes to render
    auto lMeshReferences = findMeshesToRender(pMeshes);

    // Find the mesh representing the "floor"
    auto lFloorIterator = pMeshes.find(mFloorMeshName);
    assert(lFloorIterator != pMeshes.end());

    // Render the different elements
    _shadowPass(lMeshReferences, pLights);
    _renderPass(lMeshReferences, lFloorIterator, pLights);
}

void MultipassShadowMapTechnique::floor(const string & pName)
{
    mFloorMeshName = pName;
}

void MultipassShadowMapTechnique::pointLightIndex(unsigned int pIndex)
{
    mPointLightIndex = pIndex;
}

void MultipassShadowMapTechnique::_shadowPass(vector<map<string, MeshAndTransform>::const_iterator> & pMeshIterators, const vector<shared_ptr<BaseLight>> & pLights)
{
    glCullFace(GL_FRONT);

    mMultipassShadowMap->use();

    mMultipassShadowMap->lightPosition(static_pointer_cast<PointLight>(pLights.at(mPointLightIndex))->position());

    auto lTmpCamera = *mCamera;

    // To get all the light coming from the point light to the face of the cube, use a 90 degrees field of view
    const degreef lCameraFOV = 90.0f;
    lTmpCamera.verticalFoV(lCameraFOV.toRadian());

    // To render the distance between the light and the objects in the scene, set the camera at the point light position
    lTmpCamera.position(static_pointer_cast<PointLight>(pLights[mPointLightIndex])->position());

    glClearColor(numeric_limits<float>::max(), numeric_limits<float>::max(), numeric_limits<float>::max(), numeric_limits<float>::max());

    const tuple<GLenum, vec3f, vec3f> lCameraDirections[6] = {  make_tuple(GL_TEXTURE_CUBE_MAP_POSITIVE_X, vec3f( 1.0f,  0.0f,  0.0f), vec3f( 0.0f,  1.0f, 0.0f)),
                                                                make_tuple(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, vec3f(-1.0f,  0.0f,  0.0f), vec3f( 0.0f,  1.0f, 0.0f)),
                                                                make_tuple(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, vec3f( 0.0f,  1.0f,  0.0f), vec3f( 0.0f,  0.0f,-1.0f)),
                                                                make_tuple(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, vec3f( 0.0f, -1.0f,  0.0f), vec3f( 0.0f,  0.0f, 1.0f)),
                                                                make_tuple(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, vec3f( 0.0f,  0.0f,  1.0f), vec3f( 0.0f,  1.0f, 0.0f)),
                                                                make_tuple(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, vec3f( 0.0f,  0.0f, -1.0f), vec3f( 0.0f,  1.0f, 0.0f))};

    const unsigned int cubemapFace = 0u, target = 1u, up = 2u;

    for (unsigned int i = 0; i < 6; ++i)
    {
        mMultipassShadowMapFBO->bindForWriting(get<cubemapFace>(lCameraDirections[i]));
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        lTmpCamera.lookAt(get<target>(lCameraDirections[i]));
        lTmpCamera.up(get<up>(lCameraDirections[i]));

        for (auto it : pMeshIterators)
        {
            for (auto transformation : it->second.transform)
            {
                mat4f lWorld = transformation.final();
                mat4f lWVP = lTmpCamera.projection() * lTmpCamera.view() * lWorld;

                mMultipassShadowMap->WVP(lWVP);
                mMultipassShadowMap->world(lWorld);

                it->second.mesh->render();
            }
        }
    }
}

void MultipassShadowMapTechnique::_renderPass(vector<map<string, MeshAndTransform>::const_iterator> & pMeshIterators, map<string, MeshAndTransform>::const_iterator & pFloorIterator, const vector<shared_ptr<BaseLight>> & pLights)
{
    /*! \bug The shadow is not correctly displayed if the window is not a square window (width = height) */
    /*! \bug The shadow is not correctly displayed if the light is moved toward the left or the right... */

    glCullFace(GL_BACK);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // In the shadow pass, we have to set the clear color to max float to initialize the cube map, restore default value here
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mMultipassShadowMapLighting->use();
    mMultipassShadowMapFBO->bindForReading(SHADOW_TEXTURE_UNIT);
    mMultipassShadowMapLighting->eyeWorldPosition(mCamera->position());
    mMultipassShadowMapLighting->updatePointLightState(static_pointer_cast<PointLight>(pLights.at(mPointLightIndex)));

    // Render the floor (and the wall)
    for (auto transformation : pFloorIterator->second.transform)
    {
        mat4f lWorld = transformation.final();
        mat4f lWVP = mCamera->projection() * mCamera->view() * lWorld;

        mMultipassShadowMapLighting->WVP(lWVP);
        mMultipassShadowMapLighting->world(lWorld);

        pFloorIterator->second.mesh->render();
    }

    // Render the meshes
    for (auto it : pMeshIterators)
    {
        for (auto transformation : it->second.transform)
        {
            mat4f lWorld = transformation.final();
            mat4f lWVP = mCamera->projection() * mCamera->view() * lWorld;

            mMultipassShadowMapLighting->WVP(lWVP);
            mMultipassShadowMapLighting->world(lWorld);

            it->second.mesh->render();
        }
    }
}
