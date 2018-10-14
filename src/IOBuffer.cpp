//===============================================================================================//
/*!
 *  \file      IOBuffer.cpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#include "IOBuffer.hpp"

#include <string>

#include "GLUtils.hpp"
#include "Exceptions.hpp"

using std::string;
using std::to_string;
using miniGL::IOBuffer;

IOBuffer::~IOBuffer(void)
{
    if (mFBO != 0)
        glDeleteFramebuffers(1, & mFBO);

    if (mTexture != 0)
        glDeleteTextures(1, & mTexture);

    if (mDepth != 0)
        glDeleteTextures(1, & mDepth);
}

void IOBuffer::init(unsigned int pFrameBufferWidth, unsigned int pFrameBufferHeight, bool pDepthBuffer, GLenum pInternalType)
{
    mInternalType = pInternalType;

    GLenum lFormat = GL_NONE, lType = GL_NONE;

    switch (mInternalType)
    {
        case GL_RGB32F:
            lFormat = GL_RGB;
            lType = GL_FLOAT;
            break;

        case GL_R32F:
            lFormat = GL_RED;
            lType = GL_FLOAT;
            break;

        case GL_NONE:
            break;

        default:
            throw Exceptions("Undefined internal type when initializing IOBuffer", __FILE__, __LINE__);
            break;
    }

    // Create the FBO
    glGenFramebuffers(1, & mFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, mFBO);

    // Create the texture
    if (mInternalType != GL_NONE)
    {
        glGenTextures(1, & mTexture);
        glBindTexture(GL_TEXTURE_2D, mTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, mInternalType, pFrameBufferWidth, pFrameBufferHeight, 0, lFormat, lType, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTexture, 0);

        GLenum lDrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
        glDrawBuffers(1, lDrawBuffers);
    }

    if (pDepthBuffer)
    {
        glGenTextures(1, & mDepth);
        glBindTexture(GL_TEXTURE_2D, mDepth);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, pFrameBufferWidth, pFrameBufferHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mDepth, 0);
    }

    GLenum lStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (lStatus != GL_FRAMEBUFFER_COMPLETE)
    {
        string lMessage("Shadow Map frame buffer status: ");
        lMessage.append(to_string(lStatus));
        throw Exceptions(lMessage, __FILE__, __LINE__);
    }

    // Ogldev uses GL_DRAW_FRAMEBUFFER instead of GL_FRAMEBUFFER
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); checkOpenGLState;
}

void IOBuffer::bindForWritting(void)
{
    glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
}

void IOBuffer::bindForReading(GLenum pTextureUnit)
{
    glActiveTexture(pTextureUnit);

    if (mInternalType == GL_NONE)
        glBindTexture(GL_TEXTURE_2D, mDepth);
    else
        glBindTexture(GL_TEXTURE_2D, mTexture);
}
