//===============================================================================================//
/*!
 *  \file      SimpleColorRender.cpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#include "SimpleColorRender.hpp"

#include "Shader.hpp"
#include "Exceptions.hpp"

using std::string;
using std::string;
using miniGL::SimpleColorRender;
using miniGL::Shader;
using miniGL::Exceptions;

void SimpleColorRender::init(const string & pPathVS, const string & pPathFS)
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
    mColorLocation = Program::uniformLocation("uColor");

    if(!checkUniformLocations())
        throw Exceptions("Not all uniform locations were updated", __FILE__, __LINE__);
}

void SimpleColorRender::WVP(const mat4f & pWVP)
{
    glUniformMatrix4fv(mWVPLocation, 1, GL_TRUE, const_cast<mat4f &>(pWVP).data());
}

void SimpleColorRender::color(float pRed, float pGreen, float pBlue, float pAlpha)
{
    glUniform4f(mColorLocation, pRed, pGreen, pBlue, pAlpha);
}

bool SimpleColorRender::checkUniformLocations(void) const
{
    return (mWVPLocation != Constants::invalidUniformLocation<GLuint>() && mColorLocation != Constants::invalidUniformLocation<GLuint>() );
}
