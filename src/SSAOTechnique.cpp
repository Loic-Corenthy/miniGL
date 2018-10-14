//===============================================================================================//
/*!
 *  \file      SSAOTechnique.cpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#include "SSAOTechnique.hpp"

#include "EnumClassCast.hpp"

using std::map;
using std::vector;
using std::string;
using std::make_unique;
using std::make_shared;
using std::shared_ptr;
using std::get;
using std::tuple;
using miniGL::SSAOTechnique;
using miniGL::RenderingTechniqueBase;
using miniGL::MeshAndTransform;
using miniGL::BaseLight;

SSAOTechnique::SSAOTechnique(void)
:RenderingTechniqueBase("SSAOTechnique")
{
}

void SSAOTechnique::init(unsigned int pPointLightCount, unsigned int pSpotLightCount, const tuple<unsigned int, unsigned int> & pFramebufferDimensions)
{
    if (!mCamera)
        throw Exceptions("Must set the camera before initializing this rendering technique", __FILE__, __LINE__);

    // Create and initialize geometry pass
    mSSAOGeometryPass = make_unique<SSAOGeometryPass>();
    mSSAOGeometryPass->init();

    // Create and initialize render pass
    mSSAORender = make_unique<SSAORender>();
    mSSAORender->init();
    mSSAORender->use();
    mSSAORender->sampleRadius(1.5f);
    mSSAORender->projection(mCamera->projection());
    mSSAORender->aspectRatio(mCamera->aspectRatio());
    mSSAORender->FOV(mCamera->verticalFoV());

    // Create and initialize lighting pass
    mSSAOLighting = make_unique<SSAOLighting>();
    mSSAOLighting->init(pPointLightCount, pSpotLightCount);
    enum E { WIDTH = 0, HEIGHT = 1};
    mSSAOLighting->screenSize(get<E::WIDTH>(pFramebufferDimensions), get<E::HEIGHT>(pFramebufferDimensions));
    mSSAOLighting->shaderType(toUT(mSSAOShaderType));

    // Create and initialize blur pass
    mSSAOBlur = make_unique<SSAOBlur>();
    mSSAOBlur->init();

    // Initialize IO buffers
    mSSAODepthBuffer.init(get<E::WIDTH>(pFramebufferDimensions), get<E::HEIGHT>(pFramebufferDimensions), true, GL_NONE);
    mSSAOAOBuffer.init(get<E::WIDTH>(pFramebufferDimensions), get<E::HEIGHT>(pFramebufferDimensions), false, GL_R32F);
    mSSAOBlurBuffer.init(get<E::WIDTH>(pFramebufferDimensions), get<E::HEIGHT>(pFramebufferDimensions), false, GL_R32F);

    // Create quad mesh for internal rendering
    mQuad.load(R"(./quad_no_texture.obj)", MeshBase::EOptions::UNSET);
    mQuad.frontFace(GL_CW);
}

void SSAOTechnique::render(const map<string, MeshAndTransform> & pMeshes, const vector<shared_ptr<BaseLight>> & pLights)
{
    // Find the meshes to render
    auto lMeshReferences = findMeshesToRender(pMeshes);

    // Summary of steps required to populate depth buffer
    //  - We begin with the object space position of a vertex and multiply it with the WVP matrix which is a combined transformations of local-to-world,
    //  world-to-view and projection from view on the near clipping plane.
    //  - The result is a 4D vector with the view space Z value in the fourth component. We say that this vector is in clip space at this point.
    //  - The clip space vector goes into the gl_Position output vector from the vertex shader and the GPU clips its first three components
    //  between -W and W (W is the fourth component with the view space Z value).
    //  - Next the GPU performs perspective divide which means that the vector is divided by W. Now the first three components are between
    //  -1 and 1 and the last component is simply 1.
    //  - We say that at this point the vector is in NDC space (Normalized Device Coordinates).
    //  - Usually the vertex is just one out of three vertices comprising a triangle so the GPU interpolates between the three NDC vectors across
    //  the triangle face and executes the fragment shader on each pixel.
    //  - On the way out of the fragment shader the GPU updates the depth buffer with the Z component of the NDC vector (based on several state
    //  nobs that must be configured correctly such as depth testing, depth write, etc).
    //  - An important point to remember is that before writing the Z value to the depth buffer the GPU transforms it from (-1,1) to (0,1).
    //  We must handle this correctly or else we will get visual anomalies.

    _geometryPass(lMeshReferences);
    _SSAOPass();
    _blurPass();
    _lightingPass(lMeshReferences, pLights);
}

void SSAOTechnique::shaderType(SSAOTechnique::EShaderType pType)
{
    mSSAOShaderType = pType;
}

SSAOTechnique::EShaderType SSAOTechnique::shaderType(void) const noexcept
{
    return mSSAOShaderType;
}

void SSAOTechnique::_geometryPass(vector<map<string, MeshAndTransform>::const_iterator> & pMeshIterators)
{
    mSSAOGeometryPass->use();

    mSSAODepthBuffer.bindForWritting();

    glClear(GL_DEPTH_BUFFER_BIT);

    for (auto it : pMeshIterators)
    {
        for (auto transformation : it->second.transform)
        {
            mat4f lWorld = transformation.final();
            mat4f lWVP = mCamera->projection() * mCamera->view() * lWorld;

            mSSAOGeometryPass->WVP(lWVP);

            it->second.mesh->render();
        }
    }
}

void SSAOTechnique::_SSAOPass(void)
{
    mSSAORender->use();

    mSSAORender->bindDepthBuffer(mSSAODepthBuffer);

    mSSAOAOBuffer.bindForWritting();

    glClear(GL_COLOR_BUFFER_BIT);

    mQuad.render();
}

void SSAOTechnique::_blurPass(void)
{
    mSSAOBlur->use();

    mSSAOBlur->bindInputBuffer(mSSAOAOBuffer);

    mSSAOBlurBuffer.bindForWritting();

    glClear(GL_COLOR_BUFFER_BIT);

    mQuad.render();
}

void SSAOTechnique::_lightingPass(vector<map<string, MeshAndTransform>::const_iterator> & pMeshIterators, const vector<shared_ptr<BaseLight>> & pLights)
{
    mSSAOLighting->use();
    mSSAOLighting->updateLightsState(pLights);
    mSSAOLighting->shaderType(toUT(mSSAOShaderType));

    mSSAOLighting->bindAOBuffer(mSSAOBlurBuffer);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto it : pMeshIterators)
    {
        for (auto transformation : it->second.transform)
        {
            mat4f lWorld = transformation.final();
            mat4f lWVP = mCamera->projection() * mCamera->view() * lWorld;

            mSSAOLighting->WVP(lWVP);
            mSSAOLighting->world(lWorld);

            it->second.mesh->render();
        }
    }
}
