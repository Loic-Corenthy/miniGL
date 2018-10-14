//===============================================================================================//
/*!
 *  \file      SkyBoxRender.cpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#include "SkyBoxRender.hpp"

#include "Constants.hpp"
#include "Exceptions.hpp"
#include "Shader.hpp"

using miniGL::SkyBoxRender;
using miniGL::Shader;
using miniGL::Constants;
using miniGL::Exceptions;

SkyBoxRender::SkyBoxRender(void)
{
    mWVPLocation = Constants::invalidUniformLocation<GLuint>();
    mTextureLocation = Constants::invalidUniformLocation<GLuint>();
}

SkyBoxRender::~SkyBoxRender(void)
{

}

void SkyBoxRender::init(const std::string & pPathVS, const std::string & pPathFS)
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

    mWVPLocation = Program::uniformLocation("uWVP");
    mTextureLocation = Program::uniformLocation("uCubemapTexture");

    if(!checkUniformLocations())
        throw Exceptions("Not all uniform locations were updated", __FILE__, __LINE__);
}

void SkyBoxRender::WVP(const mat4f & pWVP)
{
    glUniformMatrix4fv(mWVPLocation, 1, GL_TRUE, const_cast<mat4f &>(pWVP).data());
}

void SkyBoxRender::textureUnit(unsigned int pTexUnit)
{
    glUniform1i(mTextureLocation, pTexUnit);
}

bool SkyBoxRender::checkUniformLocations(void) const
{
    return (mWVPLocation != Constants::invalidUniformLocation<GLuint>() && mTextureLocation != Constants::invalidUniformLocation<GLuint>());
}
