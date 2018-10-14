//===============================================================================================//
/*!
 *  \file      ShadowVolumeRender.cpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#include "ShadowVolumeRender.hpp"

#include "Shader.hpp"
#include "Exceptions.hpp"

using miniGL::ShadowVolumeRender;
using miniGL::Exceptions;

ShadowVolumeRender::~ShadowVolumeRender(void)
{
}

void ShadowVolumeRender::init(void)
{
      // Create, load and compile a vertex shader
    Shader lVS(Shader::ETYPE::VERTEX);

    lVS.loadText("./Shaders/ShadowVolume.vert");
    lVS.compile();

      // Create, load and compile a geometry shader
    Shader lGS(Shader::ETYPE::GEOMETRY);

    lGS.loadText("./Shaders/ShadowVolume.geom");
    lGS.compile();

    // Create, load and compile a fragment shader
    Shader lFS(Shader::ETYPE::FRAGMENT);

    lFS.loadText("./Shaders/ShadowVolume.frag");
    lFS.compile();

    // Load the vertex and fragment shader into our program and link
    attachShader(lVS);
    attachShader(lGS);
    attachShader(lFS);

    link();

    detachAndDeleteShader(lVS);
    detachAndDeleteShader(lGS);
    detachAndDeleteShader(lFS);

    // Use our program
    use();

    mWVPLocation = Program::uniformLocation("uWVP");
    mLightPosLocation = Program::uniformLocation("uLightPos");

    // Check if we correctly initialized the uniform variables
    if (!checkUniformLocations())
        throw Exceptions("Not all uniform locations were updated", __FILE__, __LINE__);
}

void ShadowVolumeRender::WVP(const mat4f & pWVP)
{
    glUniformMatrix4fv(mWVPLocation, 1, GL_TRUE, const_cast<mat4f &>(pWVP).data());
}

void ShadowVolumeRender::lightPosition(const vec3f & pPos)
{
    glUniform3f(mLightPosLocation, pPos.x(), pPos.y(), pPos.z());
}

bool ShadowVolumeRender::checkUniformLocations(void) const
{
    return (mWVPLocation != Constants::invalidUniformLocation<GLuint>() &&
            mLightPosLocation != Constants::invalidUniformLocation<GLuint>());
}
