//===============================================================================================//
/*!
 *  \file      ShadowMapDirectionalLightTechnique.cpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//


#include "ShadowMapDirectionalLightTechnique.hpp"

#include "EngineCommon.hpp"

using std::map;
using std::vector;
using std::string;
using std::make_unique;
using std::make_shared;
using std::static_pointer_cast;
using std::shared_ptr;
using std::get;
using std::tuple;
using miniGL::RenderingTechniqueBase;
using miniGL::ShadowMapDirectionalLight;
using miniGL::ShadowMapDirectionalLightLighting;
using miniGL::ShadowMapDirectionalLightTechnique;
using miniGL::MeshAndTransform;
using miniGL::BaseLight;
using miniGL::DirectionalLight;

ShadowMapDirectionalLightTechnique::ShadowMapDirectionalLightTechnique(void)
:RenderingTechniqueBase("ShadowMapDirectionalLightTechnique")
{
}

void ShadowMapDirectionalLightTechnique::init(const tuple<unsigned int, unsigned int> & pFramebufferDimensions)
{
    mShadowMapFBO = make_unique<ShadowMapFBO>();
    mShadowMapFBO->init(get<0>(pFramebufferDimensions), get<1>(pFramebufferDimensions));

    mShadowMapDirectionalLight = make_unique<ShadowMapDirectionalLight>();
    mShadowMapDirectionalLight->init();

    mShadowMapDirectionalLightLighting = make_unique<ShadowMapDirectionalLightLighting>();
    mShadowMapDirectionalLightLighting->init();
    mShadowMapDirectionalLightLighting->use();
    mShadowMapDirectionalLightLighting->colorTextureUnit(COLOR_TEXTURE_UNIT_INDEX);
    mShadowMapDirectionalLightLighting->shadowTextureUnit(SHADOW_TEXTURE_UNIT_INDEX);
    mShadowMapDirectionalLightLighting->materialSpecularIntensity(0.0f);
    mShadowMapDirectionalLightLighting->materialSpecularPower(0.0f);
}

void ShadowMapDirectionalLightTechnique::render(const map<string, MeshAndTransform> & pMeshes, const vector<shared_ptr<BaseLight>> & pLights)
{
    // Find the meshes to render
    auto lMeshReferences = findMeshesToRender(pMeshes);

    // Find the mesh representing the "floor"
    auto lFloorIterator = pMeshes.find(mFloorMeshName);
    assert(lFloorIterator != pMeshes.end());

    assert(pLights[0]->type() == BaseLight::EType::DIRECTIONAL);

    _shadowPass(lMeshReferences, static_pointer_cast<DirectionalLight>(pLights[0]));
    _renderPass(lMeshReferences, lFloorIterator, static_pointer_cast<DirectionalLight>(pLights[0]));
}

void ShadowMapDirectionalLightTechnique::floor(const string & pName)
{
    mFloorMeshName = pName;
}

void ShadowMapDirectionalLightTechnique::_shadowPass(vector<map<string, MeshAndTransform>::const_iterator> & pMeshIterators, shared_ptr<DirectionalLight> pDirectionalLight)
{
    mShadowMapFBO->bindForWriting();
    glClear(GL_DEPTH_BUFFER_BIT);

    mShadowMapDirectionalLight->use();

    // Set the tmp camera as the directional light
    Camera lTmpCamera = *mCamera;
    lTmpCamera.position(vec3f(0.0f, 0.0f, 0.0f));
    lTmpCamera.lookAt(pDirectionalLight->direction());
    lTmpCamera.up(vec3f(0.0f, 1.0f, 0.0f));

    for (auto it : pMeshIterators)
    {
        for (auto transform : it->second.transform)
        {
            mat4f lWorld = transform.final();
            mat4f lWVP = lTmpCamera.orthogonalProjection(0) * lTmpCamera.view() * lWorld;

            mShadowMapDirectionalLight->WVP(lWVP);

            it->second.mesh->render();
        }
    }
}

void ShadowMapDirectionalLightTechnique::_renderPass(vector<map<string, MeshAndTransform>::const_iterator> & pMeshIterators, map<string, MeshAndTransform>::const_iterator & pFloorIterator, shared_ptr<DirectionalLight> pDirectionalLight)
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mShadowMapDirectionalLightLighting->use();
    mShadowMapDirectionalLightLighting->eyeWorldPosition(mCamera->position());

    mShadowMapFBO->bindForReading(SHADOW_TEXTURE_UNIT);

    assert(pFloorIterator->second.transform.size() == 1 && "Assumed the floor has only one tranform");

    // Set the tmp camera as the directional light
    Camera lTmpCamera = *mCamera;
    lTmpCamera.position(vec3f(0.0f, 0.0f, 0.0f));
    lTmpCamera.lookAt(pDirectionalLight->direction());
    lTmpCamera.up(vec3f(0.0f, 1.0f, 0.0f));

    // Configure the WVP for the light with the orthogonal projection
    mat4f lWorld = pFloorIterator->second.transform[0].final();
    mat4f lLightWVP = lTmpCamera.orthogonalProjection(0) * lTmpCamera.view() * lWorld;
    mShadowMapDirectionalLightLighting->lightWVP(lLightWVP);

    // Update the parameters of the directional light to the GPU
    mShadowMapDirectionalLightLighting->updateDirectionalLightState(pDirectionalLight);

    // Configure the WVP for the quad, with the normal perspective projection of the camera
    mat4f lWVP = mCamera->projection() * mCamera->view() * lWorld;
    mShadowMapDirectionalLightLighting->world(lWorld);
    mShadowMapDirectionalLightLighting->WVP(lWVP);

    // Render the shadow on the floor
    pFloorIterator->second.mesh->render();

    for (auto it : pMeshIterators)
    {
        for (auto transform : it->second.transform)
        {
            mat4f lWorld = transform.final();
            mat4f lWVP = mCamera->projection() * mCamera->view()  * lWorld;

            mShadowMapDirectionalLightLighting->world(lWorld);
            mShadowMapDirectionalLightLighting->WVP(lWVP);

            it->second.mesh->render();
        }
    }
}
