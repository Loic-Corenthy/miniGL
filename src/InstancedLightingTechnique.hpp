//===============================================================================================//
/*!
 *  \file      InstancedLightingTechnique.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

#include <map>
#include <vector>

#include "RenderingTechniqueBase.hpp"
#include "InstancedLighting.hpp"
#include "MeshAndTransform.hpp"
#include "BaseLight.hpp"

namespace miniGL
{
    /*!
     *  \brief  This class encapsulates all the details to handle 3D instanced rendering
     *  \details It takes care of the InstancedLighting renderer
     */
    class InstancedLightingTechnique : public RenderingTechniqueBase
    {
    public:
        /*!
         *  \brief Default constructor
         */
        InstancedLightingTechnique(void);

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
         *  \brief Set the positions where each instance of the meshes to be rendered will be placed
         *  @param pMeshes is a container with all the positions in world space
         */
        void instancePositions(const std::vector<vec3f> & pInstancePositions);

        /*!
         *  \brief Set the velocity at which each instance of the meshes to he rendered will move
         *  @param pMeshes is a container with all the velocities in arbitrary unit
         */
        void instanceVelocities(const std::vector<vec3f> & pInstanceVelocities);

        /*!
         *  \brief Set a scalar value that will multiply all the velocities
         *  @param pValue is a scalar.
         */
        void instanceVelocitiesMultiplier(float pValue);

        /*!
         *  \brief Set the indices of the lights that will be used when rendering using a specific technique
         *  @param pFirstIndex is the index of the first light to be added to the rendering technique
         */
        template<typename T, typename... Args>
        void lightToUseDuringRender(T pFirstIndex, Args... otherIndices)
        {
            mInstancedLighting->lightToUseDuringRender(pFirstIndex);
            lightToUseDuringRender(otherIndices...);
        }

        /*!
         *  \brief Set the indices of the last (or unique) light that will be used when rendering using a specific technique
         *  @param pIndex is the index of the light to be added to the rendering technique
         */
        template<typename T>
        void lightToUseDuringRender(T pIndex)
        {
            mInstancedLighting->lightToUseDuringRender(pIndex);
        }

    private:
        std::vector<vec3f> mInstancePositions;
        std::vector<vec3f> mInstanceVelocities;

        std::unique_ptr<InstancedLighting> mInstancedLighting;
        float mInstanceVelocitiesMultiplier = 1.0f;

    }; // class InstancedLightingTechnique

} // namespace miniGL

