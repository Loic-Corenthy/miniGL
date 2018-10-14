//===============================================================================================//
/*!
 *  \file      ShadowMap.cpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#include "ShadowMap.hpp"

#include "Shader.hpp"
#include "Constants.hpp"
#include "Exceptions.hpp"
#include "GLUtils.hpp"

using std::string;
using miniGL::ShadowMap;
using miniGL::Shader;
using miniGL::Program;
using miniGL::Constants;
using miniGL::Exceptions;

ShadowMap::ShadowMap(void)
:mWVPLocation(Constants::invalidUniformLocation<GLuint>()),
 mTextureLocation(Constants::invalidUniformLocation<GLuint>())
{

}

ShadowMap::~ShadowMap(void)
{

}

void ShadowMap::init(const string & pPathVS, const string & pPathFS)
{
    // Create, load and compile a vertex shader
    Shader lVS(Shader::ETYPE::VERTEX);

    lVS.loadText(pPathVS.c_str());
    lVS.compile();

    // Create, load and compile a fragment shader
    Shader lFS(Shader::ETYPE::FRAGMENT);

    lFS.loadText(pPathFS.c_str());
    lFS.compile();

    // Load the vertex and fragment shader into our program, link and validate
    attachShader(lVS);
    attachShader(lFS);

    link();

    detachAndDeleteShader(lVS);
    detachAndDeleteShader(lFS);

    // Use our program
    use();

    initWVPUniformLocation("uWVP");
    initTextureUniformLocation("uShadowMap"); // This is actually not necessary because we're not using the fragment shader in the shadow map render (only the vertex shader)

    // Check if we correctly initialized the uniform variables
    if (!checkUniformLocations())
        throw Exceptions("Not all uniform locations were updated", __FILE__, __LINE__);
}

void ShadowMap::setWVP(const mat4f & pWVP)
{
    glUniformMatrix4fv(mWVPLocation, 1, GL_TRUE, const_cast<mat4f &>(pWVP).data());
}

void ShadowMap::setTextureUnit(unsigned int pTextureUnit)
{
    glUniform1i(mTextureLocation, pTextureUnit);
    checkOpenGLState;
}

bool ShadowMap::checkUniformLocations(void) const
{
    return (mWVPLocation != Constants::invalidUniformLocation<GLuint>() && mTextureLocation != Constants::invalidUniformLocation<GLuint>());
}

void ShadowMap::initWVPUniformLocation(const char* pName)
{
    mWVPLocation = Program::uniformLocation(pName);
}

void ShadowMap::initTextureUniformLocation(const char* pName)
{
    mTextureLocation = Program::uniformLocation(pName);
}
