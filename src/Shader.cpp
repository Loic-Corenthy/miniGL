//===============================================================================================//
/*!
 *  \file      Shader.cpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#include "Shader.hpp"

#include <sstream>
#include <fstream>
#include <iostream>
#include <cassert>

#include "Exceptions.hpp"
#include "GLUtils.hpp"
#include "Log.hpp"

using std::cout;
using std::endl;
using std::string;
using miniGL::Shader;
using miniGL::Log;

Shader::Shader()
:mObjID(0),
mType(ETYPE::UNDEFINED),
mTextLoaded(false),
mIsCompiled(false),
mIsAttachedToProgram(false)
{

}

Shader::Shader(ETYPE pType)
:mObjID(0),
mType(pType),
mTextLoaded(false),
mIsCompiled(false),
mIsAttachedToProgram(false)
{
    _createObject();
}

Shader::~Shader()
{
    string lMessage("Destructor ");
    lMessage.append(__FUNCTION__);
    Log::consoleMessage(lMessage);

    lMessage.assign("Deleting ");
    lMessage.append(mPath);
    Log::consoleMessage(lMessage);
}

void Shader::type(ETYPE pType)
{
    mType = pType;
    _createObject();
}

Shader::ETYPE Shader::type(void) const noexcept
{
    return mType;
}

GLuint Shader::objectID(void) const noexcept
{
    return mObjID;
}

bool Shader::isCompiled(void) const noexcept
{
    return mIsCompiled;
}

void Shader::loadText(const char* pPath)
{
    // Save the path of the file containing the code of the shader
    mPath.assign(pPath);

    if (mType == ETYPE::UNDEFINED)
    {
        string lErrorMessage("Must set type before loading shader text. Error while loading: ");
        lErrorMessage.append(pPath);
        throw Exceptions(lErrorMessage, __FILE__, __LINE__);
    }

    // Load the file and put all the text into a string
    std::ifstream lFile(pPath, std::ifstream::in);
    bool lFileOk = false;

    if (lFile)
    {
        string lLine;
        while (getline(lFile, lLine))
        {
            if (!lLine.empty())
                mText.append(lLine);
            else
                mText.append("\n");

            lLine.clear();
        }

        lFile.close();
        lFileOk = true;
    }

    // Load the string to the source shader.
    if (lFileOk)
    {
        const char* lText = mText.c_str();
        GLint lLength = static_cast<GLint>(mText.length());
        glShaderSource(mObjID, 1, &lText, &lLength);

        mTextLoaded = true;

        checkOpenGLState;
    }
    else
    {
        string lErrorMessage("Cannot read shader file: ");
        lErrorMessage.append(pPath);
        throw Exceptions(lErrorMessage, __FILE__, __LINE__);
    }
}


void Shader::compile(void)
{
    if(!mTextLoaded)
        throw Exceptions(string("Shader file not loaded before compiling: ") + mPath, __FILE__, __LINE__);

    GLint lSuccess = 0;
    glCompileShader(mObjID);
    glGetShaderiv(mObjID, GL_COMPILE_STATUS, &lSuccess);

    checkOpenGLState;

    if(!lSuccess)
    {
        GLchar lInfoLog[1024];

        for(auto & obj : lInfoLog)
            obj = ' ';

        glGetShaderInfoLog(mObjID, 1024, 0, lInfoLog);

        string lErrorMessage("Couldn't compile shader file: ");
        size_t i = 0;
        while (lInfoLog[i] != '\0' && i < 1024)
        {
            lErrorMessage.push_back(lInfoLog[i]);
            ++i;
        }

        lErrorMessage.append(": ");
        lErrorMessage.append(mPath);
        throw Exceptions(lErrorMessage, __FILE__, __LINE__);
    }

    mIsCompiled = true;
}

void Shader::_createObject(void)
{
    switch(mType)
    {
        case Shader::ETYPE::UNDEFINED:
            assert(false && "Should not create a Shader of type UNDEFINED");
            break;

        case Shader::ETYPE::VERTEX:
            mObjID = glCreateShader(GL_VERTEX_SHADER);
            checkOpenGLState;
            break;

        case Shader::ETYPE::TESSELLATION_CONTROL:
            mObjID = glCreateShader(GL_TESS_CONTROL_SHADER);
            checkOpenGLState;
            break;

        case Shader::ETYPE::TESSELLATION_EVALUATION:
            mObjID = glCreateShader(GL_TESS_EVALUATION_SHADER);
            checkOpenGLState;
            break;

        case Shader::ETYPE::GEOMETRY:
            mObjID = glCreateShader(GL_GEOMETRY_SHADER);
            checkOpenGLState;
            break;

        case Shader::ETYPE::FRAGMENT:
            mObjID = glCreateShader(GL_FRAGMENT_SHADER);
            checkOpenGLState;
            break;

        default:
            assert(false && "Wrong Shader type :(");
            break;
    }

    if(mObjID == 0)
        throw Exceptions(string("Wrong type when creating shader: ") + mPath, __FILE__, __LINE__);
}
