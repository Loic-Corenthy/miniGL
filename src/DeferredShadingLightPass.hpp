//===============================================================================================//
/*!
 *  \file      DeferredShadingLightPass.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

#include <GL/glew.h>

#include "Program.hpp"
#include "Algebra.hpp"
#include "EnumClassCast.hpp"

namespace miniGL
{
    /*!
     *  \brief Handles shaders for the deferred shading technique
     *  \details
     */
    class DeferredShadingLightPass : public Program
    {
    public:
        /*!
         * \brief Destructor
         */
        virtual ~DeferredShadingLightPass(void);

        /*!
         * \brief Get the location for all the uniform variables used by the light pass
         */
        void init(void);

        /*!
         *  \brief Set the world view projection matrix
         *  @param pWVP is a 4x4 matrix
         */
        void WVP(const mat4f & pWVP);

        /*!
         *  \brief Set the index for the texture keeping the positions
         *  @param pTextureUnit is the corresponding index
         */
        void positionTextureUnit(unsigned int pTextureUnit);

        /*!
         *  \brief Set the index for the texture keeping the color (from a texture associated to the mesh)
         *  @param pTextureUnit is the corresponding index
         */
        void colorTextureUnit(unsigned int pTextureUnit);

        /*!
         *  \brief Set the index for the texture keeping the normals
         *  @param pTextureUnit is the corresponding index
         */
        void normalTextureUnit(unsigned int pTextureUnit);

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
         *  \brief Set the dimensions of the framebuffer used to render to screen (and not the dimensions of the window)
         *  @param pWidth is the width of the framebuffer
         *  @param pHeight is the height of the framebuffer
         */
        void screenSize(unsigned int pWidth, unsigned int pHeight);

    protected:
        /*!
         * \brief Default constuctor, protected to be used only by derived classes.
         */
        DeferredShadingLightPass(void);

        /*!
         *  \brief Implementation of a virtual method from Program
         */
        virtual bool checkUniformLocations(void) const override;

    public:
        GLuint mWVPLocation;
        GLuint mPositionTextureUnitLocation;
        GLuint mNormalTextureUnitLocation;
        GLuint mColorTextureUnitLocation;
        GLuint mEyeWorldPosLocation;
        GLuint mMaterialSpecularIntensityLocation;
        GLuint mMaterialSpecularPowerLocation;
        GLuint mScreenSizeLocation;

    }; // class DeferredShadingLightPass

    inline void DeferredShadingLightPass::WVP(const mat4f & pWVP)
    {
        glUniformMatrix4fv(mWVPLocation, 1, GL_TRUE, const_cast<mat4f&>(pWVP).data());
    }

    inline void DeferredShadingLightPass::positionTextureUnit(unsigned int pTextureUnit)
    {
        glUniform1i(mPositionTextureUnitLocation, pTextureUnit);
    }

    inline void DeferredShadingLightPass::colorTextureUnit(unsigned int pTextureUnit)
    {
        glUniform1i(mColorTextureUnitLocation, pTextureUnit);
    }

    inline void DeferredShadingLightPass::normalTextureUnit(unsigned int pTextureUnit)
    {
        glUniform1i(mNormalTextureUnitLocation, pTextureUnit);
    }

    inline void DeferredShadingLightPass::eyeWorldPosition(const vec3f & pPos)
    {
        glUniform3f(mEyeWorldPosLocation, pPos.x(), pPos.y(), pPos.z());
    }

    inline void DeferredShadingLightPass::materialSpecularIntensity(float pValue)
    {
        glUniform1f(mMaterialSpecularIntensityLocation, pValue);
    }

    inline void DeferredShadingLightPass::materialSpecularPower(float pValue)
    {
        glUniform1f(mMaterialSpecularPowerLocation, pValue);
    }

    inline void DeferredShadingLightPass::screenSize(unsigned int pWidth, unsigned int pHeight)
    {
        glUniform2f(mScreenSizeLocation, static_cast<float>(pWidth), static_cast<float>(pHeight));
    }

} // namespace miniGL
