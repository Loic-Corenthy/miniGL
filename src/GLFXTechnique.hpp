//===============================================================================================//
/*!
 *  \file      GLFXTechnique.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

#include <memory>
#include <string>
#include <vector>

#include "RenderingTechniqueBase.hpp"
#include "GLFXLighting.hpp"
#include "MeshAndTransform.hpp"
#include "BaseLight.hpp"

namespace miniGL
{
    /*!
     *  \brief   This class handles the GLFX lighting
     *  \details The GLFX lighting uses the GLFX library to handle shaders (e.g. write all shaders in one file, ...)
     */
    class GLFXTechnique : public RenderingTechniqueBase
    {
    public:
        /*!
         *  \brief Default constructor
         */
        GLFXTechnique(void);

        /*!
         *  \brief Initialize the rendering technique
         *  @param pPointLightCount is the number of point lights to render
         *  @param pSpotLightCount is the number of spot lights to render
         */
        void init(unsigned int pPointLightCount, unsigned int pSpotLightCount);

        /*!
         *  \brief Render the meshes provided as parameter using this rendering technique
         *  @param pMeshes is a container with all the meshes
         *  @param pLights is a container with all the lights
         */
        void render(const std::map<std::string, MeshAndTransform> & pMeshes, const std::vector<std::shared_ptr<BaseLight>> & pLights);

        /*!
         *  \brief Add a color to render the mesh associated with this tecnique. The order used to add the meshes will match the color order.
         *  @param pRed is in the range [0, 1]
         *  @param pGreen is in the range [0, 1]
         *  @param pBlue is in the range [0, 1]
         */
        void addUniformColor(float pRed, float pGreen, float pBlue);

        /*!
         *  \brief Set the indices of the lights that will be used when rendering using a specific technique
         *  @param pFirstIndex is the index of the first light to be added to the rendering technique
         */
        template<typename T, typename... Args>
        void lightToUseDuringRender(T pFirstIndex, Args... otherIndices)
        {
            mGLFXLighting->lightToUseDuringRender(pFirstIndex);
            lightToUseDuringRender(otherIndices...);
        }

        /*!
         *  \brief Set the indices of the last (or unique) light that will be used when rendering using a specific technique
         *  @param pIndex is the index of the light to be added to the rendering technique
         */
        template<typename T>
        void lightToUseDuringRender(T pIndex)
        {
            mGLFXLighting->lightToUseDuringRender(pIndex);
        }

    private:
        std::unique_ptr<GLFXLighting> mGLFXLighting;
        std::vector<vec3f> mUniformColors;

    }; // class GLFXTechnique

} // namespace miniGL
