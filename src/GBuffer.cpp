//===============================================================================================//
/*!
 *  \file      GBuffer.cpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 *  \date      26/12/16
 */
//===============================================================================================//

#include "GBuffer.hpp"

#include <iostream>

#include "Exceptions.hpp"
#include "Log.hpp"

using std::string;
using std::to_string;
using std::cout;
using std::endl;
using miniGL::GBuffer;
using miniGL::Log;

GBuffer::~GBuffer(void)
{
    string lMessage("Destructor ");
    lMessage.append(__FUNCTION__);
    Log::consoleMessage(lMessage);

    glDisable(GL_TEXTURE_2D);

    if (mTextures[0] != Constants::invalidBufferIndex<GLuint>())
    {
        string lMessage("Deleting ");
        lMessage.append(to_string(mTextures.size()));
        lMessage.append(" textures");
        Log::consoleMessage(lMessage);
        glDeleteTextures(mTextures.size(), mTextures.data());
    }

    if (mDepthTexture != Constants::invalidBufferIndex<GLuint>())
    {
        Log::consoleMessage("Delete depth texture");

        glDeleteTextures(1, & mDepthTexture);
    }

    if (mFinalTexture != Constants::invalidBufferIndex<GLuint>())
    {
        Log::consoleMessage("Delete final texture");
        glDeleteTextures(1, & mFinalTexture);
    }

    if (mFBO != Constants::invalidBufferIndex<GLuint>())
        glDeleteFramebuffers(1, & mFBO);
}

void GBuffer::init(unsigned int pWindowWidth, unsigned int pWindowHeight)
{
    // Create frame buffer object and bind it for following operations
    glGenFramebuffers(1, & mFBO);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFBO);

    // Create textures for the vertex attributes
    glGenTextures(static_cast<GLint>(mTextures.size()), mTextures.data());

    // Create the depth texture
    glGenTextures(1, & mDepthTexture);

    // Create the final texture
    glGenTextures(1, & mFinalTexture);

    // Define textures for the vertex attributes
    for (unsigned int i = 0; i < mTextures.size(); ++i)
    {
        glBindTexture(GL_TEXTURE_2D, mTextures[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, pWindowWidth, pWindowHeight, 0, GL_RGB, GL_FLOAT, nullptr);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, mTextures[i], 0);
    }

    // Define a texture for the depth buffer (depth + stencil)
    glBindTexture(GL_TEXTURE_2D, mDepthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH32F_STENCIL8, pWindowWidth, pWindowHeight, 0, GL_DEPTH_STENCIL, GL_FLOAT_32_UNSIGNED_INT_24_8_REV, nullptr);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, mDepthTexture, 0);

    // Define a texture for the final buffer
    glBindTexture(GL_TEXTURE_2D, mFinalTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pWindowWidth, pWindowHeight, 0, GL_RGB, GL_FLOAT, nullptr);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, mFinalTexture, 0);

    GLenum lStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (lStatus != GL_FRAMEBUFFER_COMPLETE)
    {
        string lMessage("Frame buffer error: ");
        lMessage.append(to_string(lStatus));
        throw Exceptions(lMessage, __FILE__, __LINE__);
    }

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void GBuffer::startFrame(void)
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFBO);
    glDrawBuffer(GL_COLOR_ATTACHMENT4);
    glClear(GL_COLOR_BUFFER_BIT);
}

void GBuffer::bindForGeometryPass(void)
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFBO);
    GLenum lDrawBuffers[3] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
    glDrawBuffers(3, lDrawBuffers);
}

void GBuffer::bindForStencilPass(void)
{
    // We must disable the draw buffers when rendering in the stencil buffer
    glDrawBuffer(GL_NONE);
}

void GBuffer::bindForLightPass(void)
{
    glDrawBuffer(GL_COLOR_ATTACHMENT4);

    for (unsigned int i = 0 ; i < mTextures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, mTextures[i]);
    }
}

void GBuffer::bindForFinalPass(void)
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, mFBO);
    glReadBuffer(GL_COLOR_ATTACHMENT4);
}
