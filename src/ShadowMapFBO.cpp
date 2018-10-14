//===============================================================================================//
/*!
 *  \file      ShadowMapFBO.cpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#include "ShadowMapFBO.hpp"

#include <string>
#include <cassert>

#include "Exceptions.hpp"
#include "GLUtils.hpp"

using std::string;
using std::to_string;
using miniGL::ShadowMapFBO;
using miniGL::Exceptions;

ShadowMapFBO::ShadowMapFBO(void)
:mFBO(0),
 mShadowMap(0)
{

}

ShadowMapFBO::~ShadowMapFBO(void)
{
    if (mFBO != 0)
        glDeleteFramebuffers(1, & mFBO);

    if (mShadowMap != 0)
        glDeleteTextures(1, & mShadowMap);
}

void ShadowMapFBO::init(unsigned int pWindowWidth, unsigned int pWindowHeight)
{
    // Create the FBO
    glGenFramebuffers(1, & mFBO); checkOpenGLState;

    // Create the depth buffer
    glGenTextures(1, & mShadowMap); checkOpenGLState;
    glBindTexture(GL_TEXTURE_2D, mShadowMap); checkOpenGLState;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, pWindowWidth, pWindowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL); checkOpenGLState;
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); checkOpenGLState;
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); checkOpenGLState;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE); checkOpenGLState;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); checkOpenGLState;
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); checkOpenGLState;

    glBindFramebuffer(GL_FRAMEBUFFER, mFBO); checkOpenGLState;

    // Draw the result of the depth test into the texture associated to the frame buffer
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mShadowMap, 0); checkOpenGLState;

    // Disable writes to the color buffer
    glDrawBuffer(GL_NONE); checkOpenGLState;
    glReadBuffer(GL_NONE); checkOpenGLState;

    GLenum lStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (lStatus != GL_FRAMEBUFFER_COMPLETE)
    {
        string lMessage("Shadow Map frame buffer status: ");
        lMessage.append(to_string(lStatus));
        throw Exceptions(lMessage, __FILE__, __LINE__);
    }
}

void ShadowMapFBO::bindForWriting(void)
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFBO);
}

void ShadowMapFBO::bindForReading(GLenum pTextureUnit)
{
    glActiveTexture(pTextureUnit);
    glBindTexture(GL_TEXTURE_2D, mShadowMap);
}
