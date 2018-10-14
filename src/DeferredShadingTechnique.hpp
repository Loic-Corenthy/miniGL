//===============================================================================================//
/*!
 *  \file      DeferredShadingTechnique.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

#include <map>
#include <vector>

#include "RenderingTechniqueBase.hpp"
#include "MeshAndTransform.hpp"
#include "BaseLight.hpp"
#include "GBuffer.hpp"
#include "DeferredShadingGeometryPass.hpp"
#include "DeferredShadingLightPass.hpp"
#include "DeferredShadingDirectionalLightPass.hpp"
#include "DeferredShadingPointLightPass.hpp"
#include "DeferredShadingSpotLightPass.hpp"
#include "DeferredShadingNullPass.hpp"

namespace miniGL
{
    /*!
     *  \brief  This class encapsulates all the details to handle deferred shading
     *  \details It takes care of all the different rendering passes and the buffers that are used to store
     *           the data
     */
    class DeferredShadingTechnique : public RenderingTechniqueBase
    {
    public:
        /*!
         *  \brief Default constructor
         */
        DeferredShadingTechnique(void);

        /*!
         *  \brief Destructor
         */
        ~DeferredShadingTechnique(void);

        /*!
         *  \brief Initialize the rendering technique
         *  @param pNumber of point lights to render (this technique only takes into account 1 spot light for the shadow
         *  @param pWindowWidth is used for the width of the texture
         *  @param pWindowHeight is used for the height of the texture
         */
        void init(unsigned int pPointLightCount, unsigned int pSpotLightCount, const std::tuple<unsigned int, unsigned int> & pFramebufferDimensions);

        /*!
         *  \brief Render the meshes provided as parameter using this rendering technique
         *  @param pMeshes is a container with all the meshes
         *  @param pLights is a container with all the lights
         */
        void render(const std::map<std::string, MeshAndTransform> & pMeshes, const std::vector<std::shared_ptr<BaseLight>> & pLights);

    private:
        /*!
         *  \brief Helper method to fill the stencil buffer in deferred shading
         *  \param pLight MUST BE a pointer on a point light or a spot light
         */
        template <typename T>
        void _stencilPass(const std::shared_ptr<T> pLight);

        /*!
         *  \brief Helper method to compute the radius of the bounding sphere representing the volume of the scene illuminated by the light
         *  \param pLight MUST BE a pointer on a PointLight or a SpotLight
         */
        template <typename T>
        float _sphereRadius(const std::shared_ptr<T> pLight) const;

        /*!
         *  \brief Helper method to render the VS attributes to textures
         */
        void _geometryPass(std::vector<std::map<std::string, MeshAndTransform>::const_iterator> & pMeshIterators);

        /*!
         *  \brief Helper method to display the above mentioned textures
         */
        void _lightPass(void);

        /*!
         *  \brief Helper method to start the deferred shading process
         */
        void _finalPass(void);

        /*!
         *  \brief Helper method to render the directional light
         */
        void _directionalLightPass(const std::vector<std::shared_ptr<BaseLight>> & pLights);

        /*!
         *  \brief Helper method to render the point lights
         *  \param pLight is a pointer on a PointLight
         */
        void _pointLightPass(const std::shared_ptr<PointLight> pLight);

        /*!
         *  \brief Helper method to render the spot lights
         *  \param pLight is a pointer on a SpotLight
         */
        void _spotLightPass(const std::shared_ptr<SpotLight> pLight);

    private:
        std::unique_ptr<GBuffer> mGeometryBuffers;
        std::unique_ptr<DeferredShadingGeometryPass> mDSGeometryPass;
        std::unique_ptr<DeferredShadingDirectionalLightPass> mDSDirectionalLightPass;
        std::unique_ptr<DeferredShadingPointLightPass> mDSPointLightPass;
        std::unique_ptr<DeferredShadingSpotLightPass> mDSSpotLightPass;
        std::unique_ptr<DeferredShadingNullPass> mDSNullPass;

        MeshAndTransform mSphere;
        MeshAndTransform mQuad;

        unsigned int mFramebufferWidth = 0u;
        unsigned int mFramebufferHeight = 0u;

    }; // class DeferredShadingTechnique

    template <typename T>
    void DeferredShadingTechnique::_stencilPass(const std::shared_ptr<T> pLight)
    {
        mDSNullPass->use();

        // Disable color and depth write and enable stencil
        mGeometryBuffers->bindForStencilPass();

        glEnable(GL_DEPTH_TEST);

        glDisable(GL_CULL_FACE);

        glClear(GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        // We need the stencil test to be enabled but we want it
        // to succeed always. Only the depth test matters.
        glStencilFunc(GL_ALWAYS, 0, 0);
        glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
        glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);

        Transform lTransformation;

        const auto & lPosition = pLight->position();
        lTransformation.translation(lPosition.x(), lPosition.y(), -lPosition.z());

        float lScale = _sphereRadius(pLight);
        lTransformation.scaling(lScale, lScale, lScale);

        mat4f lWorld = lTransformation.final();
        mat4f lWVP = mCamera->projection() * mCamera->view() * lWorld;

        mDSNullPass->WVP(lWVP);
        mSphere.mesh->render();
    }

    template <typename T>
    float DeferredShadingTechnique::_sphereRadius(const std::shared_ptr<T> pLight) const
    {
        auto lMaxChannel = fmaxf(fmaxf(pLight->color().x(), pLight->color().y()), pLight->color().z());

        const auto lConstant = pLight->attenuation(SpotLight::ATTENUATION_TYPE::CONSTANT);
        const auto lLinear = pLight->attenuation(SpotLight::ATTENUATION_TYPE::LINEAR);
        const auto lExp = pLight->attenuation(SpotLight::ATTENUATION_TYPE::EXPONENTIAL);

        return (-lLinear + sqrtf(lLinear * lLinear - 4.0f * lExp * (lConstant - 256.0f * lMaxChannel * pLight->diffuseIntensity()))) / (2.0f * lExp);
    }

} // namespace miniGL
