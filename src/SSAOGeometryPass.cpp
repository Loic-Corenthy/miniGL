//===============================================================================================//
/*!
 *  \file      SSAOGeometryPass.cpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#include "SSAOGeometryPass.hpp"

#include "Shader.hpp"
#include "Exceptions.hpp"

using miniGL::Exceptions;
using miniGL::SSAOGeometryPass;

SSAOGeometryPass::~SSAOGeometryPass(void)
{
}

void SSAOGeometryPass::init(void)
{
      // Create, load and compile a vertex shader
    Shader lVS(Shader::ETYPE::VERTEX);

    lVS.loadText("./Shaders/SSAOGeometryPass.vert");
    lVS.compile();

    // Create, load and compile a fragment shader
    Shader lFS(Shader::ETYPE::FRAGMENT);

    lFS.loadText("./Shaders/SSAOGeometryPass.frag");
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

    // Check if we correctly initialized the uniform variables
    if (!checkUniformLocations())
        throw Exceptions("Not all uniform locations were updated", __FILE__, __LINE__);
}

void SSAOGeometryPass::WVP(const mat4f & pWVP)
{
    glUniformMatrix4fv(mWVPLocation, 1, GL_TRUE, const_cast<mat4f &>(pWVP).data());
}

bool SSAOGeometryPass::checkUniformLocations(void) const
{
    return (mWVPLocation != Constants::invalidUniformLocation<GLuint>());
}
