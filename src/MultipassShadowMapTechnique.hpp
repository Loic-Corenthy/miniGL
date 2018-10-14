//===============================================================================================//
/*!
 *  \file      MultipassShadowMapTechnique.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

#include "RenderingTechniqueBase.hpp"
#include "MeshAndTransform.hpp"
#include "BaseLight.hpp"
#include "Constants.hpp"
#include "MultipassShadowMap.hpp"
#include "MultipassShadowMapFBO.hpp"
#include "MultipassShadowMapLighting.hpp"

namespace miniGL
{
    /*!
     *  \brief   This class encapsulate the different renderers for the multipass shadow map technique
     *  \details The rendering is done in two passes: shadow pass and render pass
     */
    class MultipassShadowMapTechnique : public RenderingTechniqueBase
    {
    public:
        /*!
         *  \brief Default constructor
         */
        MultipassShadowMapTechnique(void);

        /*!
         *  \brief Initialize the rendering technique
         */
        void init(unsigned int pPointLightCount, const std::tuple<unsigned int, unsigned int> & pFramebufferDimensions);

        /*!
         *  \brief Render the meshes provided as parameter using this rendering technique
         *  @param pMeshes is a container with all the meshes
         *  @param pLights is a container with all the lights
         */
        void render(const std::map<std::string, MeshAndTransform> & pMeshes, const std::vector<std::shared_ptr<BaseLight>> & pLights);

        /*!
         *  \brief Set the name of the mesh representing the "floor"
         *  @param pName is the name of the mesh that will be looked for in the mesh container (pMeshes) when calling the render method to render the
         *         "floor". No shadow will be computed for this mesh
         */
        void floor(const std::string & pName);

        /*!
         *  \brief Index of the light which position will be used to compute the shadow from
         *  @param pIndex is the index of a point light in the lights container
         */
        void pointLightIndex(unsigned int pIndex);

    private:
        /*!
         *  \brief Helper method for the multipass shadow map render
         */
        void _shadowPass(std::vector<std::map<std::string, MeshAndTransform>::const_iterator> & pMeshIterators, const std::vector<std::shared_ptr<BaseLight>> & pLights);

        /*!
         *  \brief Helper method for the multipass shadow map render
         */
        void _renderPass(std::vector<std::map<std::string, MeshAndTransform>::const_iterator> & pMeshIterators, std::map<std::string, MeshAndTransform>::const_iterator & pFloorIterator, const std::vector<std::shared_ptr<BaseLight>> & pLights);

    private:
        std::unique_ptr<MultipassShadowMapFBO> mMultipassShadowMapFBO;
        std::unique_ptr<MultipassShadowMap> mMultipassShadowMap;
        std::unique_ptr<MultipassShadowMapLighting> mMultipassShadowMapLighting;
        std::string mFloorMeshName;
        unsigned int mPointLightIndex = Constants::invalidBufferIndex<unsigned int>();

    }; // class MultipassShadowMapTechnique

} // namespace miniGL
