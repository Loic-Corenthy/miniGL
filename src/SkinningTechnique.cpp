//===============================================================================================//
/*!
 *  \file      SkinningTechnique.cpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#include "SkinningTechnique.hpp"

#include "MeshSOA.hpp"

using std::map;
using std::vector;
using std::string;
using std::make_unique;
using std::make_shared;
using std::shared_ptr;
using std::get;
using std::tuple;
using miniGL::SkinningTechnique;
using miniGL::RenderingTechniqueBase;
using miniGL::MeshSOA;
using miniGL::MeshAndTransform;
using miniGL::BaseLight;

SkinningTechnique::SkinningTechnique(void)
:RenderingTechniqueBase("SkinningTechnique")
{
}

void SkinningTechnique::init(unsigned int pPointLightCount, unsigned int pSpotLightCount, bool pActivateMotionBlur, const std::tuple<unsigned int, unsigned int> & pFramebufferDimensions)
{
    mActivateMotionBlur = pActivateMotionBlur;

    mSkinning = make_unique<Skinning>();

    if (mActivateMotionBlur)
    {
        mIntermediateBuffer.init(get<0>(pFramebufferDimensions), get<1>(pFramebufferDimensions));

        mSkinning->init(pPointLightCount, pSpotLightCount, true, string(string(R"(./Shaders/SkinningWithMotionBlur.vert)")), string(string(R"(./Shaders/SkinningWithMotionBlur.frag)")));

        mMotionBlur = make_unique<MotionBlur>();

        mMotionBlur->init();

        mMotionBlur->use();
        mMotionBlur->colorTextureUnit(0);
        mMotionBlur->motionTextureUnit(1);

        // Initialize quad
        mQuad.mesh = make_shared<MeshSOA>(string("quad"));
        mQuad.mesh->load(R"(./quad_r_no_texture.obj)");
        mQuad.mesh->frontFace(GL_CW);
    }
    else
    {
        mSkinning->init(pPointLightCount, pSpotLightCount);
    }
}

void SkinningTechnique::render(const std::map<std::string, MeshAndTransform> & pMeshes, const vector<shared_ptr<BaseLight>> & pLights)
{
    // The rendering follows two "different paths" according to the value of mActivateMotionBlur
    // If we render WITHOUT motion blur, then we directly render to screen
    // If we render WITH motion blur, we first render to a texture in the intermediate buffers and then render to the screen using the quad

    if (mActivateMotionBlur)
    {
        mIntermediateBuffer.bindForWritting();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    mSkinning->use();
    mSkinning->useNormalMap(false);
    mSkinning->useShadowMap(false);

    mSkinning->eyeWorldPosition(mCamera->position());

    mSkinning->updateLightsState(pLights);

    assert(mMeshToRenderNames.size() == 1 && "Rendering only one mesh for the moment");

    for (auto it = pMeshes.cbegin(); it != pMeshes.cend(); it++)
    {
        for (const auto name : mMeshToRenderNames)
        {
            if (it->first == name)
            {
                // The following code is ran only once during the first call to render to initialize the previous transformations with time t = 0.
                // This works because we are assuming only 1 mesh to render. If we wanted to render multiple meshes with this technique, we would need an array of bools
                // to ensure each one gets its transforms initialized
                if (mActivateMotionBlur && mInitializePreviousTransforms)
                {
                    it->second.mesh->boneTransform(0.0f, mPreviousBoneTransforms);
                    mInitializePreviousTransforms = false;
                }

                // Update and get all the bone transform matrices from the mesh
                vector<mat4f> lTransforms;
                it->second.mesh->boneTransform(mRunningTime, lTransforms);

                // Update all bone transform matrices in the shader
                for (unsigned int i = 0; i < lTransforms.size(); ++i)
                {
                    mSkinning->boneTransforms(i, lTransforms[i]);

                    if (mActivateMotionBlur)
                        mSkinning->previousBoneTransforms(i, mPreviousBoneTransforms[i]);
                }

                for (auto transformation : it->second.transform)
                {
                    mat4f lWorld = transformation.final();
                    mat4f lWVP = mCamera->projection() * mCamera->view() * lWorld;

                    mSkinning->worldMatrix(lWorld);
                    mSkinning->WVP(lWVP);

                    it->second.mesh->render();
                }
            }
        }
    }

    if (mActivateMotionBlur)
    {
        // Motion blur pass
        mIntermediateBuffer.bindForReading();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mMotionBlur->use();

        mQuad.mesh->render();
    }
}

void SkinningTechnique::runningTime(float pRunningTime)
{
    mRunningTime = pRunningTime;
}
