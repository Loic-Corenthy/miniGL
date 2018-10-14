//===============================================================================================//
/*!
 *  \file      MultipassShadowMap.cpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#include "MultipassShadowMap.hpp"

#include "Shader.hpp"
#include "Exceptions.hpp"
#include "GLUtils.hpp"

using miniGL::MultipassShadowMap;
using miniGL::Shader;
using miniGL::Program;
using miniGL::Constants;
using miniGL::Exceptions;

void MultipassShadowMap::init(void)
{
    // Create, load and compile a vertex shader
    Shader lVS(Shader::ETYPE::VERTEX);
    lVS.loadText("./Shaders/MultipassShadowMap.vert");
    lVS.compile();

    // Create, load and compile a fragment shader
    Shader lFS(Shader::ETYPE::FRAGMENT);
    lFS.loadText("./Shaders/MultipassShadowMap.frag");
    lFS.compile();

    // Load the vertex and fragment shader into our program, link and validate
    attachShader(lVS);
    attachShader(lFS);

    link();

    detachAndDeleteShader(lVS);
    detachAndDeleteShader(lFS);

    // Use our program
    use();

    mWVPLocation = Program::uniformLocation("uWVP");
    mWorldLocation = Program::uniformLocation("uWorld");
    mLightWorldPositionLocation = Program::uniformLocation("uLightWorldPos");

    // Check if we correctly initialized the uniform variables
    if (!checkUniformLocations())
        throw Exceptions("Not all uniform locations were updated", __FILE__, __LINE__);
}

void MultipassShadowMap::WVP(const mat4f & pWVP)
{
    glUniformMatrix4fv(mWVPLocation, 1, GL_TRUE, const_cast<mat4f&>(pWVP).data());
}

void MultipassShadowMap::world(const mat4f & pWorld)
{
    glUniformMatrix4fv(mWorldLocation, 1, GL_TRUE, const_cast<mat4f&>(pWorld).data());
}

void MultipassShadowMap::lightPosition(const vec3f & pPos)
{
    glUniform3f(mLightWorldPositionLocation, pPos.x(), pPos.y(), pPos.z());
}

bool MultipassShadowMap::checkUniformLocations(void) const
{
    return (mWVPLocation != Constants::invalidUniformLocation<GLuint>()               &&
            mWorldLocation != Constants::invalidUniformLocation<GLuint>()             &&
            mLightWorldPositionLocation!= Constants::invalidUniformLocation<GLuint>() );
}

