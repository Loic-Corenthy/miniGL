//===============================================================================================//
/*!
 *  \file      IntermediateBuffer.cpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#include "IntermediateBuffer.hpp"

#include "Exceptions.hpp"

using miniGL::Exceptions;
using miniGL::IntermediateBuffer;

IntermediateBuffer::~IntermediateBuffer(void)
{
    if (mFBO != 0)
    {
        glDeleteFramebuffers(1, & mFBO);
        mFBO = 0;
    }

    if (mColorBuffer != 0)
    {
        glDeleteTextures(1, & mColorBuffer);
        mColorBuffer = 0;
    }

    if (mMotionBuffer != 0)
    {
        glDeleteTextures(1, & mMotionBuffer);
        mMotionBuffer = 0;
    }

    if (mDepthBuffer != 0)
    {
        glDeleteTextures(1, & mDepthBuffer);
        mDepthBuffer = 0;
    }
}

void IntermediateBuffer::init(unsigned int pFrameBufferWidth, unsigned int pFrameBufferHeight)
{
    // Create the FBO
    glGenFramebuffers(1, & mFBO);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFBO);

    // Create the different buffers associated to the FBO
    glGenTextures(1, & mColorBuffer);
    glGenTextures(1, & mMotionBuffer);
    glGenTextures(1, & mDepthBuffer);

    // Color buffer
    glBindTexture(GL_TEXTURE_2D, mColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, pFrameBufferWidth, pFrameBufferHeight, 0, GL_RGB, GL_FLOAT, nullptr);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mColorBuffer, 0);

    // Motion buffer
    glBindTexture(GL_TEXTURE_2D, mMotionBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, pFrameBufferWidth, pFrameBufferHeight, 0, GL_RG, GL_FLOAT, nullptr);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, mMotionBuffer, 0);

    // Color buffer
    glBindTexture(GL_TEXTURE_2D, mDepthBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, pFrameBufferWidth, pFrameBufferHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mDepthBuffer, 0);

    // Check the configuration
    GLenum lDrawBuffers[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};

    glDrawBuffers(2, lDrawBuffers);

    GLenum lStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (lStatus != GL_FRAMEBUFFER_COMPLETE)
        throw Exceptions("IntermediateBuffer not configured properly", __FILE__, __LINE__);

    // Restore the default FBO
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void IntermediateBuffer::bindForReading(void)
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mColorBuffer);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, mMotionBuffer);
}

void IntermediateBuffer::bindForWritting(void)
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFBO);
}
