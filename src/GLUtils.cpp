//===============================================================================================//
/*!
 *  \file      GLUtils.cpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#include "GLUtils.hpp"

#include <iostream>

#include "Log.hpp"

using std::cout;
using std::endl;
using std::string;
using std::to_string;
using miniGL::GLUtils;
using miniGL::Log;

GLboolean GLUtils::mDepthTestIsEnabled = false;
GLboolean GLUtils::mDepthTestWriteMaskIsEnabled = false;
GLboolean GLUtils::mStencilTestIsEnabled = false;
GLboolean GLUtils::mBlendIsEnabled = false;
GLboolean GLUtils::mCullFaceIsEnabled = false;
GLboolean GLUtils::mTextureCubeMapIsEnabled = false;
GLboolean GLUtils::mRasterizerDiscardIsEnabled = false;

void GLCheck(const string & pFile, int pLineNumber, bool pLog)
{
    GLenum lErrorFlag = glGetError();
    if (lErrorFlag != GL_NO_ERROR)
    {
        string lMessage("GL error\nFlag: ");
        lMessage.append(to_string(lErrorFlag));
        lMessage.append("\nMessage: ");

        switch (lErrorFlag)
        {
            case GL_INVALID_ENUM:
                lMessage.append("INVALID ENUM");
                break;

            case GL_INVALID_VALUE:
                lMessage.append("INVALID VALUE");
                break;

            case GL_INVALID_OPERATION:
                lMessage.append("INVALID OPERATION");
                break;

            case GL_INVALID_FRAMEBUFFER_OPERATION:
                lMessage.append("INVALID FRAMEBUFFER OPERATION: The framebuffer object is not complete.");
                break;

            case GL_OUT_OF_MEMORY:
                lMessage.append("OUT OF MEMORY - UNDEFINED STATE: There is not enough memory left to execute the command.");
                break;

            case GL_STACK_UNDERFLOW:
                lMessage.append("STACK UNDERFLOW.");
                break;

            case GL_STACK_OVERFLOW:
                lMessage.append("STACK OVERFLOW.");
                break;

            default:
                lMessage.append("UNKNOWN ERROR.");
                break;
        }

        lMessage.append("\nFile: ");
        lMessage.append(pFile);
        lMessage.append("\nLine: ");
        lMessage.append(to_string(pLineNumber));

        if (pLog)
            Log::write(Log::EType::ERROR, lMessage);
        else
            cout << lMessage << endl;
    }
}

void GLUtils::check(const std::string & pFile, int pLineNumber, bool pLog)
{
    GLenum lErrorFlag = glGetError();
    if (lErrorFlag != GL_NO_ERROR)
    {
        std::string lMessage("GL error\nFlag: ");
        lMessage.append(std::to_string(lErrorFlag));
        lMessage.append("\nMessage: ");

        switch (lErrorFlag)
        {
            case GL_INVALID_ENUM:
                lMessage.append("INVALID ENUM");
                break;

            case GL_INVALID_VALUE:
                lMessage.append("INVALID VALUE");
                break;

            case GL_INVALID_OPERATION:
                lMessage.append("INVALID OPERATION");
                break;

            case GL_INVALID_FRAMEBUFFER_OPERATION:
                lMessage.append("INVALID FRAMEBUFFER OPERATION: The framebuffer object is not complete.");
                break;

            case GL_OUT_OF_MEMORY:
                lMessage.append("OUT OF MEMORY - UNDEFINED STATE: There is not enough memory left to execute the command.");
                break;

            case GL_STACK_UNDERFLOW:
                lMessage.append("STACK UNDERFLOW.");
                break;

            case GL_STACK_OVERFLOW:
                lMessage.append("STACK OVERFLOW.");
                break;

            default:
                lMessage.append("UNKNOWN ERROR.");
                break;
        }

        lMessage.append("\nFile: ");
        lMessage.append(pFile);
        lMessage.append("\nLine: ");
        lMessage.append(to_string(pLineNumber));

        if (pLog)
            Log::write(miniGL::Log::EType::ERROR, lMessage);
        else
            cout << lMessage << endl;
    }
}

GLint GLUtils::currentReadFBO(void)
{
    GLint lReadFBOId = -1;
    glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &lReadFBOId);
    return lReadFBOId;
}

GLint GLUtils::currentWriteFBO(void)
{
    GLint lWriteFBOId = -1;
    glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &lWriteFBOId);
    return lWriteFBOId;
}

void GLUtils::currentState(void)
{
    glGetBooleanv(GL_DEPTH_TEST, & mDepthTestIsEnabled);
    glGetBooleanv(GL_DEPTH_WRITEMASK, & mDepthTestWriteMaskIsEnabled);
    glGetBooleanv(GL_STENCIL_TEST, & mStencilTestIsEnabled);
    glGetBooleanv(GL_BLEND, & mBlendIsEnabled);
    glGetBooleanv(GL_CULL_FACE, & mCullFaceIsEnabled);
    glGetBooleanv(GL_TEXTURE_CUBE_MAP, & mTextureCubeMapIsEnabled);
    glGetBooleanv(GL_RASTERIZER_DISCARD, & mRasterizerDiscardIsEnabled);

    cout << "====================" << endl;
    cout << "OpenGL current state" << endl;
    cout << "====================" << endl;

    string state(mDepthTestIsEnabled?"ON":"OFF");
    cout << "Depth test: " << state << endl;

    state.assign(mDepthTestWriteMaskIsEnabled?"ON":"OFF");
    cout << "Depth test write mask: " << state << endl;

    state.assign(mStencilTestIsEnabled?"ON":"OFF");
    cout << "Stencil test: " << state << endl;

    state.assign(mBlendIsEnabled?"ON":"OFF");
    cout << "Blend: " << state << endl;

    state.assign(mCullFaceIsEnabled?"ON":"OFF");
    cout << "Cull face: " << state << endl;

    state.assign(mTextureCubeMapIsEnabled?"ON":"OFF");
    cout << "Texture cube map: " << state << endl;

    state.assign(mRasterizerDiscardIsEnabled?"ON":"OFF");
    cout << "Rasterizer discard: " << state << endl;
}
