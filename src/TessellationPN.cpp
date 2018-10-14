//===============================================================================================//
/*!
 *  \file      TessellationPN.cpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#include "TessellationPN.hpp"

#include "EngineCommon.hpp"
#include "GLUtils.hpp"

using std::string;
using std::shared_ptr;
using std::make_unique;
using std::map;
using std::vector;
using miniGL::Camera;
using miniGL::TessellationPN;
using miniGL::MeshAndTransform;
using miniGL::BaseLight;

void TessellationPN::init(unsigned int pPointLightCount)
{
    // Setup the lighting for the tessellation shaders
    mTessellationLighting = make_unique<TessellationLighting>(TessellationLighting::EMode::POINT_NORMAL);

    const auto lSpotLightCount = 0u;
    mTessellationLighting->init(pPointLightCount, lSpotLightCount, string(R"(./Shaders/TessellationPNLighting.vert)"), string(R"(./Shaders/TessellationPNLighting.ctrl)"), string(R"(./Shaders/TessellationPNLighting.eval)"), string(R"(./Shaders/TessellationPNLighting.frag)"));

    // Set number of vertices per patch (vertices used by the vertex shader)
    glPatchParameteri(GL_PATCH_VERTICES, 3); checkOpenGLState;

    //    _validateShaderWithMesh(mTessellationPNLighting, lName);
}

void TessellationPN::render(map<string, MeshAndTransform> & pMeshes, const vector<shared_ptr<BaseLight>> & pLights)
{
    // Find the meshes to render
    vector<map<string, MeshAndTransform>::iterator> lMeshReferences;
    lMeshReferences.reserve(mMeshToRenderNames.size());

    for (auto it = pMeshes.begin(); it != pMeshes.end(); it++)
    {
        for (const auto name : mMeshToRenderNames)
        {
            if (it->first == name)
            {
                lMeshReferences.push_back(it);
            }
        }
    }

    mTessellationLighting->use();
    mTessellationLighting->eyeWorldPosition(mCamera->position());
    mTessellationLighting->useSampler(false);

    mat4f lVP = mCamera->projection() * mCamera->view();
    mTessellationLighting->VP(lVP);

    mTessellationLighting->updateLightsState(pLights);

    unsigned int i = 0;
    for (auto it : lMeshReferences)
    {
        for (auto transformation : it->second.transform)
        {
            mat4f lWorld = transformation.final();
            mTessellationLighting->worldMatrix(lWorld);

            mTessellationLighting->tessellationLevel(mTessellationLevel.at(i++));
            it->second.mesh->render(MeshBase::EPrimitiveType::PATCH);
        }
    }
}

void TessellationPN::camera(const shared_ptr<Camera> pCamera)
{
    mCamera = pCamera;
}

void TessellationPN::addMeshToRender(const string & pName)
{
    mMeshToRenderNames.push_back(pName);
}

void TessellationPN::addTessellationLevel(float pValue)
{
    mTessellationLevel.push_back(pValue);
}

void TessellationPN::tessellationLevel(unsigned int pIndex, float pValue)
{
    assert(pIndex < mTessellationLevel.size());
    mTessellationLevel[pIndex] = pValue;
}

float TessellationPN::tessellationLevel(unsigned int pIndex) const
{
    return mTessellationLevel.at(pIndex);
}

