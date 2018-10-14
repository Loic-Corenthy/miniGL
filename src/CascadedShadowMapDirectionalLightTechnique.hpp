//===============================================================================================//
/*!
 *  \file      CascadedShadowMapDirectionalLightTechnique.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

#include <map>
#include <vector>

#include "CascadedShadowMapFBO.hpp"
#include "CascadedShadowMapDirectionalLight.hpp"
#include "CascadedShadowMapDirectionalLightLighting.hpp"
#include "RenderingTechniqueBase.hpp"
#include "MeshAndTransform.hpp"
#include "BaseLight.hpp"
#include "Constants.hpp"

namespace miniGL
{
    /*!
     *  \brief   This technique enable the computation of shadows for a directional light. The closer the object to the camera, the higher quality the shadow.
     *  \details The frustum is divided into regions (cascades). A bounding box which is oriented along the light direction vector is created for each cascade.
     *           Smaller bounding box (for regions closer to the camera) will have a higher resolution for the shadow texture hence a nicer shadow
     */
    class CascadedShadowMapDirectionalLightTechnique : public RenderingTechniqueBase
    {
    public:
        /*!
         *  \brief Default constructor
         */
        CascadedShadowMapDirectionalLightTechnique(void);

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
         *  \brief Helper function
         */
        void _computeOrthogonalProjection(std::shared_ptr<DirectionalLight> pLight);

        /*!
         *  \brief Helper function
         */
        void _shadowPass(std::vector<std::map<std::string, MeshAndTransform>::const_iterator> & pMeshIterators, std::shared_ptr<DirectionalLight> pLight);

        /*!
         *  \brief Helper function
         */
        void _renderPass(std::vector<std::map<std::string, MeshAndTransform>::const_iterator> & pMeshIterators, std::map<std::string, MeshAndTransform>::const_iterator & pFloorIterator, std::shared_ptr<DirectionalLight> pLight);

    private:
        std::unique_ptr<CascadedShadowMapFBO> mCascadedShadowMapFBO;
        std::unique_ptr<CascadedShadowMapDirectionalLight> mCascadedShadowMapDirectionalLight;
        std::unique_ptr<CascadedShadowMapDirectionalLightLighting> mCascadedShadowMapDirectionalLightLighting;
        std::array<float, 4> mCascadeEnds;
        std::string mFloorMeshName;
    }; // class CascadedShadowMapDirectionalLightTechnique

} // namespace miniGL
