//===============================================================================================//
/*!
 *  \file      ShadowMapDirectionalLightTechnique.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

#include <map>
#include <vector>

#include "RenderingTechniqueBase.hpp"
#include "MeshAndTransform.hpp"
#include "DirectionalLight.hpp"
#include "Constants.hpp"
#include "ShadowMapDirectionalLight.hpp"
#include "ShadowMapDirectionalLightLighting.hpp"
#include "ShadowMapFBO.hpp"

namespace miniGL
{
    /*!
     *  \brief   This technique handles shadows for a directional light
     *  \details It works only with directional lights
     */
    class ShadowMapDirectionalLightTechnique : public RenderingTechniqueBase
    {
    public:
        /*!
         *  \brief Default constructor
         */
        ShadowMapDirectionalLightTechnique(void);

        /*!
         *  \brief Initialize the rendering technique
         */
        void init(const std::tuple<unsigned int, unsigned int> & pFramebufferDimensions);

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

    private:
        /*!
         *  \brief Intermediate step to render a directional light with a shadow map
         */
        void _shadowPass(std::vector<std::map<std::string, MeshAndTransform>::const_iterator> & pMeshIterators, std::shared_ptr<DirectionalLight> pDirectionalLight);

        /*!
         *  \brief Intermediate step to render a directional light with a shadow map
         */
        void _renderPass(std::vector<std::map<std::string, MeshAndTransform>::const_iterator> & pMeshIterators, std::map<std::string, MeshAndTransform>::const_iterator & pFloorIterator, std::shared_ptr<DirectionalLight> pDirectionalLight);

    private:
        std::unique_ptr<ShadowMapDirectionalLight> mShadowMapDirectionalLight;
        std::unique_ptr<ShadowMapDirectionalLightLighting> mShadowMapDirectionalLightLighting;
        std::unique_ptr<ShadowMapFBO> mShadowMapFBO;
        std::string mFloorMeshName;
    }; // class ShadowMapDirectionalLightTechnique

} // namespace miniGL
