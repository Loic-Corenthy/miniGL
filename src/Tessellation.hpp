//===============================================================================================//
/*!
 *  \file      Tessellation.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

#include <string>
#include <map>
#include <vector>

#include "BaseLight.hpp"
#include "Camera.hpp"
#include "MeshAndTransform.hpp"
#include "TessellationLighting.hpp"
#include "Texture.hpp"

namespace miniGL
{
    /*!
     *  \brief   This class encapsulate the default tessellation rendering technique
     *  \details It uses a TessellationLighting initialized in the default mode. It also contains a texture
     *           to store one distance map.
     */
    class Tessellation
    {
    public:
        /*!
         *  \brief Initialize the rendering technique
         *  @param pNumber of point lights to render
         *  @param pDisplacementMapFilename is the path to the texture that will be used as displacement map
         */
        void init(unsigned int pPointLightCount, const std::string & pDisplacementMapFilename);

        /*!
         *  \brief Render the meshes provided as parameter using this rendering technique
         *  @param pMeshes is a container with all the meshes
         *  @param pLights is a container with all the lights
         */
        void render(std::map<std::string, MeshAndTransform> & pMeshes, const std::vector<std::shared_ptr<BaseLight>> & pLights);

        /*!
         *  \brief Set a copy of the camera
         *  @param pCamera is a reference to the main camera in the scene
         */
        void camera(const std::shared_ptr<Camera> pCamera);

        /*!
         *  \brief Add the name of a mesh to be rendered by this technique
         *  @param pName is a name that will be matched with the mesh container (pMeshes) when calling the render method
         */
        void addMeshToRender(const std::string & pName);

        /*!
         *  \brief Set the displacement factor
         *  @param pValue is in arbitrary units
         */
        void displacementFactor(float pValue) noexcept;

        /*!
         *  \brief Get the displacement factor
         *  @return a value in arbitrary units
         */
        float displacementFactor(void) const noexcept;

        /*!
         *  \brief Set the indices of the lights that will be used when rendering using a specific technique
         *  @param pFirstIndex is the index of the first light to be added to the rendering technique
         */
        template<typename T, typename... Args>
        void lightToUseDuringRender(T pFirstIndex, Args... otherIndices)
        {
            mTessellationLighting->lightToUseDuringRender(pFirstIndex);
            lightToUseDuringRender(otherIndices...);
        }

        /*!
         *  \brief Set the indices of the last (or unique) light that will be used when rendering using a specific technique
         *  @param pIndex is the index of the light to be added to the rendering technique
         */
        template<typename T>
        void lightToUseDuringRender(T pIndex)
        {
            mTessellationLighting->lightToUseDuringRender(pIndex);
        }

    private:
        std::vector<std::string> mMeshToRenderNames;
        std::unique_ptr<TessellationLighting> mTessellationLighting;
        std::shared_ptr<Camera> mCamera;
        Texture mDisplacementMap;
        float mDisplacementFactor = 1.0f;

    }; // class Tessellation

} // namespace miniGL
