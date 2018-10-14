//===============================================================================================//
/*!
 *  \file      IOBuffer.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

#include <GL/glew.h>

namespace miniGL
{
    /*!
     *  \brief   This class handles input / output texture buffers
     *  \details It has a texture for depth information and a generic color texture
     */
    class IOBuffer
    {
    public:
        /*!
         *  \brief Destructor
         */
        ~IOBuffer(void);

        /*!
         *  \brief Initialize the buffer
         *  @param pFrameBufferWidth is the width of the framebuffer in pixels
         *  @param pFrameBufferHeight is the width of the framebuffer in pixels
         *  @param pDepthBuffer defines if a depth buffer is required
         *  @param pInternalType is the type of texture
         */
        void init(unsigned int pFrameBufferWidth, unsigned int pFrameBufferHeight, bool pDepthBuffer, GLenum pInternalType);

        /*!
         *  \brief Make the FBO active for writting with the shader
         */
        void bindForWritting(void);

        /*!
         *  \brief Make the texture linked to the FBO active for reading
         *  @param pTextureUnit is the ID of the texture to which the values of the FBO will be copied to
         */
        void bindForReading(GLenum pTextureUnit);

    private:
        GLuint mFBO = 0;
        GLuint mTexture = 0;
        GLuint mDepth = 0;
        GLenum mInternalType = GL_NONE;

    }; // class IOBuffer

} // namespace miniGL
