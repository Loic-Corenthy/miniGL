//===============================================================================================//
/*!
 *  \file      Tessellation.cpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#include "Tessellation.hpp"

#include "EngineCommon.hpp"

using std::string;
using std::shared_ptr;
using std::make_unique;
using std::map;
using std::vector;
using miniGL::Camera;
using miniGL::Tessellation;
using miniGL::MeshAndTransform;
using miniGL::BaseLight;

void Tessellation::init(unsigned int pPointLightCount, const string & pDisplacementMapFilename)
{
    // Setup the lighting for the tessellation shaders
    mTessellationLighting = make_unique<TessellationLighting>(TessellationLighting::EMode::DEFAULT);

    // Set number of vertices per patch (vertices used by the vertex shader)
//    glPatchParameteri(GL_PATCH_VERTICES, 3); checkOpenGLState;

    const auto lSpotLightCount = 0u;
    mTessellationLighting->init(pPointLightCount, lSpotLightCount);

    // Setup the displacement map
    mDisplacementMap.target(GL_TEXTURE_2D);
    mDisplacementMap.loadImage(pDisplacementMapFilename);
    mDisplacementMap.bind(DISPLACEMENT_TEXTURE_UNIT);
}

void Tessellation::render(map<string, MeshAndTransform> & pMeshes, const vector<shared_ptr<BaseLight>> & pLights)
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
    mTessellationLighting->maxDistanceToCameraCoeff(10.0f);

    mat4f lVP = mCamera->projection() * mCamera->view();
    mTessellationLighting->VP(lVP);

    mTessellationLighting->displacementFactor(mDisplacementFactor);

    mTessellationLighting->updateLightsState(pLights);

    mDisplacementMap.bind(DISPLACEMENT_TEXTURE_UNIT);

    // Disable face culling for tessalation as apparently, we cannot garantee the orientation of the generated triangles
    glDisable(GL_CULL_FACE);

    for (auto it : lMeshReferences)
    {
        for (auto transformation : it->second.transform)
        {
            mat4f lWorld = transformation.final();
            mTessellationLighting->worldMatrix(lWorld);

            it->second.mesh->render(MeshBase::EPrimitiveType::PATCH);
        }
    }

    glEnable(GL_CULL_FACE);
}

void Tessellation::camera(const shared_ptr<Camera> pCamera)
{
    mCamera = pCamera;
}

void Tessellation::addMeshToRender(const string & pName)
{
    mMeshToRenderNames.push_back(pName);
}

void Tessellation::displacementFactor(float pValue) noexcept
{
    mDisplacementFactor = pValue;
}

float Tessellation::displacementFactor(void) const noexcept
{
    return mDisplacementFactor;
}
