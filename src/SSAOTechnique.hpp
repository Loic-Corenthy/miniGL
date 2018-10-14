//===============================================================================================//
/*!
 *  \file      SSAOTechnique.hpp
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
#include "Constants.hpp"
#include "IOBuffer.hpp"
#include "SSAOGeometryPass.hpp"
#include "SSAOBlur.hpp"
#include "SSAOLighting.hpp"
#include "SSAORender.hpp"
#include "MeshAOS.hpp"

namespace miniGL
{
    /*!
     *  \brief   SSAO with depth reconstruction.
     *  \details The main idea is to calculate the view space position directly from the depth buffer
     */
    class SSAOTechnique : public RenderingTechniqueBase
    {
    public:
        enum class EShaderType
        {
            SSAO    = 0,
            NO_SSAO = 1,
            ONLY_AO = 2
        };

    public:
        /*!
         *  \brief Default constructor
         */
        SSAOTechnique(void);

        /*!
         *  \brief Initialize the rendering technique
         */
        void init(unsigned int pPointLightCount, unsigned int pSpotLightCount, const std::tuple<unsigned int, unsigned int> & pFramebufferDimensions);

        /*!
         *  \brief Render the meshes provided as parameter using this rendering technique
         *  @param pMeshes is a container with all the meshes
         *  @param pLights is a container with all the lights
         */
        void render(const std::map<std::string, MeshAndTransform> & pMeshes, const std::vector<std::shared_ptr<BaseLight>> & pLights);

        /*!
         *  \brief Configure the shader type
         *  @param pType is SSAO to use the technique, NO_SSAO to see the difference without it, ONLY_AO to debug
         */
        void shaderType(EShaderType pType);

        /*!
         *  \brief Get the current shader type
         *  @return SSAO, NO_SSAO or ONLY_AO
         */
        EShaderType shaderType(void) const noexcept;

        /*!
         *  \brief Set the indices of the lights that will be used when rendering using a specific technique
         *  @param pFirstIndex is the index of the first light to be added to the rendering technique
         */
        template<typename T, typename... Args>
        void lightToUseDuringRender(T pFirstIndex, Args... otherIndices)
        {
            mSSAOLighting->lightToUseDuringRender(pFirstIndex);
            lightToUseDuringRender(otherIndices...);
        }

        /*!
         *  \brief Set the indices of the last (or unique) light that will be used when rendering using a specific technique
         *  @param pIndex is the index of the light to be added to the rendering technique
         */
        template<typename T>
        void lightToUseDuringRender(T pIndex)
        {
            mSSAOLighting->lightToUseDuringRender(pIndex);
        }

    private:
        /*!
         *  \brief Intermediate step for the Screen Space Ambient Occlusion render
         */
        void _geometryPass(std::vector<std::map<std::string, MeshAndTransform>::const_iterator> & pMeshIterators);

        /*!
         *  \brief Intermediate step for the Screen Space Ambient Occlusion render
         */
        void _SSAOPass(void);

        /*!
         *  \brief Intermediate step for the Screen Space Ambient Occlusion render
         */
        void _blurPass(void);

        /*!
         *  \brief Intermediate step for the Screen Space Ambient Occlusion render
         */
        void _lightingPass(std::vector<std::map<std::string, MeshAndTransform>::const_iterator> & pMeshIterators, const std::vector<std::shared_ptr<BaseLight>> & pLights);

    private:
        std::unique_ptr<SSAORender> mSSAORender;
        std::unique_ptr<SSAOGeometryPass> mSSAOGeometryPass;
        std::unique_ptr<SSAOLighting> mSSAOLighting;
        std::unique_ptr<SSAOBlur> mSSAOBlur;
        MeshAOS mQuad;
        IOBuffer mSSAODepthBuffer;
        IOBuffer mSSAOAOBuffer;
        IOBuffer mSSAOBlurBuffer;
        EShaderType mSSAOShaderType = EShaderType::SSAO;

    }; // class SSAOTechnique

} // namespace miniGL
