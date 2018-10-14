//===============================================================================================//
/*!
 *  \file      Lighting.hpp
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
     *  \brief  This class manages the lights in the scene using an ambiant/diffuse/specular model
     *  \details This lighting method handles a directional light, up to 4 point lights and up
     *           to 4 spot lights.
     */
    class Lighting : public LightingBase<Program>
    {
    public:
        enum class GENERAL_LIGHTING_PARAM : size_t
        {
            LIGHT_WVP,
            SAMPLER,
            USE_SAMPLER,
            SHADOW_MAP,
            SHADOW_MAP_SIZE,
            USE_SHADOW_MAP,
            NORMAL_MAP,
            USE_NORMAL_MAP,
            EYE_WORLD_POS,
            MATERIAL_SPECULAR_INTENSITY,
            MATERIAL_SPECULAR_POWER
        };
        enum class NORMAL_LIGHTING_PARAM : size_t
        {
            WVP,
            WORLD_MATRIX,
        };

    public:
        /*!
         *  \brief Default constructor
         */
        Lighting(void);

        /*!
         *  \brief Destructor
         */
        ~Lighting(void);

        /*!
         *  \brief Init the lighting technique: create, compile and link the shaders and initialize all the uniform parameters
         *  @param pPointLightCount is the number of point lights
         *  @param pSpotLightCount is the number of spot lights
         *  @param pPathVS is the path to the vertex shader
         *  @param pPathFS is the path to the fragment shader
         */
        void init(unsigned int pPointLightCount, unsigned int pSpotLightCount, const std::string & pPathVS = std::string(R"(./Shaders/Lighting.vert)"), const std::string & pPathFS = std::string(R"(./Shaders/Lighting.frag)"));

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
         *  \brief Set the world view projection matrix from the light point of view
         *  @param pLightWVP is a 4x4 matrix
         */
        void lightWVP(const mat4f & pLightWVP);

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
        void initUniformLocation(GENERAL_LIGHTING_PARAM pParam, const char* pName);

        /*!
         *  \brief Initialize the location of a uniform variable for a normal lighting parameter
         *  @param pParameter in the index used to store the location of the corresponding uniform parameter in a container, use an enum value.
         *  @param pName is the name of the uniform parameter in the shader
         */
        void initUniformLocation(NORMAL_LIGHTING_PARAM pParameter, const char* pName);

    private:
        using generalParamLoc = std::array<GLuint, 11>; // container for the general light parameter locations

        generalParamLoc mGeneralParameterLocations;
        std::array<GLuint, 2> mNormalLightingParameterLocations;

    }; // class Lighting

} // namespace miniGL

