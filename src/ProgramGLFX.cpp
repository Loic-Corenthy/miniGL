//===============================================================================================//
/*!
 *  \file      ProgramGLFX.cpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#include "ProgramGLFX.hpp"

#ifdef WIN32
	#include <GL/glfx.h>
#else
	#include <glfx.h>
#endif

#include "Exceptions.hpp"
#include "Constants.hpp"

using std::string;
using miniGL::ProgramGLFX;

ProgramGLFX::ProgramGLFX(void)
{
    mEffect = glfxGenEffect();
}

ProgramGLFX::~ProgramGLFX(void)
{
    if (mProgram != 0)
    {
        glDeleteProgram(mProgram);
        mProgram = 0;
    }

    glfxDeleteEffect(mEffect);
}

void ProgramGLFX::effectFile(const string & pFile)
{
    mEffectFile = pFile;
}

void ProgramGLFX::compileProgram(const string & pProgram)
{
    if (mEffectFile.empty())
    {
        string lMessage("Must set the effect file prior to compiling the program");
        throw Exceptions(lMessage, __FILE__, __LINE__);
    }

    if (!glfxParseEffectFromFile(mEffect, mEffectFile.c_str()))
    {
        const char* lLog = glfxGetEffectLog(mEffect);
        string lMessage("Could not create effect from file ");
        lMessage.append(mEffectFile);
        lMessage.append(" : ");
        lMessage.append(lLog);
        throw Exceptions(lMessage, __FILE__, __LINE__);
    }

    mProgram = glfxCompileProgram(mEffect, pProgram.c_str());

    if (mProgram < 0)
    {
        const char* lLog = glfxGetEffectLog(mEffect);
        string lMessage("Could not compile program ");
        lMessage.append(pProgram);
        lMessage.append(" in effect file ");
        lMessage.append(mEffectFile);
        lMessage.append(" : ");
        lMessage.append(lLog);
        throw Exceptions(lMessage, __FILE__, __LINE__);
    }

    mProgramIsValid = true;
}

void ProgramGLFX::use(void) const
{
    glUseProgram(mProgram);
}

GLuint ProgramGLFX::id(void) const
{
    return static_cast<GLuint>(mProgram);
}

GLint ProgramGLFX::uniformLocation(const char* pName)
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

bool ProgramGLFX::isValid(void) const
{
    return mProgramIsValid;
}

GLint ProgramGLFX::param(GLint pParam) const
{
    GLint lParam;
    glGetProgramiv(mProgram, pParam, &lParam);
    return lParam;
}
