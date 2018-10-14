//===============================================================================================//
/*!
 *  \file      GLFXLighting.hpp
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
#include "ProgramGLFX.hpp"
#include "LightingBase.hpp"

namespace miniGL
{
    /*!
     *  \brief   This class handles the lighting using the glfx library to create effects
     *  \details This library enables the creation of shaders in a unique file for all components (vertex shader, fragment shader, ...)
     */
    class GLFXLighting : public LightingBase<ProgramGLFX>
    {
    public:
        /*!
         *  \brief Default constructor
         */
        GLFXLighting(void);

        /*!
         *  \brief Destructor
         */
        virtual ~GLFXLighting(void);

        /*!
         *  \brief Init the lighting technique: compile the glsl program containing all the shaders in the same file
         *  @param pPointLightCount is the number of point lights
         *  @param pSpotLightCount is the number of spot lights
         *  @param pPathGLSL is the path of the glsl file containing all the shaders information
         */
        void init(unsigned int pPointLightCount, unsigned int pSpotLightCount, const std::string & pPathGLSL = std::string(R"(./Shaders/GLFXLighting.glsl)"));

        /*!
         *  \brief Set the world view projection matrix
         *  @param pWVP is a 4x4 matrix
         */
        void WVP(const mat4f & pWVP);

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
         *  @param if pActivate is true, the final color is the product of the texel color
         *         and the current value of the light. Otherwise it is just the light
         */
        void useSampler(bool pActivate);

        /*!
         *  \brief Set a uniform color on the mesh, without the need for a texture
         *  @param pRed is the red component in [0,1]
         *  @param pGreen is the green component in [0,1]
         *  @param pBlue is the blue component in [0,1]
         *  @param pAlpha is the transparency component in [0,1]
         */
        void uniformColor(float pRed, float pGreen, float pBlue, float pAlpha = 1.0f);

    private:
        /*!
         *  \brief Set the texture unit
         *  @param pTexUnit is the texture unit index
         */
        void textureUnit(unsigned int pTexUnit);

        /*!
         *  \brief Implementation of a virtual method from Program
         */
        virtual bool checkUniformLocations(void) const final;

    private:
        std::string mProgramName;
        GLuint mWVPLocation = Constants::invalidUniformLocation<GLuint>();
        GLuint mWorldLocation = Constants::invalidUniformLocation<GLuint>();
        GLuint mSamplerLocation = Constants::invalidUniformLocation<GLuint>();
        GLuint mUseSamplerLocation = Constants::invalidUniformLocation<GLuint>();
        GLuint mUniformColorLocation = Constants::invalidUniformLocation<GLuint>();
        GLuint mEyeWorldPosLocation = Constants::invalidUniformLocation<GLuint>();
        GLuint mMaterialSpecularIntensityLocation = Constants::invalidUniformLocation<GLuint>();
        GLuint mMaterialSpecularPowerLocation = Constants::invalidUniformLocation<GLuint>();

    }; // class GLFXLighting

} // namespace miniGL
