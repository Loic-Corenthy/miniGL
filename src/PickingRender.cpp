//===============================================================================================//
/*!
 *  \file      PickingRender.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#include "PickingRender.hpp"

#include "Shader.hpp"
#include "Exceptions.hpp"

using std::string;
using miniGL::PickingRender;
using miniGL::Shader;
using miniGL::Exceptions;

void PickingRender::init(const std::string & pPathVS, const std::string & pPathFS)
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

    mWVPLocation = Program::uniformLocation("uWVP");
    mDrawIndexLocation = Program::uniformLocation("uDrawIndex");
    mObjectIndexLocation = Program::uniformLocation("uObjectIndex");

    if(!checkUniformLocations())
        throw Exceptions("Not all uniform locations were updated", __FILE__, __LINE__);
}

void PickingRender::WVP(const mat4f & pWVP)
{
    glUniformMatrix4fv(mWVPLocation, 1, GL_TRUE, const_cast<mat4f &>(pWVP).data());
}

void PickingRender::objectIndex(unsigned int pObjectIndex)
{
    glUniform1ui(mObjectIndexLocation, pObjectIndex);
}

void PickingRender::drawStartCallback(unsigned int pDrawIndex)
{
    glUniform1ui(mDrawIndexLocation, pDrawIndex);
}

bool PickingRender::checkUniformLocations(void) const
{
    return (mWVPLocation != Constants::invalidUniformLocation<GLuint>()         &&
            mDrawIndexLocation != Constants::invalidUniformLocation<GLuint>()   &&
            mObjectIndexLocation != Constants::invalidUniformLocation<GLuint>());
}
