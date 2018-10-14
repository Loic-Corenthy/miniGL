//===============================================================================================//
/*!
 *  \file      SSAOLighting.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

#include <GL/glew.h>

#include "Program.hpp"
#include "LightingBase.hpp"
#include "IOBuffer.hpp"
#include "Constants.hpp"

namespace miniGL
{
    /*!
     *  \brief   Screen Space Ambient Occlusion renderer
     *  \details Compute the light taking into account how flat/cornered objects are
     */
    class SSAOLighting : public LightingBase<Program>
    {
    public:
        /*!
         *  \brief Destructor
         */
        virtual ~SSAOLighting(void);

        /*!
         *  \brief Implementation of a virtual method from LightingBase
         */
        virtual void init(unsigned int pPointLightCount, unsigned int pSpotLightCount);

        /*!
         *  \brief Set the world view projection matrix
         *  @param pWVP is a 4x4 matrix
         */
        void WVP(const mat4f & pWVP);

        /*!
         *  \brief Set the world matrix
         *  @param pWorld is a 4x4 matrix
         */
        void world(const mat4f & pWorld);

        /*!
         *  \brief Set the type of shader
         *  @param pShaderType is 0 for SHADER_TYPE_SSAO, 1 for SHADER_TYPE_NO_SSAO, 2 for SHADER_TYPE_ONLY_AO
         */
        void shaderType(int pShaderType);

        /*!
         *  \brief Bind the input/output buffer for reading the AO map
         *  @param pBuffer is the buffer to setup for reading operation
         */
        void bindAOBuffer(IOBuffer & pBuffer);

        /*!
         *  \brief Set the dimensions of the framebuffer used to render to screen (and not the dimensions of the window)
         *  @param pWidth is the width of the framebuffer
         *  @param pHeight is the height of the framebuffer
         */
        void screenSize(unsigned int pWidth, unsigned int pHeight);

        /*!
         *  \brief Activate the use of a texture to sample the final color from
         *  @param if pActivate is true, the final color is the product of the texel color and the current
         *         value of the light. Otherwise it is just the light
         */
        void useSampler(bool pActivate);

    private:
        /*!
         *  \brief Implementation of a virtual method from LightingBase
         */
        virtual bool checkUniformLocations(void) const final;

    private:
        GLuint mSamplerLocation = Constants::invalidUniformLocation<GLuint>();
        GLuint mUseSamplerLocation = Constants::invalidUniformLocation<GLuint>();
        GLuint mEyeWorldPosLocation = Constants::invalidUniformLocation<GLuint>();
        GLuint mMaterialSpecularIntensityLocation = Constants::invalidUniformLocation<GLuint>();
        GLuint mMaterialSpecularPowerLocation = Constants::invalidUniformLocation<GLuint>();

        GLuint mWVPLocation = Constants::invalidUniformLocation<GLuint>();
        GLuint mWorldLocation = Constants::invalidUniformLocation<GLuint>();
        GLuint mShaderTypeLocation = Constants::invalidUniformLocation<GLuint>();
        GLuint mScreenSizeLocation = Constants::invalidUniformLocation<GLuint>();
        GLuint mAmbientOcculusionTextureLocation = Constants::invalidUniformLocation<GLuint>();

    }; // class SSAOLighting

} // namespace miniGL
