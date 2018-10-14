//===============================================================================================//
/*!
 *  \file      CascadedShadowMapDirectionalLight.cpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#include "CascadedShadowMapDirectionalLight.hpp"

#include "Shader.hpp"
#include "Constants.hpp"
#include "Exceptions.hpp"
#include "GLUtils.hpp"

using miniGL::Shader;
using miniGL::Program;
using miniGL::Constants;
using miniGL::Exceptions;
using miniGL::CascadedShadowMapDirectionalLight;

CascadedShadowMapDirectionalLight::~CascadedShadowMapDirectionalLight(void)
{

}

void CascadedShadowMapDirectionalLight::init(const std::string & pPathVS, const std::string & pPathFS)
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

    // Check if we correctly initialized the uniform variables
    if (!checkUniformLocations())
        throw Exceptions("Not all uniform locations were updated", __FILE__, __LINE__);
}

void CascadedShadowMapDirectionalLight::WVP(const mat4f & pWVP)
{
    glUniformMatrix4fv(mWVPLocation, 1, GL_TRUE, const_cast<mat4f &>(pWVP).data());
}

bool CascadedShadowMapDirectionalLight::checkUniformLocations(void) const
{
    return mWVPLocation != Constants::invalidUniformLocation<GLuint>();
}
