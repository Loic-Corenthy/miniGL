//===============================================================================================//
/*!
 *  \file      MultipassShadowMapLighting.cpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#include "MultipassShadowMapLighting.hpp"

#include "Shader.hpp"
#include "EnumClassCast.hpp"
#include "Exceptions.hpp"

using std::string;
using std::shared_ptr;
using miniGL::MultipassShadowMapLighting;
using miniGL::Exceptions;
using miniGL::Shader;
using miniGL::PointLight;

void MultipassShadowMapLighting::init(const string & pPathVS, const string & pPathFS)
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
    mWorldLocation = Program::uniformLocation("uWorld");
    mColorMapLocation = Program::uniformLocation("uSampler");
    mShadowMapLocation = Program::uniformLocation("uShadowMap");
    mEyeWorldPosLocation = Program::uniformLocation("uEyeWorldPos");
    mUseColorMapLocation = Program::uniformLocation("uUseSampler");
    mUseNormalMapLocation = Program::uniformLocation("uUseNormalMap");
    mMaterialSpecularIntensityLocation = Program::uniformLocation("uMaterialSpecularIntensity");
    mMaterialSpecularPowerLocation = Program::uniformLocation("uMaterialSpecularPower");

    string lParamName;

    string lPointLightName("uPointLight");

    lParamName = lPointLightName + string(".base.color");
    mPointLightLocation[toUT(EPointLightParam::color)] = Program::uniformLocation(lParamName.c_str());

    lParamName = lPointLightName + string(".base.ambientIntensity");
    mPointLightLocation[toUT(EPointLightParam::ambiantIntensity)] = Program::uniformLocation(lParamName.c_str());

    lParamName = lPointLightName + string(".base.diffuseIntensity");
    mPointLightLocation[toUT(EPointLightParam::diffuseIntensity)] = Program::uniformLocation(lParamName.c_str());

    lParamName = lPointLightName + string(".position");
    mPointLightLocation[toUT(EPointLightParam::position)] = Program::uniformLocation(lParamName.c_str());

    lParamName = lPointLightName + string(".attenuation.constant");
    mPointLightLocation[toUT(EPointLightParam::attenuationConstant)] = Program::uniformLocation(lParamName.c_str());

    lParamName = lPointLightName + string(".attenuation.linear");
    mPointLightLocation[toUT(EPointLightParam::attenuationLinear)] = Program::uniformLocation(lParamName.c_str());

    lParamName = lPointLightName + string(".attenuation.exponential");
    mPointLightLocation[toUT(EPointLightParam::attenuationExponential)] = Program::uniformLocation(lParamName.c_str());

    // Check if we correctly initialized the uniform variables
    if (!checkUniformLocations())
        throw Exceptions("Not all uniform locations were updated", __FILE__, __LINE__);


    // Use our program
    use();

    useColorTexture(true);
}

void MultipassShadowMapLighting::updatePointLightState(const shared_ptr<PointLight> pLight)
{
    if (pLight != nullptr)
    {
        vec3f lColor = pLight->color();
        glUniform3f(mPointLightLocation[toUT(EPointLightParam::color)], lColor.x(), lColor.y(), lColor.z());
        glUniform1f(mPointLightLocation[toUT(EPointLightParam::ambiantIntensity)], pLight->ambientIntensity());
        glUniform1f(mPointLightLocation[toUT(EPointLightParam::diffuseIntensity)], pLight->diffuseIntensity());
        glUniform3f(mPointLightLocation[toUT(EPointLightParam::position)], pLight->position().x(), pLight->position().y(), pLight->position().z());
        glUniform1f(mPointLightLocation[toUT(EPointLightParam::attenuationConstant)], pLight->attenuation(PointLight::ATTENUATION_TYPE::CONSTANT));
        glUniform1f(mPointLightLocation[toUT(EPointLightParam::attenuationLinear)], pLight->attenuation(PointLight::ATTENUATION_TYPE::LINEAR));
        glUniform1f(mPointLightLocation[toUT(EPointLightParam::attenuationExponential)], pLight->attenuation(PointLight::ATTENUATION_TYPE::EXPONENTIAL));
    }
    else
        throw Exceptions("Trying to set a point light with a nullptr", __FILE__, __LINE__);
}

void MultipassShadowMapLighting::WVP(const mat4f & pWVP)
{
    glUniformMatrix4fv(mWVPLocation, 1, GL_TRUE, const_cast<mat4f&>(pWVP).data());
}

void MultipassShadowMapLighting::world(const mat4f & pWorld)
{
    glUniformMatrix4fv(mWorldLocation, 1, GL_TRUE, const_cast<mat4f&>(pWorld).data());
}

void MultipassShadowMapLighting::eyeWorldPosition(const vec3f & pPos)
{
    glUniform3f(mEyeWorldPosLocation, pPos.x(), pPos.y(), pPos.z());
}

void MultipassShadowMapLighting::colorTextureUnit(unsigned int pTextureUnit)
{
    glUniform1i(mColorMapLocation, pTextureUnit);
}

void MultipassShadowMapLighting::useColorTexture(bool pActivate)
{
    glUniform1i(mUseColorMapLocation, pActivate ? 1 : 0);
}

void MultipassShadowMapLighting::shadowTextureUnit(unsigned int pTextureUnit)
{
    glUniform1i(mShadowMapLocation, pTextureUnit);
}

void MultipassShadowMapLighting::useNormalMap(bool pActivate)
{
    glUniform1i(mUseNormalMapLocation, pActivate ? 1 : 0);
}

void MultipassShadowMapLighting::materialSpecularIntensity(float pValue)
{
    glUniform1f(mMaterialSpecularIntensityLocation, pValue);
}

void MultipassShadowMapLighting::materialSpecularPower(float pValue)
{
    glUniform1f(mMaterialSpecularPowerLocation, pValue);
}

bool MultipassShadowMapLighting::checkUniformLocations(void) const
{
    bool lRes = (mWVPLocation != Constants::invalidUniformLocation<GLuint>()                       &&
                 mWorldLocation != Constants::invalidUniformLocation<GLuint>()                     &&
                 mColorMapLocation != Constants::invalidUniformLocation<GLuint>()                  &&
                 mUseColorMapLocation != Constants::invalidUniformLocation<GLuint>()               &&
                 mUseNormalMapLocation != Constants::invalidUniformLocation<GLuint>()              &&
                 mShadowMapLocation != Constants::invalidUniformLocation<GLuint>()                 &&
                 mEyeWorldPosLocation != Constants::invalidUniformLocation<GLuint>()               &&
                 mMaterialSpecularIntensityLocation != Constants::invalidUniformLocation<GLuint>() &&
                 mMaterialSpecularPowerLocation != Constants::invalidUniformLocation<GLuint>()     );

    for (const auto it : mPointLightLocation)
        lRes &= (it != Constants::invalidUniformLocation<GLuint>());

    return lRes;
}
