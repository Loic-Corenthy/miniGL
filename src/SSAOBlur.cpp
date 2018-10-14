//===============================================================================================//
/*!
 *  \file      SSAOBlur.cpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#include "SSAOBlur.hpp"

#include "Shader.hpp"
#include "Exceptions.hpp"
#include "EngineCommon.hpp"

using miniGL::Exceptions;
using miniGL::SSAOBlur;
using miniGL::IOBuffer;

SSAOBlur::~SSAOBlur(void)
{
}

void SSAOBlur::init(void)
{
    // Create, load and compile a vertex shader
    Shader lVS(Shader::ETYPE::VERTEX);

    lVS.loadText("./Shaders/SSAOBlur.vert");
    lVS.compile();

    // Create, load and compile a fragment shader
    Shader lFS(Shader::ETYPE::FRAGMENT);

    lFS.loadText("./Shaders/SSAOBlur.frag");
    lFS.compile();

    // Load the vertex and fragment shader into our program and link
    attachShader(lVS);
    attachShader(lFS);

    link();

    // Use our program
    use();

    detachAndDeleteShader(lVS);
    detachAndDeleteShader(lFS);

    mInputTextureUnitLocation = Program::uniformLocation("uColorMap");

    // Check if we correctly initialized the uniform variables
    if (!checkUniformLocations())
        throw Exceptions("Not all uniform locations were updated", __FILE__, __LINE__);

    glUniform1i(mInputTextureUnitLocation, COLOR_TEXTURE_UNIT_INDEX);
}

void SSAOBlur::bindInputBuffer(IOBuffer & pInputBuffer)
{
    pInputBuffer.bindForReading(COLOR_TEXTURE_UNIT);
}

bool SSAOBlur::checkUniformLocations(void) const
{
    return mInputTextureUnitLocation != Constants::invalidUniformLocation<GLuint>();
}
