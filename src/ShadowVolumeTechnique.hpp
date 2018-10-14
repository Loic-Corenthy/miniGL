//===============================================================================================//
/*!
 *  \file      ShadowVolumeTechnique.hpp
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
#include "ShadowVolumeRender.hpp"
#include "NullRender.hpp"
#include "Lighting.hpp"

namespace miniGL
{
    /*!
     *  \brief   Rendering technique to compute shadows for point lights
     *  \details This renderer only takes into account 1 point light
     */
    class ShadowVolumeTechnique : public RenderingTechniqueBase
    {
    public:
        /*!
         *  \brief Default constructor
         */
        ShadowVolumeTechnique(void);

        /*!
         *  \brief Initialize the rendering technique
         *  @param pPointLightCount is the number of point lights
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
         *  \brief Set the name of the mesh representing the "floor"
         *  @param pName is the name of the mesh that will be looked for in the mesh container (pMeshes) when calling the render method to render the
         *         "floor". No shadow will be computed for this mesh
         */
        void floor(const std::string & pName);

        /*!
         *  \brief Set the name of the mesh representing the "floor"
         *  @param pName is the name of a mesh corresponding to the mesh added with the addMeshToRender method but that was loaded with the Adjacencies option
         */
        void addMeshWithAdjacenciesToRender(const std::string & pName);

        /*!
         *  \brief Get the list of meshes with adjacencies that were added to this rendering technique
         *  @return a container with all the names
         */
        const std::vector<std::string> & meshWithAdjacenciesNames(void) const;

        /*!
         *  \brief Index of the light which position will be used to compute the silhouette from
         *  @param pIndex is the index of a point light in the lights container
         */
        void pointLightToUseDuringRender(unsigned int pIndex);

    private:
        /*!
         *  \brief Helper method for the Shadow Volume render
         */
        void _renderSceneIntoDepth(std::vector<std::map<std::string, MeshAndTransform>::const_iterator> & pMeshAdjacenciesIterators, std::map<std::string, MeshAndTransform>::const_iterator & pFloorIterator);

        /*!
         *  \brief
         */
        void _renderShadowVolumeIntoStencil(std::vector<std::map<std::string, MeshAndTransform>::const_iterator> & pMeshAdjacenciesIterators, std::vector<std::shared_ptr<BaseLight>> pLights);

        /*!
         *  \brief
         */
        void _renderShadowedScene(std::vector<std::map<std::string, MeshAndTransform>::const_iterator> & pMeshAdjacenciesIterators, std::map<std::string, MeshAndTransform>::const_iterator & pFloorIterator, std::vector<std::shared_ptr<BaseLight>> pLights);

        /*!
         *  \brief
         */
        void _renderAmbientLight(std::vector<std::map<std::string, MeshAndTransform>::const_iterator> & pMeshIterators, std::map<std::string, MeshAndTransform>::const_iterator & pFloorIterator);

    private:
        std::unique_ptr<Lighting> mLighting;
        std::unique_ptr<ShadowVolumeRender> mShadowVolume;
        std::unique_ptr<NullRender> mNullRender;
        std::vector<std::string> mMeshWithAdjacenciesNames;
        std::string mFloorMeshName;
        unsigned int mPointLightIndex = Constants::invalidBufferIndex<unsigned int>();

    }; // class ShadowVolumeTechnique

} // namespace miniGL
