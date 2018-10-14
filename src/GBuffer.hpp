//===============================================================================================//
/*!
 *  \file      GBuffer.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 *  \date      26/12/16
 */
//===============================================================================================//

#pragma once

#include <array>

#include <GL/glew.h>

#include "EnumClassCast.hpp"
#include "Constants.hpp"

namespace miniGL
{
    /*!
     *  \brief This class handles a frame buffer object with several textures
     *  \details This class handles the following textures for Multiple Render Targets: position,
     *           diffuse color component, normal and texture coordinates
     */
    class GBuffer
    {
    public:
        enum class ETextureType : unsigned int
        {
            POSITION,
            DIFFUSE,
            NORMAL,
            TEXCOORD
        };

    public:
        /*!
         * \brief Destructor
         */
        ~GBuffer(void);

        /*!
         * \brief Initialize the geometry buffer: get handles for the fbo, the depth texture and the different textures for the VS parameters
         * \param pWindowWidth is the width of the output frame buffer
         * \param pWindowHeight is the height of the output frame buffer
         */
        void init(unsigned int pWindowWidth, unsigned int pWindowHeight);

        /*!
         * \brief Clear the color framebuffer before new rendering
         */
        void startFrame(void);

        /*!
         * \brief Bind the buffers that will be filled during the geometry pass (store position, color and normal
         *        parameters in the different textures)
         */
        void bindForGeometryPass(void);

        /*!
         * \brief Unbind the draw buffers prior to stencil pass
         */
        void bindForStencilPass(void);

        /*!
         * \brief Set color buffer 4 as draw buffer and bind the textures filled with the parameters obtained from the geometry pass
         */
        void bindForLightPass(void);

        /*!
         * \brief Bind the default framebuffer to render to screen. Bind FBO and color buffer 4 to get values from them.
         */
        void bindForFinalPass(void);

        /*!
         * \brief Binds the textures as a target during the geometry pass
         */
        [[deprecated("Method used in tutorial 36. Do not use anymore")]]
         void bindForWriting(void);

        /*!
         * \brief Binds the FBO as input so its contents can be dumped to the screen.
         */
        [[deprecated("Method used in tutorial 36. Do not use anymore")]]
        void bindForReading(void);

        /*!
         * \brief Read the buffer associated to a texture
         * \param pType is either POSITION, DIFFUSE, NORMAL or TEXCOORD
         */
        [[deprecated("Method used in tutorial 36. Do not use anymore")]]
        void readBuffer(ETextureType pType);

    private:
        std::array<GLuint, 3> mTextures = {{Constants::invalidBufferIndex<GLuint>(), Constants::invalidBufferIndex<GLuint>(), Constants::invalidBufferIndex<GLuint>()}};
        GLuint mFBO = Constants::invalidBufferIndex<GLuint>();
        GLuint mDepthTexture = Constants::invalidBufferIndex<GLuint>();
        GLuint mFinalTexture = Constants::invalidBufferIndex<GLuint>();

    }; // class GBuffer

    inline void GBuffer::bindForWriting(void)
    {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFBO);
    }

    inline void GBuffer::bindForReading(void)
    {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

        for (unsigned int i = 0 ; i < mTextures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, mTextures[i]);
        }
    }

    inline void GBuffer::readBuffer(ETextureType pTextureType)
    {
        glReadBuffer(GL_COLOR_ATTACHMENT0 + toUT(pTextureType));
    }

} // namespace miniGL
