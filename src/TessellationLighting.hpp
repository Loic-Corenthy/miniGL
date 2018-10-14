//===============================================================================================//
/*!
 *  \file      TessellationLighting.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

#include <GL/glew.h>
#include <map>
#include <vector>
#include <string>
#include <array>

#include "Algebra.hpp"
#include "Program.hpp"
#include "LightingBase.hpp"

namespace miniGL
{
    /*!
     *  \brief   This class implements lighting with tessellation
     *  \details Similar functionalities as in Lighting class but uses Control and Evaluation shaders
     *           for tessellation
     */
    class TessellationLighting : public LightingBase<Program>
    {
    public:
        enum class EMode
        {
            DEFAULT         = 0,
            POINT_NORMAL    = 1
        };

        enum class GENERAL_LIGHTING_PARAM : size_t
        {
            VP                                = 0,
            WORLD_MATRIX                    = 1,
            SAMPLER                            = 2,
            MAX_DISTANCE_TO_CAMERA_COEFF    = 3,
            DISPLACEMENT_MAP                = 4,
            DISPLACEMENT_FACTOR                = 5,
            TESSELLATION_LEVEL                = 6,
            USE_SAMPLER                        = 7,
            EYE_WORLD_POS                    = 8,
            MATERIAL_SPECULAR_INTENSITY        = 9,
            MATERIAL_SPECULAR_POWER            = 10
        };

    public:
        /*!
         *  \brief Default constructor
         */
        TessellationLighting(EMode pMode = EMode::DEFAULT);

        /*!
         *  \brief Destructor
         */
        ~TessellationLighting(void);

        /*!
         *  \brief Load and compile the shaders associated to this lighting technique.
         *         Initialize the id of the uniform variables used in the shaders.
         *  @param pPointLightCount is the number of point lights
         *  @param pSpotLightCount is the number of spot lights
         *  @param pPathVS is the path to the vertex shader
         *  @param pPathCS is the path to the tessellation control shader
         *  @param pPathES is the path to the tessellation evaluation shader
         *  @param pPathFS is the path to the fragment shader
         */
        void init(unsigned int pPointLightCount,
                  unsigned int pSpotLightCount,
                  const std::string & pPathVS = std::string(R"(./Shaders/TessellationLighting.vert)"),
                  const std::string & pPathCS = std::string(R"(./Shaders/TessellationLighting.ctrl)"),
                  const std::string & pPathES = std::string(R"(./Shaders/TessellationLighting.eval)"),
                  const std::string & pPathFS = std::string(R"(./Shaders/TessellationLighting.frag)"));

        /*!
         *  \brief Set the view projection matrix
         *  @param pVP is a 4x4 matrix
         */
        void VP(const mat4f & pVP);

        /*!
         *  \brief Set the world matrix
         *  @param pWorld is a 4x4 matrix
         */
        void worldMatrix(const mat4f & pWorld);

        /*!
         *  \brief Set the position of the camera to calculate the specular component
         *  @param pPos is the position of the camera
         */
        void eyeWorldPosition(const vec3f & pPos);

        /*!
         *  \brief Set the texture unit
         *  @param pParam is a general parameter corresponding to a texture
         *  @param pTexUnit is the texture unit index
         */
        void textureUnit(GENERAL_LIGHTING_PARAM pParam, unsigned int pTexUnit);

        /*!
         *  \brief Set material specular intensity
         *  @param pValue is a parameter in the range [0,1]
         */
        void materialSpecularIntensity(float pValue);

        /*!
         *  \brief Set material specular power
         *  @param pValue is a parameter in the range [1,32]
         */
        void materialSpecularPower(float pValue);

        /*!
         *  \brief Activate the use of a texture to sample the final color from
         *  @param if pActivate is true, the final color is the product of the texel color and the current value of the light. Otherwise
         *         it is just the light
         */
        void useSampler(bool pActivate);

        /*!
         *  \brief Set the displacement factor
         *  @param pValue is the factor by which the translation from the normal map is multiplied
         */
        void displacementFactor(float pValue);

        /*!
         *  \brief Set the maximum distance from the camera used to compute the tessellation level in the control shader
         *  @param pValue is distance factor
         */
        void maxDistanceToCameraCoeff(float pValue);

        /*!
         *  \brief Set the tessellation level in the control shader
         *  @param pValue is the tessellation level
         */
        void tessellationLevel(float pValue);

    private:
        /*!
         *  \brief Implementation of a virtual method from Program
         */
        virtual bool checkUniformLocations(void) const final;

        /*!
         *  \brief Initialize the location of a uniform variable for a general parameter
         *  @param pParam in the index used to store the location of the corresponding uniform parameter in a container, use an enum value.
         *  @param pName is the name of the uniform parameter in the shader
         */
        GLint initUniformLocation(GENERAL_LIGHTING_PARAM pParam, const char* pName);

    private:
        std::array<GLuint, 11> mGeneralParameterLocations;
        const EMode mMode;

    }; // class TessellationLighting

} // namespace miniGL
