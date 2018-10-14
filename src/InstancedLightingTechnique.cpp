//===============================================================================================//
/*!
 *  \file      InstancedLightingTechnique.cpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#include "InstancedLightingTechnique.hpp"

using std::vector;
using std::map;
using std::make_unique;
using std::shared_ptr;
using std::string;
using std::get;
using std::tuple;
using miniGL::InstancedLightingTechnique;
using miniGL::RenderingTechniqueBase;
using miniGL::MeshAndTransform;
using miniGL::BaseLight;

InstancedLightingTechnique::InstancedLightingTechnique(void)
:RenderingTechniqueBase("InstancedLightingTechnique")
{
}

void InstancedLightingTechnique::init(unsigned int pPointLightCount, const tuple<unsigned int, unsigned int> & pFramebufferDimensions)
{
    mInstancedLighting = make_unique<InstancedLighting>();
    mInstancedLighting->init(pPointLightCount, 0u);
    mInstancedLighting->shadowMapSize(get<0>(pFramebufferDimensions), get<1>(pFramebufferDimensions));
}

void InstancedLightingTechnique::render(const map<string, MeshAndTransform> & pMeshes, const vector<shared_ptr<BaseLight>> & pLights)
{
    mInstancedLighting->use();
    mInstancedLighting->useNormalMap(false);
    mInstancedLighting->useShadowMap(false);

    mInstancedLighting->eyeWorldPosition(mCamera->position());

    mInstancedLighting->updateLightsState(pLights);

    assert(mMeshToRenderNames.size() == 1 && "Rendering only one mesh for the moment");

    for (auto it = pMeshes.cbegin(); it != pMeshes.cend(); it++)
    {
        for (const auto name : mMeshToRenderNames)
        {
            if (it->first == name)
            {
                // Container for all the WVP and world matrices that will be sent to the GPU to render the mesh at different postions
                vector<mat4f> lWVPs(mInstancePositions.size());
                vector<mat4f> lWorlds(mInstancePositions.size());

                for (unsigned int i = 0; i < mInstancePositions.size(); ++i)
                {
                    auto lTransform = it->second.transform[0];
                    const auto lUpdatedPosition = mInstancePositions[i] + (mInstanceVelocities[i] * mInstanceVelocitiesMultiplier);
                    lTransform.translation(lUpdatedPosition.x(), lUpdatedPosition.y(), lUpdatedPosition.z());
                    lWorlds[i] = lTransform.final();

                    lWVPs[i] = mCamera->projection() * mCamera->view() * lWorlds[i];

                    lWorlds[i].transpose();
                    lWVPs[i].transpose();
                }

                it->second.mesh->render(mInstancePositions.size(), lWVPs.data(), lWorlds.data());
            }
        }
    }
}

void InstancedLightingTechnique::instancePositions(const vector<vec3f> & pInstancePositions)
{
    mInstancePositions = pInstancePositions;
}

void InstancedLightingTechnique::instanceVelocities(const vector<vec3f> & pInstanceVelocities)
{
    mInstanceVelocities = pInstanceVelocities;
}

void InstancedLightingTechnique::instanceVelocitiesMultiplier(float pValue)
{
    mInstanceVelocitiesMultiplier = pValue;
}
