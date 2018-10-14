//===============================================================================================//
/*!
 *  \file      InstancedLighting.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

#include <GL/glew.h>
#include <map>
#include <vector>
#include <string>

#include "Program.hpp"
#include "LightingBase.hpp"
#include "Constants.hpp"

namespace miniGL
{
    /*!
     *  \brief   This class handles the lighting shaders for instanced rendering
     *  \details This lighting renderer differs from the normal one mainly in the fact that the WVP
     *           and world matrices are provided dynamically for instance rendering
     */
    class InstancedLighting : public LightingBase<Program>
    {
    public:
        /*!
         *  \brief Default constructor
         */
        InstancedLighting(void);

        /*!
         *  \brief Destructor
         */
        ~InstancedLighting(void);

        /*!
         *  \brief Init the instanced lighting technique: create, compile and link the shaders and initialize all the uniform parameters
         *  @param pPointLightCount is the number of point lights
         *  @param pSpotLightCount is the number of spot lights
         *  @param pPathVS is the path to the vertex shader
         *  @param pPathFS is the path to the fragment shader
         */
        void init(unsigned int pPointLightCount, unsigned int pSpotLightCount, const std::string & pPathVS = std::string(R"(./Shaders/InstancedLighting.vert)"), const std::string & pPathFS = std::string(R"(./Shaders/InstancedLighting.frag)"));

        /*!
         *  \brief Activate the use of a texture to sample the final color from
         *  @param if pActivate is true, the final color is the product of the texel color and the current value of the light. Otherwise
         *         it is just the light
         */
        void useSampler(bool pActivate);

        /*!
         *  \brief Activate the computation of a shadow component to the color using the shadow map technique
         *  @param if pActivate is true, shasow is computed using the shadow map texture.
         */
        void useShadowMap(bool pActivate);

        /*!
         *  \brief Set the size of the framebuffer for the shadow map
         *  @param pFramebufferWidth is the width of the framebuffer in pixel
         *  @param pFramebufferHeight is the height of the framebuffer in pixel
         */
        void shadowMapSize(unsigned int pFramebufferWidth, unsigned int pFramebufferHeight);

        /*!
         *  \brief Activate the interpolation of the local normal using the normal mapping method
         *  @param if pActivate is true, the normal is interpolated. Otherwise, simply use the normal provided in the mesh
         */
        void useNormalMap(bool pActivate);

        /*!
         *  \brief Set the position of the camera to calculate the specular component
         *  @param pEyeWorldPos is the position of the camera
         */
        void eyeWorldPosition(const vec3f & pEyeWorldPos);

    private:
        /*!
         *  \brief Implementation of a virtual method from Program
         */
        virtual bool checkUniformLocations(void) const final;

    private:
        GLuint mSamplerLocation = Constants::invalidUniformLocation<GLuint>();
        GLuint mUseSamplerLocation = Constants::invalidUniformLocation<GLuint>();
        GLuint mShadowMapLocation = Constants::invalidUniformLocation<GLuint>();
        GLuint mUseShadowMapLocation = Constants::invalidUniformLocation<GLuint>();
        GLuint mNormalMapLocation = Constants::invalidUniformLocation<GLuint>();
        GLuint mUseNormalMapLocation = Constants::invalidUniformLocation<GLuint>();
        GLuint mLightWVPLocation = Constants::invalidUniformLocation<GLuint>();
        GLuint mEyeWorldPosLocation = Constants::invalidUniformLocation<GLuint>();
        GLuint mMaterialSpecularIntensityLocation = Constants::invalidUniformLocation<GLuint>();
        GLuint mMaterialSpecularPowerLocation = Constants::invalidUniformLocation<GLuint>();

        GLuint mInstanceColor0Location = Constants::invalidUniformLocation<GLuint>();
        GLuint mInstanceColor1Location = Constants::invalidUniformLocation<GLuint>();
        GLuint mInstanceColor2Location = Constants::invalidUniformLocation<GLuint>();
        GLuint mInstanceColor3Location = Constants::invalidUniformLocation<GLuint>();
    }; // class InstancedLighting

  } // namespace miniGL
