//===============================================================================================//
/*!
 *  \file      MultipassShadowMapFBO.cpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#include "MultipassShadowMapFBO.hpp"

#include <string>
#include <algorithm>

#include "GLUtils.hpp"
#include "Exceptions.hpp"

using std::string;
using std::to_string;
using miniGL::MultipassShadowMapFBO;

MultipassShadowMapFBO::~MultipassShadowMapFBO(void)
{
    if (mFBO != 0)
        glDeleteFramebuffers(1, & mFBO);

    if (mShadowMap != 0)
        glDeleteTextures(1, & mShadowMap);

    if (mDepth != 0)
        glDeleteTextures(1, & mDepth);
}

void MultipassShadowMapFBO::init(unsigned int pWindowWidth, unsigned int pWindowHeight)
{
    // Create the FBO
    glGenFramebuffers(1, & mFBO); checkOpenGLState;
    glBindFramebuffer(GL_FRAMEBUFFER, mFBO); checkOpenGLState;

    auto lEdge = std::max(pWindowWidth, pWindowHeight);

    // Create the depth buffer
    glGenTextures(1, & mDepth); checkOpenGLState;
    glBindTexture(GL_TEXTURE_2D, mDepth); checkOpenGLState;
    // Create the depth texture with same width and height to match the cube map, see below
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, lEdge, lEdge, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL); checkOpenGLState;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); checkOpenGLState;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); checkOpenGLState;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); checkOpenGLState;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); checkOpenGLState;
    glBindTexture(GL_TEXTURE_2D, 0);

    // Create the cubemap
    glGenTextures(1, & mShadowMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, mShadowMap);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR); checkOpenGLState;
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR); checkOpenGLState;
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); checkOpenGLState;
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); checkOpenGLState;
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE); checkOpenGLState;

    for (unsigned int i = 0; i < 6; ++i)
    {
        // The texture set to create a cube map MUST BE A SQUARE TEXTURE: width MUST EQUAL height
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_R32F, lEdge, lEdge, 0, GL_RED, GL_FLOAT, NULL);
        checkOpenGLState;
    }

    // Draw the result of the depth test into the texture associated to the frame buffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mDepth, 0); checkOpenGLState;

    // Disable read and writes to the color buffer
    glDrawBuffer(GL_NONE); checkOpenGLState;
    glReadBuffer(GL_NONE); checkOpenGLState;

    GLenum lStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (lStatus != GL_FRAMEBUFFER_COMPLETE)
    {
        string lMessage("Shadow Map frame buffer status: ");
        lMessage.append(to_string(lStatus));
        throw Exceptions(lMessage, __FILE__, __LINE__);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0); checkOpenGLState;
}

void MultipassShadowMapFBO::bindForWriting(GLenum pCubeFace)
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, pCubeFace, mShadowMap, 0);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
}

void MultipassShadowMapFBO::bindForReading(GLenum pTextureUnit)
{
    glActiveTexture(pTextureUnit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, mShadowMap);
}
