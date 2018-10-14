//===============================================================================================//
/*!
 *  \file      Program.cpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#include "Program.hpp"

#include <cassert>
#include <string>
#include <iostream>

#include "Shader.hpp"
#include "Constants.hpp"
#include "Exceptions.hpp"
#include "GLUtils.hpp"

using std::string;
using std::to_string;
using miniGL::Program;
using miniGL::Shader;
using miniGL::Constants;

Program::Program(void)
:mProgram(0),
 mProgramIsValid(false)
{
    // Init the mInfoLog to empty space
    for (auto & obj : mInfoLog)
        obj = ' ';

    // Create the program and check for errors
    mProgram = glCreateProgram();

    if (mProgram == 0)
        throw Exceptions("Error creating shader program", __FILE__, __LINE__);
}

Program::~Program(void)
{
    if (mProgram != 0)
    {
        glDeleteProgram(mProgram);
        mProgram = 0;
        mProgramIsValid = false;
    }
}

void Program::attachShader(const Shader & pShader)
{
    if (!pShader.isCompiled())
        throw Exceptions("Added shader not compiled", __FILE__, __LINE__);

    glAttachShader(mProgram, pShader.objectID());
}

void Program::detachAndDeleteShader(const Shader & pShader)
{
    glDetachShader(mProgram, pShader.objectID());
    glDeleteShader(pShader.objectID());
}

void Program::link(void)
{
    glLinkProgram(mProgram);
    _checkErrors(GL_LINK_STATUS);
}

void Program::validate(void)
{
    glValidateProgram(mProgram);
    _checkErrors(GL_VALIDATE_STATUS);
    mProgramIsValid = true;
}

void Program::use(void) const
{
    glUseProgram(mProgram);
}

GLuint Program::id(void) const
{
    return mProgram;
}

GLint Program::uniformLocation(const char* pName)
{
    GLuint lLocation = glGetUniformLocation(mProgram, pName);

    if (lLocation == Constants::invalidUniformLocation<GLuint>())
    {
        string lErrorMessage("Warning! Unable to get the location of uniform ");
        lErrorMessage.append(pName);
        throw Exceptions(lErrorMessage, __FILE__, __LINE__);
    }

    return lLocation;
}

bool Program::isValid(void) const
{
    return mProgramIsValid;
}

GLint Program::param(GLint pParam) const
{
    GLint lParam;
    glGetProgramiv(mProgram, pParam, &lParam);
    return lParam;
}

void Program::_checkErrors(GLenum pStatus)
{
    GLint lOk = 0;

    glGetProgramiv(mProgram, pStatus, &lOk);

    if (lOk == 0)
    {
        glGetProgramInfoLog(mProgram, sizeof(mInfoLog), NULL, mInfoLog);

        string lErrorMessage(1024, ' ');
        for(size_t i = 0; i < 1024; i++)
            lErrorMessage[i] = mInfoLog[i];

        throw Exceptions(lErrorMessage, __FILE__, __LINE__);
    }

    checkOpenGLState;
}
