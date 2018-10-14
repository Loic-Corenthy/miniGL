//===============================================================================================//
/*!
 *  \file      Skinning.cpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#include "Skinning.hpp"

#include <cstring>
#include <iostream>

#include "Shader.hpp"
#include "Constants.hpp"
#include "EnumClassCast.hpp"
#include "Exceptions.hpp"
#include "GLUtils.hpp"
#include "EngineCommon.hpp"
#include "DirectionalLight.hpp"
#include "PointLight.hpp"
#include "SpotLight.hpp"

using std::string;
using std::vector;
using std::to_string;
using miniGL::Skinning;
using miniGL::LightingBase;
using miniGL::DirectionalLight;
using miniGL::PointLight;
using miniGL::SpotLight;
using miniGL::Constants;
using miniGL::toUT;
using miniGL::Shader;
using miniGL::Exceptions;

Skinning::Skinning(void)
:LightingBase()
{
}

Skinning::~Skinning(void)
{
}

bool Skinning::checkUniformLocations(void) const
{
    bool lRes = true;

    lRes &= mWVPLocation != Constants::invalidUniformLocation<GLuint>();
    lRes &= mWorldLocation != Constants::invalidUniformLocation<GLuint>();

    for(const auto location : mBoneLocations)
        lRes &= (location != Constants::invalidUniformLocation<GLuint>());

    if (mUsePreviousBones)
    {
        for(const auto location : mPreviousBoneLocations)
            lRes &= (location != Constants::invalidUniformLocation<GLuint>());
    }

    return lRes;
}

void Skinning::init(unsigned int pPointLightCount, unsigned int pSpotLightCount, bool pActivateMotionBlur, const std::string & pPathVS, const std::string & pPathFS)
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

    LightingBase::pointLights(pPointLightCount);
    LightingBase::spotLights(pSpotLightCount);

    // Init the location of uniform variables
    LightingBase::initLightParametersLocations();

    // Init the location of uniform variables
    mWVPLocation = Program::uniformLocation("uWVP");
    mWorldLocation = Program::uniformLocation("uWorld");
    mLightWVPLocation = Program::uniformLocation("uLightWVP");

    mSamplerLocation = Program::uniformLocation("uSampler");
    mShadowMapLocation = Program::uniformLocation("uShadowMap");
    mNormalMapLocation = Program::uniformLocation("uNormalMap");
    mEyeWorldPosLocation = Program::uniformLocation("uEyeWorldPos");
    mMaterialSpecularIntensityLocation = Program::uniformLocation("uMaterialSpecularIntensity");
    mMaterialSpecularPowerLocation = Program::uniformLocation("uMaterialSpecularPower");

    // Use of sampler (color texture)
    mUseSamplerLocation = Program::uniformLocation("uUseSampler");

    // Use of shadow map
    mUseShadowMapLocation = Program::uniformLocation("uUseShadowMap");

    // Use of normal map
    mNormalMapLocation = Program::uniformLocation("uUseNormalMap");

    glUniform1i(mSamplerLocation, COLOR_TEXTURE_UNIT_INDEX); checkOpenGLState;
    glUniform1i(mShadowMapLocation, SHADOW_TEXTURE_UNIT_INDEX); checkOpenGLState;
    glUniform1i(mNormalMapLocation, NORMAL_TEXTURE_UNIT_INDEX); checkOpenGLState;

    mBoneLocations.assign(100, Constants::invalidUniformLocation<GLuint>());
    for (unsigned int i = 0; i < mBoneLocations.size(); ++i)
    {
        string lBone("uBone[");
        lBone.append(to_string(i));
        lBone.append("]");
        mBoneLocations[i] = Program::uniformLocation(lBone.c_str());
    }

    // We need to use the previous bones to be able to render motion blur
    mUsePreviousBones = pActivateMotionBlur;

    if (mUsePreviousBones)
    {
        mPreviousBoneLocations.assign(100, Constants::invalidUniformLocation<GLuint>());
        for (unsigned int i = 0; i < mBoneLocations.size(); ++i)
        {
            string lBone("uPreviousBone[");
            lBone.append(to_string(i));
            lBone.append("]");
            mPreviousBoneLocations[i] = Program::uniformLocation(lBone.c_str());
        }
    }

    // Check if we correctly initialized the uniform variables
    if (!Skinning::checkUniformLocations())
        throw Exceptions("Not all uniform locations were updated", __FILE__, __LINE__);

    // By default, use the color texture
    useSampler(true);
}

void Skinning::WVP(const mat4f & pWVP)
{
    glUniformMatrix4fv(mWVPLocation, 1, GL_TRUE, const_cast<mat4f&>(pWVP).data());
}

void Skinning::worldMatrix(const mat4f & pWorld)
{
    glUniformMatrix4fv(mWorldLocation, 1, GL_TRUE, const_cast<mat4f&>(pWorld).data());
}

void Skinning::lightWVP(const mat4f & pWVP)
{
    glUniformMatrix4fv(mLightWVPLocation, 1, GL_TRUE, const_cast<mat4f&>(pWVP).data());
}

void Skinning::eyeWorldPosition(const vec3f & pEyeWorldPos)
{
    glUniform3f(mEyeWorldPosLocation, pEyeWorldPos.x(), pEyeWorldPos.y(), pEyeWorldPos.z());
}

void Skinning::materialSpecularIntensity(float pValue)
{
    glUniform1f(mMaterialSpecularIntensityLocation, pValue);
}

void Skinning::materialSpecularPower(float pValue)
{
    glUniform1f(mMaterialSpecularPowerLocation, pValue);
}

void Skinning::useSampler(bool pActivate)
{
    glUniform1i(mUseSamplerLocation, pActivate?1:0);
}

void Skinning::useShadowMap(bool pActivate)
{
    glUniform1i(mUseShadowMapLocation, pActivate?1:0);
}

void Skinning::useNormalMap(bool pActivate)
{
    glUniform1i(mUseNormalMapLocation, pActivate?1:0);
}

void Skinning::boneTransforms(unsigned int pIndex, const mat4f & pTransform)
{
    glUniformMatrix4fv(mBoneLocations[pIndex], 1, GL_TRUE, const_cast<mat4f&>(pTransform).data());
}

void Skinning::previousBoneTransforms(unsigned int pIndex, mat4f & pTransform)
{
    if (mUsePreviousBones)
        glUniformMatrix4fv(mPreviousBoneLocations[pIndex], 1, GL_TRUE, pTransform.data());
}
