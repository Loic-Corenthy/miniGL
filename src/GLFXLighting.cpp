//===============================================================================================//
/*!
 *  \file      GLFXLighting.cpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#include "GLFXLighting.hpp"

#include <cstring>
#include <iostream>

#include "Shader.hpp"
#include "Constants.hpp"
#include "EnumClassCast.hpp"
#include "Exceptions.hpp"
#include "GLUtils.hpp"

using std::string;
using std::vector;
using std::shared_ptr;
using std::to_string;
using miniGL::LightingBase;
using miniGL::ProgramGLFX;
using miniGL::GLFXLighting;
using miniGL::Constants;
using miniGL::toUT;
using miniGL::Shader;
using miniGL::Exceptions;

GLFXLighting::GLFXLighting(void)
:LightingBase()
{
}

GLFXLighting::~GLFXLighting(void)
{
}

void GLFXLighting::init(unsigned int pPointLightCount, unsigned int pSpotLightCount, const string & pPathGLSL)
{
    mProgramName.assign("GLFXLighting");

    // Set the name of the effect file
    effectFile(pPathGLSL);

    // compile the shaders in the effect file add associate them to the program with the name mProgramName
    compileProgram(mProgramName);

    // Use our program
    use();

    LightingBase::pointLights(pPointLightCount);
    LightingBase::spotLights(pSpotLightCount);
    LightingBase::initLightParametersLocations();

    // General lighting parameters
    mWVPLocation = ProgramGLFX::uniformLocation("uWVP");
    mWorldLocation = ProgramGLFX::uniformLocation("uWorld");
    mSamplerLocation = ProgramGLFX::uniformLocation("uSampler");
    mUseSamplerLocation = ProgramGLFX::uniformLocation("uUseSampler");
    mUniformColorLocation = ProgramGLFX::uniformLocation("uColor");
    mEyeWorldPosLocation = ProgramGLFX::uniformLocation("uEyeWorldPos");
    mMaterialSpecularIntensityLocation = ProgramGLFX::uniformLocation("uMaterialSpecularIntensity");
    mMaterialSpecularPowerLocation = ProgramGLFX::uniformLocation("uMaterialSpecularPower");


    // Check if we correctly initialized the uniform variables
    if (!GLFXLighting::checkUniformLocations())
        throw Exceptions("Not all uniform locations were updated", __FILE__, __LINE__);

    // By default, use the color texture
    useSampler(true);

    // The uniform color is white by default
    uniformColor(1.0f, 1.0f, 1.0f);
}

bool GLFXLighting::checkUniformLocations(void) const
{
    bool lRes = true;

    lRes &= (mWVPLocation != Constants::invalidUniformLocation<GLuint>());
    lRes &= (mWorldLocation != Constants::invalidUniformLocation<GLuint>());
    lRes &= (mSamplerLocation != Constants::invalidUniformLocation<GLuint>());
    lRes &= (mUseSamplerLocation != Constants::invalidUniformLocation<GLuint>());
    lRes &= (mUniformColorLocation != Constants::invalidUniformLocation<GLuint>());
    lRes &= (mEyeWorldPosLocation != Constants::invalidUniformLocation<GLuint>());
    lRes &= (mMaterialSpecularIntensityLocation != Constants::invalidUniformLocation<GLuint>());
    lRes &= (mMaterialSpecularPowerLocation != Constants::invalidUniformLocation<GLuint>());

    return lRes;
}

void GLFXLighting::eyeWorldPosition(const vec3f & pEyeWorldPos)
{
    glUniform3f(mEyeWorldPosLocation, pEyeWorldPos.x(), pEyeWorldPos.y(), pEyeWorldPos.z());
}

void GLFXLighting::textureUnit(unsigned int pTexUnit)
{
    glUniform1i(mSamplerLocation, pTexUnit);
}

void GLFXLighting::WVP(const mat4f & pWVP)
{
    glUniformMatrix4fv(mWVPLocation, 1, GL_TRUE, const_cast<mat4f&>(pWVP).data());
}

void GLFXLighting::worldMatrix(const mat4f & pWorld)
{
    glUniformMatrix4fv(mWorldLocation, 1, GL_TRUE, const_cast<mat4f&>(pWorld).data());
}

void GLFXLighting::materialSpecularIntensity(float pValue)
{
    glUniform1f(mMaterialSpecularIntensityLocation, pValue);
}

void GLFXLighting::materialSpecularPower(float pValue)
{
    glUniform1f(mMaterialSpecularPowerLocation, pValue);
}

void GLFXLighting::useSampler(bool pActivate)
{
    glUniform1i(mUseSamplerLocation, pActivate?1:0);
}

void GLFXLighting::uniformColor(float pRed, float pGreen, float pBlue, float pAlpha)
{
    glUniform4f(mUniformColorLocation, pRed, pGreen, pBlue, pAlpha);
}

