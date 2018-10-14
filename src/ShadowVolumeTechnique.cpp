//===============================================================================================//
/*!
 *  \file      ShadowVolumeTechnique.cpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#include "ShadowVolumeTechnique.hpp"

using std::map;
using std::vector;
using std::string;
using std::make_unique;
using std::make_shared;
using std::shared_ptr;
using std::static_pointer_cast;
using std::get;
using std::tuple;
using miniGL::ShadowVolumeTechnique;
using miniGL::RenderingTechniqueBase;
using miniGL::Lighting;
using miniGL::MeshAndTransform;
using miniGL::BaseLight;

ShadowVolumeTechnique::ShadowVolumeTechnique(void)
:RenderingTechniqueBase("ShadowVolumeTechnique")
{
}

void ShadowVolumeTechnique::init(unsigned int pPointLightCount, const tuple<unsigned int, unsigned int> & pFramebufferDimensions)
{
    // Initialize rendering techniques
    mLighting = make_unique<Lighting>();
    assert(pPointLightCount == 1 && "This rendering technique only creates shadows for 1 point light for the moment");
    const unsigned int lSpotLightCount = 0u; // In this case, the lighting renderer only deals with point lights
    mLighting->init(pPointLightCount, lSpotLightCount);

    mNullRender = make_unique<NullRender>();
    mNullRender->init();

    mShadowVolume = make_unique<ShadowVolumeRender>();
    mShadowVolume->init();
}

void ShadowVolumeTechnique::render(const map<string, MeshAndTransform> & pMeshes, const vector<shared_ptr<BaseLight>> & pLights)
{
    // Find the meshes to render
    vector<map<string, MeshAndTransform>::const_iterator> lMeshAdjacenciesReferences;
    lMeshAdjacenciesReferences.reserve(mMeshToRenderNames.size());

    for (auto it = pMeshes.cbegin(); it != pMeshes.cend(); it++)
    {
        for (const auto name : mMeshWithAdjacenciesNames)
        {
            if (it->first == name)
            {
                lMeshAdjacenciesReferences.push_back(it);
            }
        }
    }

    // Find the graphical meshes to render
    auto lMeshReferences = findMeshesToRender(pMeshes);

    // Find the mesh representing the "floor"
    auto lFloorIterator = pMeshes.find(mFloorMeshName);
    assert(lFloorIterator != pMeshes.end());

    glDepthMask(GL_TRUE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    _renderSceneIntoDepth(lMeshAdjacenciesReferences, lFloorIterator);
    glEnable(GL_STENCIL_TEST);
    _renderShadowVolumeIntoStencil(lMeshAdjacenciesReferences, pLights);
    _renderShadowedScene(lMeshAdjacenciesReferences, lFloorIterator, pLights);
    glDisable(GL_STENCIL_TEST);
    _renderAmbientLight(lMeshReferences, lFloorIterator);
}

void ShadowVolumeTechnique::floor(const string & pName)
{
    mFloorMeshName = pName;
}

void ShadowVolumeTechnique::addMeshWithAdjacenciesToRender(const string & pName)
{
    mMeshWithAdjacenciesNames.push_back(pName);
}

const vector<string> & ShadowVolumeTechnique::meshWithAdjacenciesNames(void) const
{
    return mMeshWithAdjacenciesNames;
}

void ShadowVolumeTechnique::pointLightToUseDuringRender(unsigned int pIndex)
{
    mPointLightIndex = pIndex;

    // Since this technique only takes into account 1 point light, assign it directly to the Lighting renderer
    mLighting->lightToUseDuringRender(pIndex);
}

void ShadowVolumeTechnique::_renderSceneIntoDepth(vector<map<string, MeshAndTransform>::const_iterator> & pMeshAdjacenciesIterators, map<string, MeshAndTransform>::const_iterator & pFloorIterator)
{
    glDrawBuffer(GL_NONE);

    mNullRender->use();

    // Render the meshes
    for (auto it : pMeshAdjacenciesIterators)
    {
        for (auto transform : it->second.transform)
        {
            mat4f lWorld = transform.final();
            mat4f lWVP = mCamera->projection() * mCamera->view() * lWorld;

            mNullRender->WVP(lWVP);

            it->second.mesh->render();
        }
    }

    // Render the "floor"
    for (auto transform : pFloorIterator->second.transform)
    {
        mat4f lWorld = transform.final();
        mat4f lWVP = mCamera->projection() * mCamera->view() * lWorld;

        mNullRender->WVP(lWVP);

        pFloorIterator->second.mesh->render();
    }
}

void ShadowVolumeTechnique::_renderShadowVolumeIntoStencil(vector<map<string, MeshAndTransform>::const_iterator> & pMeshAdjacenciesIterators, vector<shared_ptr<BaseLight>> pLights)
{
    glDepthMask(GL_FALSE);
    glEnable(GL_DEPTH_CLAMP);
    glDisable(GL_CULL_FACE);

    // The stencil test must be enabled and always succeed.
    // Only the depth test matters
    glStencilFunc(GL_ALWAYS, 0, 0xff);

    glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
    glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);

    mShadowVolume->use();

    assert(pLights.at(mPointLightIndex)->type() == BaseLight::EType::POINT);
    mShadowVolume->lightPosition(static_pointer_cast<PointLight>(pLights.at(mPointLightIndex))->position());

    // Render the occluder
    for (auto it : pMeshAdjacenciesIterators)
    {
        for (auto transform : it->second.transform)
        {
            const auto lWorld = transform.final();
            mShadowVolume->WVP(mCamera->projection() * mCamera->view() * lWorld);
            it->second.mesh->render();
        }
    }

    // Restore previous state
    glDisable(GL_DEPTH_CLAMP);
    glEnable(GL_CULL_FACE);
}

void ShadowVolumeTechnique::_renderShadowedScene(vector<map<string, MeshAndTransform>::const_iterator> & pMeshAdjacenciesIterators, map<string, MeshAndTransform>::const_iterator & pFloorIterator, vector<shared_ptr<BaseLight>> pLights)
{
    glDrawBuffer(GL_BACK);

    // Draw only if the corresponding stencil value is zero
    glStencilFunc(GL_EQUAL, 0x0, 0xff);

    // Prevent update to the stencil buffer
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

    mLighting->use();
    mLighting->useNormalMap(false);
    mLighting->useShadowMap(false);
    mLighting->updateLightsState(pLights);

    mLighting->eyeWorldPosition(mCamera->position());

    for (auto it : pMeshAdjacenciesIterators)
    {
        for (auto transformation : it->second.transform)
        {
            mat4f lWorld = transformation.final();
            mat4f lWVP = mCamera->projection() * mCamera->view() * lWorld;

            mLighting->worldMatrix(lWorld);
            mLighting->WVP(lWVP);

            it->second.mesh->render();
        }
    }

    assert(pFloorIterator->second.transform.size() == 1 && "Assumed the floor has only one tranform");
    mat4f lWorld = pFloorIterator->second.transform[0].final();
    mat4f lWVP = mCamera->projection() * mCamera->view() * lWorld;

    mLighting->worldMatrix(lWorld);
    mLighting->WVP(lWVP);

    // Render the "floor" with the shadows from the spot light
    pFloorIterator->second.mesh->render();

}

void ShadowVolumeTechnique::_renderAmbientLight(vector<map<string, MeshAndTransform>::const_iterator> & pMeshIterators, map<string, MeshAndTransform>::const_iterator & pFloorIterator)
{
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_ONE, GL_ONE);

    for (auto it : pMeshIterators)
    {
        for (auto transformation : it->second.transform)
        {
            mat4f lWorld2 = transformation.final();
            mat4f lWVP2 = mCamera->projection() * mCamera->view() * lWorld2;

            mLighting->worldMatrix(lWorld2);
            mLighting->WVP(lWVP2);

            it->second.mesh->render();
        }
    }

    assert(pFloorIterator->second.transform.size() == 1 && "Assumed the floor has only one tranform");
    mat4f lWorld = pFloorIterator->second.transform[0].final();
    mat4f lWVP = mCamera->projection() * mCamera->view() * lWorld;

    mLighting->worldMatrix(lWorld);
    mLighting->WVP(lWVP);

    // Render the "floor" with the shadows from the spot light
    pFloorIterator->second.mesh->render();

    glDisable(GL_BLEND);
}
