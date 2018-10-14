//===============================================================================================//
/*!
 *  \file      DeferredShadingTechnique.cpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#include "DeferredShadingTechnique.hpp"

#include "EngineCommon.hpp"
#include "EnumClassCast.hpp"
#include "MeshSOA.hpp"
#include "DirectionalLight.hpp"
#include "SpotLight.hpp"
#include "PointLight.hpp"
#include "GLUtils.hpp"

using std::map;
using std::vector;
using std::string;
using std::make_unique;
using std::make_shared;
using std::shared_ptr;
using std::static_pointer_cast;
using std::get;
using std::tuple;
using miniGL::DeferredShadingTechnique;
using miniGL::RenderingTechniqueBase;
using miniGL::MeshSOA;
using miniGL::MeshAndTransform;
using miniGL::BaseLight;
using miniGL::PointLight;
using miniGL::DirectionalLight;
using miniGL::SpotLight;

DeferredShadingTechnique::DeferredShadingTechnique(void)
:RenderingTechniqueBase("DeferredShadingTechnique")
{
}

DeferredShadingTechnique::~DeferredShadingTechnique()
{
    // Re activate glDepthMask to be able to use the depth test (which is the default behaviour)
    glDepthMask(GL_TRUE);
}

void DeferredShadingTechnique::init(unsigned int pPointLightCount, unsigned int pSpotLightCount, const tuple<unsigned int, unsigned int> & pFramebufferDimensions)
{
    mFramebufferWidth = get<0>(pFramebufferDimensions);
    mFramebufferHeight = get<1>(pFramebufferDimensions);

    // Create and initialize geometry buffer
    mGeometryBuffers = make_unique<GBuffer>();
    mGeometryBuffers->init(mFramebufferWidth, mFramebufferHeight);

    // Create and initialize geometry pass
    mDSGeometryPass = make_unique<DeferredShadingGeometryPass>();
    mDSGeometryPass->init();
    mDSGeometryPass->use();
    mDSGeometryPass->colorTextureUnit(COLOR_TEXTURE_UNIT_INDEX);

    // Directional light
    mDSDirectionalLightPass = make_unique<DeferredShadingDirectionalLightPass>();
    mDSDirectionalLightPass->init();
    mDSDirectionalLightPass->use();
    mDSDirectionalLightPass->positionTextureUnit(toUT(GBuffer::ETextureType::POSITION));
    mDSDirectionalLightPass->colorTextureUnit(toUT(GBuffer::ETextureType::DIFFUSE));
    mDSDirectionalLightPass->normalTextureUnit(toUT(GBuffer::ETextureType::NORMAL));
    mDSDirectionalLightPass->screenSize(mFramebufferWidth, mFramebufferHeight);
    mDSDirectionalLightPass->WVP(mat4f(1.0f));

    // Point lights
    mDSPointLightPass = make_unique<DeferredShadingPointLightPass>();
    mDSPointLightPass->init();
    mDSPointLightPass->use();
    mDSPointLightPass->positionTextureUnit(toUT(GBuffer::ETextureType::POSITION));
    mDSPointLightPass->colorTextureUnit(toUT(GBuffer::ETextureType::DIFFUSE));
    mDSPointLightPass->normalTextureUnit(toUT(GBuffer::ETextureType::NORMAL));
    mDSPointLightPass->screenSize(mFramebufferWidth, mFramebufferHeight);

    // Spot lights
    mDSSpotLightPass = make_unique<DeferredShadingSpotLightPass>();
    mDSSpotLightPass->init();
    mDSSpotLightPass->use();
    mDSSpotLightPass->positionTextureUnit(toUT(GBuffer::ETextureType::POSITION));
    mDSSpotLightPass->colorTextureUnit(toUT(GBuffer::ETextureType::DIFFUSE));
    mDSSpotLightPass->normalTextureUnit(toUT(GBuffer::ETextureType::NORMAL));
    mDSSpotLightPass->screenSize(mFramebufferWidth, mFramebufferHeight);

    // Create and initialize the null pass
    mDSNullPass = make_unique<DeferredShadingNullPass>();
    mDSNullPass->init();

    // Create quad
    mQuad.mesh = make_shared<MeshSOA>(string("quad"));
    mQuad.mesh->load(R"(./quad.obj)");
    mQuad.mesh->frontFace(GL_CW);

    // Sphere
    mSphere.mesh = make_shared<MeshSOA>(string("sphere"));
    mSphere.mesh->load(R"(./sphere.obj)");
    mSphere.mesh->frontFace(GL_CW);
}

void DeferredShadingTechnique::render(const map<string, MeshAndTransform> & pMeshes, const vector<shared_ptr<BaseLight>> & pLights)
{
    // Find the meshes to render
    auto lMeshReferences = findMeshesToRender(pMeshes);

    mGeometryBuffers->startFrame();

    // Process the geometry of all the meshes that we want to render
    _geometryPass(lMeshReferences);

    // We need stencil to be enabled in the stencil pass to get the stencil buffer
    // updated and we also need it in the light pass because we render the light
    // only if the stencil passes.
    glEnable(GL_STENCIL_TEST);

    // Process the point lights
    for (const auto it : pLights)
    {
        if (it ->type() == BaseLight::EType::POINT)
        {
            auto lPointLight = static_pointer_cast<PointLight>(it);
            _stencilPass(lPointLight);
            _pointLightPass(lPointLight);
        }
    }

    // Process the spot lights
    for (const auto it : pLights)
    {
        if (it ->type() == BaseLight::EType::SPOT)
        {
            auto lSpotLight = static_pointer_cast<SpotLight>(it);
            _stencilPass(lSpotLight);
            _spotLightPass(lSpotLight);
        }
    }

    // The directional light does not need a stencil test because its volume
    // is unlimited and the final pass simply copies the texture.
    glDisable(GL_STENCIL_TEST);

    // Process the directional light
    _directionalLightPass(pLights);

    _finalPass();
}

void DeferredShadingTechnique::_geometryPass(vector<map<string, MeshAndTransform>::const_iterator> & pMeshIterators)
{
    mDSGeometryPass->use();
    mGeometryBuffers->bindForGeometryPass();

    // Only the geometry pass updates the depth buffer
    glDepthMask(GL_TRUE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    for (const auto it : pMeshIterators)
    {
        for (auto trans : it->second.transform)
        {
            mat4f lWorld = trans.final();
            mat4f lWVP = mCamera->projection() * mCamera->view() * lWorld;

            mDSGeometryPass->worldMatrix(lWorld);
            mDSGeometryPass->WVP(lWVP);

            glFrontFace(it->second.mesh->frontFace());

            it->second.mesh->render();
        }
    }

    // When we get here the depth buffer is already populated and the stencil pass
    // depends on it, but it does not write to it.
    glDepthMask(GL_FALSE);
}

void DeferredShadingTechnique::_finalPass(void)
{
    mGeometryBuffers->bindForFinalPass();

    glBlitFramebuffer(0, 0, mFramebufferWidth, mFramebufferHeight, 0, 0, mFramebufferWidth, mFramebufferHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR); checkOpenGLState;
}

void DeferredShadingTechnique::_directionalLightPass(const vector<shared_ptr<BaseLight>> & pLights)
{
    mGeometryBuffers->bindForLightPass();

    mDSDirectionalLightPass->use();
    mDSDirectionalLightPass->eyeWorldPosition(mCamera->position());

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_ONE, GL_ONE);

    for (const auto it : pLights)
    {
        if (it->type() == BaseLight::EType::DIRECTIONAL)
        {
            mDSDirectionalLightPass->updateLightsState(static_pointer_cast<DirectionalLight>(it));

            // The mesh in this case is just to select the points on screen that we want to apply the fragment shader to, i.e.
            // this mesh is NOT a mesh we want to render on screen (those were renderered in the geometry pass)
            // Use a quad as we want to render the whole frame (the directional light affects all the objects in the scene)
            mQuad.mesh->render();
        }
    }

    glDisable(GL_BLEND);
}

void DeferredShadingTechnique::_pointLightPass(const shared_ptr<PointLight> pLight)
{
    mGeometryBuffers->bindForLightPass();

    mDSPointLightPass->use();
    mDSPointLightPass->eyeWorldPosition(mCamera->position());

    glStencilFunc(GL_NOTEQUAL, 0, 0xFF);

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_ONE, GL_ONE);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    Transform lTransformation;

    const auto & lPosition = pLight->position();
    lTransformation.translation(lPosition.x(), lPosition.y(), -lPosition.z());

    const float lSphereScale = _sphereRadius(pLight);

    lTransformation.scaling(lSphereScale, lSphereScale, lSphereScale);
    mat4f lWorld = lTransformation.final();
    mat4f lWVP = mCamera->projection() * mCamera->view() * lWorld;

    mDSPointLightPass->WVP(lWVP);
    mDSPointLightPass->updateLightState(*pLight);

    // The mesh in this case is just to select the points on screen that we want to apply the fragment shader to, i.e.
    // this mesh is NOT a mesh we want to render on screen (those were renderered in the geometry pass)
    // Use a sphere as we want to render all the object within the "reach" of the current point light, i.e. the size of
    // sphere lSphereScale is function of the intensity of the point light.
    mSphere.mesh->render();

    glCullFace(GL_BACK);
    glDisable(GL_BLEND);
}

void DeferredShadingTechnique::_spotLightPass(const shared_ptr<SpotLight> pLight)
{
    mGeometryBuffers->bindForLightPass();

    mDSSpotLightPass->use();
    mDSSpotLightPass->eyeWorldPosition(mCamera->position());

    glStencilFunc(GL_NOTEQUAL, 0, 0xFF);

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_ONE, GL_ONE);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    Transform lTransformation;

    const auto & lPosition = pLight->position();
    lTransformation.translation(lPosition.x(), lPosition.y(), -lPosition.z());

    float lSphereScale = _sphereRadius(pLight);

    lTransformation.scaling(lSphereScale, lSphereScale, lSphereScale);
    mat4f lWorld = lTransformation.final();
    mat4f lWVP = mCamera->projection() * mCamera->view() * lWorld;

    mDSSpotLightPass->WVP(lWVP);
    mDSSpotLightPass->updateLightState(*pLight);

    // The mesh in this case is just to select the points on screen that we want to apply the fragment shader to, i.e.
    // this mesh is NOT a mesh we want to render on screen (those were renderered in the geometry pass)
    // Use a sphere as we want to render all the object within the "reach" of the current spot light, i.e. the size of
    // sphere lSphereScale is function of the intensity of the spot light.
    mSphere.mesh->render();

    glCullFace(GL_BACK);
    glDisable(GL_BLEND);
}
