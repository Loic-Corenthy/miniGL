//===============================================================================================//
/*!
 *  \file      SilhouetteTechnique.cpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#include "SilhouetteTechnique.hpp"

#include "PointLight.hpp"
#include "SpotLight.hpp"

using std::map;
using std::vector;
using std::string;
using std::make_unique;
using std::make_shared;
using std::shared_ptr;
using std::static_pointer_cast;
using std::get;
using std::tuple;
using miniGL::SilhouetteTechnique;
using miniGL::RenderingTechniqueBase;
using miniGL::MeshAndTransform;
using miniGL::BaseLight;

SilhouetteTechnique::SilhouetteTechnique(void)
:RenderingTechniqueBase("SilhouetteTechnique")
{
}

void SilhouetteTechnique::init(void)
{
    mSilhouetteRender = make_unique<SilhouetteRender>();
    mSilhouetteRender->init();
}

void SilhouetteTechnique::render(const map<string, MeshAndTransform> & pMeshes, const vector<shared_ptr<BaseLight>> & pLights)
{
    mSilhouetteRender->use();

    assert(mMeshToRenderNames.size() == 1 && "Rendering only one mesh for the moment");

    for (auto it = pMeshes.begin(); it != pMeshes.end(); it++)
    {
        for (const auto name : mMeshToRenderNames)
        {
            if (it->first == name)
            {
                // To be able to render the silhouette of the mesh, the latter needs to be loaded with adjacencies
                assert(it->second.mesh->loadOption() == MeshBase::EOptions::ADJACENCIES);

                for (auto transformation : it->second.transform)
                {
                    mat4f lWorld = transformation.final();
                    mat4f lWVP = mCamera->projection() * mCamera->view() * lWorld;

                    mSilhouetteRender->WVP(lWVP);
                    mSilhouetteRender->worldMatrix(lWorld);

                    if (pLights.at(mLightIndex)->type() == BaseLight::EType::POINT)
                        mSilhouetteRender->lightPosition(static_pointer_cast<PointLight>(pLights.at(mLightIndex))->position());
                    else if (pLights.at(mLightIndex)->type() == BaseLight::EType::SPOT)
                        mSilhouetteRender->lightPosition(static_pointer_cast<SpotLight>(pLights.at(mLightIndex))->position());
                    else
                        assert(false && "Light position must come from a point or a spot light");

                    it->second.mesh->render();
                }
            }
        }
    }
}

void SilhouetteTechnique::lightIndex(unsigned int pIndex)
{
    mLightIndex = pIndex;
}
