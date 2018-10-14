//===============================================================================================//
/*!
 *  \file      CascadedShadowMapFBO.cpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#include "CascadedShadowMapFBO.hpp"

#include <string>
#include <cassert>

#include "Exceptions.hpp"
#include "GLUtils.hpp"
#include "EngineCommon.hpp"

using std::string;
using std::to_string;
using miniGL::CascadedShadowMapFBO;
using miniGL::Exceptions;

CascadedShadowMapFBO::CascadedShadowMapFBO(void)
:mFBO(0),
mShadowMap{{0, 0, 0}}
{

}

CascadedShadowMapFBO::~CascadedShadowMapFBO(void)
{
    if (mFBO != 0)
        glDeleteFramebuffers(1, & mFBO);

    if (mShadowMap[0] != 0)
        glDeleteTextures(mShadowMap.size(), mShadowMap.data());
}

void CascadedShadowMapFBO::init(unsigned int pWindowWidth, unsigned int pWindowHeight)
{
    // Create the FBO
    glGenFramebuffers(1, & mFBO); checkOpenGLState;

    // Create the depth buffer
    glGenTextures(mShadowMap.size(), mShadowMap.data()); checkOpenGLState;

    for (auto it : mShadowMap)
    {
        glBindTexture(GL_TEXTURE_2D, it); checkOpenGLState;
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, pWindowWidth, pWindowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr); checkOpenGLState;
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); checkOpenGLState;
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); checkOpenGLState;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE); checkOpenGLState;
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); checkOpenGLState;
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); checkOpenGLState;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, mFBO); checkOpenGLState;

    // Draw the result of the depth test into the texture associated to the frame buffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mShadowMap[0], 0); checkOpenGLState;
//    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mShadowMap[1], 0); checkOpenGLState;
//    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mShadowMap[2], 0); checkOpenGLState;

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

void CascadedShadowMapFBO::bindForWriting(unsigned int pCascadeIndex)
{
    assert(pCascadeIndex < mShadowMap.size() && "Wrong cascade index in shadow map");

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mShadowMap[pCascadeIndex], 0);
}

void CascadedShadowMapFBO::bindForReading(void)
{
    glActiveTexture(CASCADE_SHADOW_TEXTURE_UNIT0);
    glBindTexture(GL_TEXTURE_2D, mShadowMap[0]);

    glActiveTexture(CASCADE_SHADOW_TEXTURE_UNIT1);
    glBindTexture(GL_TEXTURE_2D, mShadowMap[1]);

    glActiveTexture(CASCADE_SHADOW_TEXTURE_UNIT2);
    glBindTexture(GL_TEXTURE_2D, mShadowMap[2]);
}

size_t CascadedShadowMapFBO::size(void) const noexcept
{
    return mShadowMap.size();
}
