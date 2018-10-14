//===============================================================================================//
/*!
 *  \file      SimpleLightingWithShadow.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

#include <map>
#include <vector>

#include "RenderingTechniqueBase.hpp"
#include "ShadowMap.hpp"
#include "ShadowMapFBO.hpp"
#include "MeshAndTransform.hpp"
#include "Lighting.hpp"
#include "BaseLight.hpp"
#include "Texture.hpp"

namespace miniGL
{
    /*!
     *  \brief  This class encapsulate all the classes used to provide a light renderer that can compute shadows from a single spot light.
     *  \details The rendering technique supports a directional light, up to 4 point lights and 1 spot light. The shadow is computed using a shadow map.
     *           The shadow map uses Percentage Closer Filtering.
     */
    class SimpleLightingWithShadow : public RenderingTechniqueBase
    {
    public:
        /*!
         *  \brief Default constructor
         */
        SimpleLightingWithShadow(void);

        /*!
         *  \brief Initialize the rendering technique
         *  @param pNumber of point lights to render (this technique only takes into account 1 spot light for the shadow
         *  @param pFramebufferDimensions contains the width (0) and the height (1) of the framebuffer. It is used to initialize
         *         internal textures
         */
        void init(unsigned int pPointLightCount, const std::tuple<unsigned int, unsigned int> & pFramebufferDimensions);

        /*!
         *  \brief Render the meshes provided as parameter using this rendering technique
         *  @param pMeshes is a container with all the meshes
         *  @param pLights is a container with all the lights
         */
        void render(const std::map<std::string, MeshAndTransform> & pMeshes, const std::vector<std::shared_ptr<BaseLight>> & pLights);

        /*!
         *  \brief Render with shadows from a spot light
         *  @param pActivate to true to compute a shadow map within the shader, false otherwise
         */
        void useShadowMap(bool pActivate) noexcept;

        /*!
         *  \brief Set the name of the mesh representing the "floor"
         *  @param pName is the name of the mesh that will be looked for in the mesh container (pMeshes) when calling the render method to render the
         *         "floor". No shadow will be computed for this mesh
         */
        void floor(const std::string & pName);

        /*!
         *  \brief Set the indices of the lights that will be used when rendering using a specific technique
         *  @param pFirstIndex is the index of the first light to be added to the rendering technique
         */
        template<typename T, typename... Args>
        void lightToUseDuringRender(T pFirstIndex, Args... otherIndices)
        {
            mLighting->lightToUseDuringRender(pFirstIndex);
            lightToUseDuringRender(otherIndices...);
        }

        /*!
         *  \brief Set the indices of the last (or unique) light that will be used when rendering using a specific technique
         *  @param pIndex is the index of the light to be added to the rendering technique
         */
        template<typename T>
        void lightToUseDuringRender(T pIndex)
        {
            mLighting->lightToUseDuringRender(pIndex);
        }

    private:
        /*!
         *  \brief Helper method to render the shadow in a frame buffer object
         */
        void _shadowMapPass(std::vector<std::map<std::string, MeshAndTransform>::const_iterator> & pMeshIterators, std::vector<std::shared_ptr<BaseLight>>::const_iterator pSpotLightIterator);

        /*!
         *  \brief Helper method to render the meshes using the shadow information
         */
        void _renderWithShadowAndBumpMapping(std::vector<std::map<std::string, MeshAndTransform>::const_iterator> & pMeshIterators, std::map<std::string, MeshAndTransform>::const_iterator & pFloorIterator, std::vector<std::shared_ptr<BaseLight>> pLights, std::vector<std::shared_ptr<BaseLight>>::const_iterator pSpotLightIterator);

    private:
        Texture mNormalMap;
        std::unique_ptr<ShadowMap> mShadowMap;
        std::unique_ptr<ShadowMapFBO> mShadowMapFBO;
        std::unique_ptr<Lighting> mLighting;
        std::string mFloorMeshName;
        bool mUseShadowMap = true;
    }; // class SimpleLightingWithShadow

} // namespace miniGL
