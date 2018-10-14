//===============================================================================================//
/*!
 *  \file      SimpleLightingWithShadow.cpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#include "SimpleLightingWithShadow.hpp"

#include "SpotLight.hpp"
#include "EngineCommon.hpp"

using std::vector;
using std::map;
using std::string;
using std::make_unique;
using std::shared_ptr;
using std::static_pointer_cast;
using std::get;
using miniGL::SimpleLightingWithShadow;
using miniGL::RenderingTechniqueBase;
using miniGL::MeshAndTransform;
using miniGL::BaseLight;

SimpleLightingWithShadow::SimpleLightingWithShadow(void)
:RenderingTechniqueBase("SimpleLightingWithShadow")
{
}

void SimpleLightingWithShadow::init(unsigned int pPointLightCount, const std::tuple<unsigned int, unsigned int> & pFramebufferDimensions)
{
    mShadowMapFBO = make_unique<ShadowMapFBO>();
    mShadowMapFBO->init(get<0>(pFramebufferDimensions), get<1>(pFramebufferDimensions));

    mShadowMap = make_unique<ShadowMap>();
    mShadowMap->init("./Shaders/ShadowMap.vert", "./Shaders/ShadowMap.frag");

    mLighting = make_unique<Lighting>();

    // This rendering technique only compute shadows for 1 spot light
    const unsigned int lSpotLightCount = 1;
    mLighting->init(pPointLightCount, lSpotLightCount);
    mLighting->shadowMapSize(get<0>(pFramebufferDimensions), get<1>(pFramebufferDimensions));

    /*! \todo This normal map should not be stored in this class! */
    mNormalMap.target(GL_TEXTURE_2D);
    mNormalMap.loadImage(R"(./normal_map.jpg)");
}

void SimpleLightingWithShadow::render(const map<string, MeshAndTransform> & pMeshes, const vector<shared_ptr<BaseLight>> & pLights)
{
    // Find the meshes to render
    auto lMeshReferences = findMeshesToRender(pMeshes);

    // Find the mesh representing the "floor"
    auto lFloorIterator = pMeshes.find(mFloorMeshName);
    assert(lFloorIterator != pMeshes.end());

    // Find the first spot light among all the lights
    vector<shared_ptr<BaseLight>>::const_iterator lFirstSpotLightIterator = pLights.cbegin();

    for (; lFirstSpotLightIterator != pLights.end(); lFirstSpotLightIterator++)
    {
        if ((*lFirstSpotLightIterator)->type() == BaseLight::EType::SPOT)
            break;
    }

    if (mUseShadowMap)
        _shadowMapPass(lMeshReferences, lFirstSpotLightIterator);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    _renderWithShadowAndBumpMapping(lMeshReferences, lFloorIterator, pLights, lFirstSpotLightIterator);
}

void SimpleLightingWithShadow::useShadowMap(bool pActivate) noexcept
{
    mUseShadowMap = pActivate;
}

void SimpleLightingWithShadow::floor(const string & pName)
{
    mFloorMeshName = pName;
}

void SimpleLightingWithShadow::_shadowMapPass(vector<map<string, MeshAndTransform>::const_iterator> & pMeshIterators, vector<shared_ptr<BaseLight>>::const_iterator pSpotLightIterator)
{
    glCullFace(GL_FRONT);

    mShadowMapFBO->bindForWriting();

    glClear(GL_DEPTH_BUFFER_BIT);

    mShadowMap->use();

    // Set the camera at the spot light position
    Camera lTmpCamera = *mCamera;
    lTmpCamera.position(static_pointer_cast<SpotLight>(*pSpotLightIterator)->position());
    lTmpCamera.lookAt(static_pointer_cast<SpotLight>(*pSpotLightIterator)->direction());
    lTmpCamera.up(vec3f(0.0f, 1.0f, 0.0f));

    for (auto it : pMeshIterators)
    {
        for (auto transform : it->second.transform)
        {
            mat4f lWorld = transform.final();

            mat4f lWVP = lTmpCamera.projection() * lTmpCamera.view() * lWorld;

            mShadowMap->setWVP(lWVP);

            it->second.mesh->render();
        }
    }
}

void SimpleLightingWithShadow::_renderWithShadowAndBumpMapping(vector<map<string, MeshAndTransform>::const_iterator> & pMeshIterators, map<string, MeshAndTransform>::const_iterator & pFloorIterator, vector<shared_ptr<BaseLight>> pLights, vector<shared_ptr<BaseLight>>::const_iterator pSpotLightIterator)
{
    glCullFace(GL_BACK);

    mLighting->use();
    mLighting->useShadowMap(mUseShadowMap);

    mLighting->eyeWorldPosition(mCamera->position());

    if (mUseShadowMap)
        mShadowMapFBO->bindForReading(SHADOW_TEXTURE_UNIT);

    assert(pFloorIterator->second.transform.size() == 1 && "Assumed the floor has only one tranform");
    mat4f lWorld = pFloorIterator->second.transform[0].final();
    mat4f lWVP = mCamera->projection() * mCamera->view() * lWorld;

    mLighting->worldMatrix(lWorld);
    mLighting->WVP(lWVP);

    // Set the camera at the spot light position
    Camera lLightCamera = *mCamera;

    lLightCamera.position(static_pointer_cast<SpotLight>(*pSpotLightIterator)->position());
    lLightCamera.lookAt(static_pointer_cast<SpotLight>(*pSpotLightIterator)->direction());
    lLightCamera.up(vec3f(0.0f, 1.0f, 0.0f));

    mat4f lLightWVP = lLightCamera.projection() * lLightCamera.view() * lWorld;

    mLighting->lightWVP(lLightWVP);

    mLighting->updateLightsState(pLights);

    // Render the "floor" with the shadows from the spot light
    pFloorIterator->second.mesh->render();

    mLighting->useShadowMap(false);

    for (auto it : pMeshIterators)
    {
        if(it->second.mesh->loadOption() == MeshBase::EOptions::COMPUTE_TANGENT_SPACE)
        {
            mLighting->useNormalMap(true);
            mNormalMap.bind(NORMAL_TEXTURE_UNIT);
        }
        else
            mLighting->useNormalMap(false);

        for (auto transformation : it->second.transform)
        {
            mat4f lWorld2 = transformation.final();
            mat4f lWVP2 = mCamera->projection() * mCamera->view() * lWorld2;

            mLighting->worldMatrix(lWorld2);
            mLighting->WVP(lWVP2);

            mat4f lLightWVP2 = lLightCamera.projection() * lLightCamera.view() * lWorld2;
            mLighting->lightWVP(lLightWVP2);

            it->second.mesh->render();
        }
    }
}
