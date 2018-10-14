//===============================================================================================//
/*!
 *  \file      GLFXTechnique.cpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//


#include "GLFXTechnique.hpp"

using std::map;
using std::vector;
using std::string;
using std::make_unique;
using std::shared_ptr;
using miniGL::GLFXTechnique;
using miniGL::RenderingTechniqueBase;
using miniGL::MeshAndTransform;
using miniGL::BaseLight;

GLFXTechnique::GLFXTechnique(void)
:RenderingTechniqueBase("GLFXTechnique")
{
}

void GLFXTechnique::init(unsigned int pPointLightCount, unsigned int pSpotLightCount)
{
    mGLFXLighting = make_unique<GLFXLighting>();
    mGLFXLighting->init(pPointLightCount, pSpotLightCount);
}

void GLFXTechnique::render(const map<string, MeshAndTransform> & pMeshes, const vector<shared_ptr<BaseLight>> & pLights)
{
    mGLFXLighting->use();
    mGLFXLighting->useSampler(false);
    mGLFXLighting->updateLightsState(pLights);

    mGLFXLighting->eyeWorldPosition(mCamera->position());

    glDisable(GL_CULL_FACE);

    size_t i = 0;
    for (auto it = pMeshes.cbegin(); it != pMeshes.cend(); it++)
    {
        for (const auto name : mMeshToRenderNames)
        {
            if (it->first == name)
            {
                for (auto transformation : it->second.transform)
                {
                    mat4f lWorld = transformation.final();
                    mat4f lWVP = mCamera->projection() * mCamera->view() * lWorld;

                    mGLFXLighting->worldMatrix(lWorld);
                    mGLFXLighting->WVP(lWVP);
                    mGLFXLighting->uniformColor(mUniformColors[i].x(), mUniformColors[i].y(), mUniformColors[i].z());

                    if (i < mMeshToRenderNames.size() - 1)
                        i++;

                    it->second.mesh->render();
                }
            }
        }
    }

    glEnable(GL_CULL_FACE);
}

void GLFXTechnique::addUniformColor(float pRed, float pGreen, float pBlue)
{
    mUniformColors.emplace_back(pRed, pGreen, pBlue);
}
