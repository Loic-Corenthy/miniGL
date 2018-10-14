//===============================================================================================//
/*!
 *  \file      InstancedLighting.cpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#include "InstancedLighting.hpp"

#include <cstring>
#include <iostream>

#include "Shader.hpp"
#include "Constants.hpp"
#include "EnumClassCast.hpp"
#include "Exceptions.hpp"
#include "GLUtils.hpp"
#include "EngineCommon.hpp"

using std::string;
using std::vector;
using std::to_string;
using miniGL::LightingBase;
using miniGL::InstancedLighting;
using miniGL::Constants;
using miniGL::toUT;
using miniGL::Shader;
using miniGL::Exceptions;

InstancedLighting::InstancedLighting(void)
:LightingBase()
{

}

InstancedLighting::~InstancedLighting(void)
{
}

bool InstancedLighting::checkUniformLocations(void) const
{
    bool lRes = true;

    lRes &= (mSamplerLocation != Constants::invalidUniformLocation<GLuint>());
    lRes &= (mUseSamplerLocation != Constants::invalidUniformLocation<GLuint>());
    lRes &= (mShadowMapLocation != Constants::invalidUniformLocation<GLuint>());
    lRes &= (mUseShadowMapLocation != Constants::invalidUniformLocation<GLuint>());
    lRes &= (mNormalMapLocation != Constants::invalidUniformLocation<GLuint>());
    lRes &= (mUseNormalMapLocation != Constants::invalidUniformLocation<GLuint>());
    lRes &= (mLightWVPLocation != Constants::invalidUniformLocation<GLuint>());
//    lRes &= (mShadowMapSizeLocation != Constants::invalidUniformLocation<GLuint>());
    lRes &= (mEyeWorldPosLocation != Constants::invalidUniformLocation<GLuint>());
    lRes &= (mMaterialSpecularIntensityLocation != Constants::invalidUniformLocation<GLuint>());
    lRes &= (mMaterialSpecularPowerLocation != Constants::invalidUniformLocation<GLuint>());

    lRes &= (mInstanceColor0Location != Constants::invalidUniformLocation<GLuint>());
    lRes &= (mInstanceColor1Location != Constants::invalidUniformLocation<GLuint>());
    lRes &= (mInstanceColor2Location != Constants::invalidUniformLocation<GLuint>());
    lRes &= (mInstanceColor3Location != Constants::invalidUniformLocation<GLuint>());

    return lRes;
}

void InstancedLighting::init(unsigned int pPointLightCount, unsigned int pSpotLightCount, const std::string & pPathVS, const std::string & pPathFS)
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
    Program::attachShader(lVS);
    Program::attachShader(lFS);

    Program::link();

    detachAndDeleteShader(lVS);
    detachAndDeleteShader(lFS);

    // Use our program
    Program::use();

    LightingBase::pointLights(pPointLightCount);
    LightingBase::spotLights(pSpotLightCount);

    // Init the location of uniform variables
    LightingBase::initLightParametersLocations();

    // General parameters
    mLightWVPLocation = Program::uniformLocation("uLightWVP");
    mSamplerLocation = Program::uniformLocation("uSampler");
    mShadowMapLocation = Program::uniformLocation("uShadowMap");
//    mShadowMapSizeLocation = Program::uniformLocation("uShadowMapSize");
    mNormalMapLocation = Program::uniformLocation("uNormalMap");
    mEyeWorldPosLocation = Program::uniformLocation("uEyeWorldPos");
    mMaterialSpecularIntensityLocation = Program::uniformLocation("uMaterialSpecularIntensity");
    mMaterialSpecularPowerLocation = Program::uniformLocation("uMaterialSpecularPower");

    // Use of sampler (color texture)
    mUseSamplerLocation = Program::uniformLocation("uUseSampler");

    // Use of shadow map
    mUseShadowMapLocation = Program::uniformLocation("uUseShadowMap");

    // Use of normal map
    mUseNormalMapLocation = Program::uniformLocation("uUseNormalMap");

    // Init location handle for instance color parameter
    mInstanceColor0Location = Program::uniformLocation("uInstanceColor[0]");
    mInstanceColor1Location = Program::uniformLocation("uInstanceColor[1]");
    mInstanceColor2Location = Program::uniformLocation("uInstanceColor[2]");
    mInstanceColor3Location = Program::uniformLocation("uInstanceColor[3]");

    // Check if we correctly initialized the uniform variables
    if (!InstancedLighting::checkUniformLocations())
        throw Exceptions("Not all uniform locations were updated", __FILE__, __LINE__);

    // Set default instance colors
    glUniform4f(mInstanceColor0Location, 1.0f, 1.0f, 0.0f, 1.0f);
    glUniform4f(mInstanceColor1Location, 0.0f, 1.0f, 1.0f, 1.0f);
    glUniform4f(mInstanceColor2Location, 0.5f, 0.5f, 0.5f, 1.0f);
    glUniform4f(mInstanceColor3Location, 0.7f, 0.7f, 0.3f, 1.0f);

    glUniform1i(mSamplerLocation, COLOR_TEXTURE_UNIT_INDEX); checkOpenGLState;
    glUniform1i(mShadowMapLocation, SHADOW_TEXTURE_UNIT_INDEX); checkOpenGLState;
    glUniform1i(mNormalMapLocation, NORMAL_TEXTURE_UNIT_INDEX); checkOpenGLState;

    // By default, use the color texture but not the shadow map and the normal map
    useSampler(true);
    useShadowMap(false);
    useNormalMap(false);
}

void InstancedLighting::useSampler(bool pActivate)
{
    glUniform1i(mUseSamplerLocation, pActivate?1:0);
}

void InstancedLighting::useShadowMap(bool pActivate)
{
    glUniform1i(mUseShadowMapLocation, pActivate?1:0);
}

void InstancedLighting::shadowMapSize(unsigned int pFramebufferWidth, unsigned int pFramebufferHeight)
{
//    glUniform2f(mShadowMapSizeLocation, static_cast<float>(pFramebufferWidth), static_cast<float>(pFramebufferHeight));
}

void InstancedLighting::useNormalMap(bool pActivate)
{
    glUniform1i(mUseNormalMapLocation, pActivate?1:0);
}

void InstancedLighting::eyeWorldPosition(const vec3f & pEyeWorldPos)
{
    glUniform3f(mEyeWorldPosLocation, pEyeWorldPos.x(), pEyeWorldPos.y(), pEyeWorldPos.z());
}
