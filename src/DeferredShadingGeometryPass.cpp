//===============================================================================================//
/*!
 *  \file      DeferredShadingGeometryPass.cpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 *  \date      28/12/16
 */
//===============================================================================================//

#include "DeferredShadingGeometryPass.hpp"

#include "Exceptions.hpp"
#include "Shader.hpp"

using miniGL::DeferredShadingGeometryPass;
using miniGL::Exceptions;

void DeferredShadingGeometryPass::init(void)
{
    // Create, load and compile a vertex shader
    Shader lVS(Shader::ETYPE::VERTEX);

    lVS.loadText("./Shaders/GeometryPass.vert");
    lVS.compile();

    // Create, load and compile a fragment shader
    Shader lFS(Shader::ETYPE::FRAGMENT);

    lFS.loadText("./Shaders/GeometryPass.frag");
    lFS.compile();

    // Load the vertex and fragment shader into our program and link
    attachShader(lVS);
    attachShader(lFS);

    link();

    detachAndDeleteShader(lVS);
    detachAndDeleteShader(lFS);

    // Use our program
    use();

    mWVPLocation = Program::uniformLocation("uWVP");
    mWorldLocation = Program::uniformLocation("uWorld");
    mColorTextureLocation = Program::uniformLocation("uColorMap");

    // Check if we correctly initialized the uniform variables
    if (!checkUniformLocations())
        throw Exceptions("Not all uniform locations were updated", __FILE__, __LINE__);
}

void DeferredShadingGeometryPass::WVP(const mat4f & pWVP)
{
    glUniformMatrix4fv(mWVPLocation, 1, GL_TRUE, const_cast<mat4f&>(pWVP).data());
}

void DeferredShadingGeometryPass::worldMatrix(const mat4f & pWorld)
{
    glUniformMatrix4fv(mWorldLocation, 1, GL_TRUE, const_cast<mat4f&>(pWorld).data());
}

void DeferredShadingGeometryPass::colorTextureUnit(unsigned int pTextureUnit)
{
    glUniform1i(mColorTextureLocation, pTextureUnit);
}

bool DeferredShadingGeometryPass::checkUniformLocations(void) const
{
    return (mWVPLocation != Constants::invalidUniformLocation<GLuint>() &&
            mWorldLocation != Constants::invalidUniformLocation<GLuint>() &&
            mColorTextureLocation != Constants::invalidUniformLocation<GLuint>());
}
