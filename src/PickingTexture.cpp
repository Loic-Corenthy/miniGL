//===============================================================================================//
/*!
 *  \file      PickingTexture.cpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#include "PickingTexture.hpp"

#include <string>

#include "GLUtils.hpp"
#include "Exceptions.hpp"

using std::string;
using std::to_string;
using miniGL::PickingTexture;

PickingTexture::~PickingTexture(void)
{
    if (mFBO != 0)
        glDeleteFramebuffers(1, & mFBO);

    if (mPickingTexture != 0)
        glDeleteTextures(1, & mPickingTexture);

    if (mDepthTexture != 0)
        glDeleteTextures(1, & mDepthTexture);
}

void PickingTexture::init(unsigned int pFrameBufferWidth, unsigned int pFrameBufferHeight)
{
    // Create the frame buffer object
    glGenFramebuffers(1, & mFBO); checkOpenGLState;
    glBindFramebuffer(GL_FRAMEBUFFER, mFBO); checkOpenGLState;

    // Create a texture object to store the primitive information
    glGenTextures(1, & mPickingTexture); checkOpenGLState;
    glBindTexture(GL_TEXTURE_2D, mPickingTexture); checkOpenGLState;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, pFrameBufferWidth, pFrameBufferHeight, 0, GL_RGB, GL_FLOAT, NULL); checkOpenGLState;
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mPickingTexture, 0); checkOpenGLState;

    // Create a texture for the depth buffer
    glGenTextures(1, & mDepthTexture); checkOpenGLState;
    glBindTexture(GL_TEXTURE_2D, mDepthTexture); checkOpenGLState;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, pFrameBufferWidth, pFrameBufferHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL); checkOpenGLState;
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_COMPONENT, GL_TEXTURE_2D, mDepthTexture, 0); checkOpenGLState;

    // Disable the reading to avoid problems that could occur with older graphic cards
    glReadBuffer(GL_NONE); checkOpenGLState;

    glDrawBuffer(GL_COLOR_ATTACHMENT0); checkOpenGLState;

    // Check the FBO
    GLenum lStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (lStatus != GL_FRAMEBUFFER_COMPLETE)
    {
        string lMessage("Error in creating FBO: ");
        lMessage.append(std::to_string(lStatus));
        throw Exceptions(lMessage, __FILE__, __LINE__);
    }

    // Restore default frame buffer
    glBindTexture(GL_TEXTURE_2D, 0); checkOpenGLState;
    glBindFramebuffer(GL_FRAMEBUFFER, 0); checkOpenGLState;
}

void PickingTexture::enableWritting(void)
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFBO);
}

void PickingTexture::disableWritting(void)
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

PickingTexture::PixelInfo PickingTexture::readPixel(unsigned int pX, unsigned int pY)
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, mFBO);
    glReadBuffer(GL_COLOR_ATTACHMENT0);

    PixelInfo lPixelInfo;
    glReadPixels(pX, pY, 1, 1, GL_RGB, GL_FLOAT, & lPixelInfo);

    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

    return lPixelInfo;
}
