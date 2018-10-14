//===============================================================================================//
/*!
 *  \file      Lighting.cpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#include "Lighting.hpp"

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
using miniGL::Lighting;
using miniGL::LightingBase;
using miniGL::DirectionalLight;
using miniGL::PointLight;
using miniGL::SpotLight;
using miniGL::Constants;
using miniGL::toUT;
using miniGL::Shader;
using miniGL::Exceptions;

Lighting::Lighting(void)
:LightingBase()
{
    // Init all the location indices to invalid location
    for (auto & param : mGeneralParameterLocations)
        param = Constants::invalidUniformLocation<GLuint>();

    for(auto & param : mNormalLightingParameterLocations)
        param = Constants::invalidUniformLocation<GLuint>();
}

Lighting::~Lighting(void)
{
}

bool Lighting::checkUniformLocations(void) const
{
    bool lRes = true;

    // Check that all the location indices are set to a valid value
    for (const auto param : mGeneralParameterLocations)
        lRes &= (param != Constants::invalidUniformLocation<GLuint>());

    // Check that all the location indices are set to a valid value
    for (const auto param :mNormalLightingParameterLocations)
        lRes &= (param != Constants::invalidUniformLocation<GLuint>());

    return lRes;
}

void Lighting::init(unsigned int pPointLightCount, unsigned int pSpotLightCount, const string & pPathVS, const string & pPathFS)
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
    initUniformLocation(Lighting::GENERAL_LIGHTING_PARAM::LIGHT_WVP, "uLightWVP");
    initUniformLocation(Lighting::GENERAL_LIGHTING_PARAM::SAMPLER, "uSampler");
    initUniformLocation(Lighting::GENERAL_LIGHTING_PARAM::SHADOW_MAP, "uShadowMap");
    initUniformLocation(Lighting::GENERAL_LIGHTING_PARAM::SHADOW_MAP_SIZE, "uShadowMapSize");
    initUniformLocation(Lighting::GENERAL_LIGHTING_PARAM::NORMAL_MAP, "uNormalMap");
    initUniformLocation(Lighting::GENERAL_LIGHTING_PARAM::EYE_WORLD_POS, "uEyeWorldPos");
    initUniformLocation(Lighting::GENERAL_LIGHTING_PARAM::MATERIAL_SPECULAR_INTENSITY, "uMaterialSpecularIntensity");
    initUniformLocation(Lighting::GENERAL_LIGHTING_PARAM::MATERIAL_SPECULAR_POWER, "uMaterialSpecularPower");

    initUniformLocation(Lighting::NORMAL_LIGHTING_PARAM::WVP, "uWVP");
    initUniformLocation(Lighting::NORMAL_LIGHTING_PARAM::WORLD_MATRIX, "uWorld");

    // Use of sampler (color texture)
    initUniformLocation(Lighting::GENERAL_LIGHTING_PARAM::USE_SAMPLER, "uUseSampler");

    // Use of shadow map
    initUniformLocation(Lighting::GENERAL_LIGHTING_PARAM::USE_SHADOW_MAP, "uUseShadowMap");

    // Use of normal map
    initUniformLocation(Lighting::GENERAL_LIGHTING_PARAM::USE_NORMAL_MAP, "uUseNormalMap");

    // Check if we correctly initialized the uniform variables
    if (!Lighting::checkUniformLocations())
        throw Exceptions("Not all uniform locations were updated", __FILE__, __LINE__);

    textureUnit(Lighting::GENERAL_LIGHTING_PARAM::SAMPLER, COLOR_TEXTURE_UNIT_INDEX);
    textureUnit(Lighting::GENERAL_LIGHTING_PARAM::SHADOW_MAP, SHADOW_TEXTURE_UNIT_INDEX);
    textureUnit(Lighting::GENERAL_LIGHTING_PARAM::NORMAL_MAP, NORMAL_TEXTURE_UNIT_INDEX);

    // By default, use the color texture
    useSampler(true);
}

void Lighting::WVP(const mat4f & pWVP)
{
    glUniformMatrix4fv(mNormalLightingParameterLocations[toUT(NORMAL_LIGHTING_PARAM::WVP)], 1, GL_TRUE, const_cast<mat4f&>(pWVP).data());
}

void Lighting::worldMatrix(const mat4f & pWorld)
{
    glUniformMatrix4fv(mNormalLightingParameterLocations[toUT(NORMAL_LIGHTING_PARAM::WORLD_MATRIX)], 1, GL_TRUE, const_cast<mat4f&>(pWorld).data());
}

void Lighting::lightWVP(const mat4f & pWVP)
{
    glUniformMatrix4fv(mGeneralParameterLocations[toUT(GENERAL_LIGHTING_PARAM::LIGHT_WVP)], 1, GL_TRUE, const_cast<mat4f&>(pWVP).data());
}

void Lighting::eyeWorldPosition(const vec3f & pEyeWorldPos)
{
    glUniform3f(mGeneralParameterLocations[toUT(GENERAL_LIGHTING_PARAM::EYE_WORLD_POS)], pEyeWorldPos.x(), pEyeWorldPos.y(), pEyeWorldPos.z());
}

void Lighting::textureUnit(GENERAL_LIGHTING_PARAM pParam, unsigned int pTexUnit)
{
    switch (pParam)
    {
        case GENERAL_LIGHTING_PARAM::SAMPLER:
            glUniform1i(mGeneralParameterLocations[toUT(GENERAL_LIGHTING_PARAM::SAMPLER)], pTexUnit);
            checkOpenGLState;
            break;

        case GENERAL_LIGHTING_PARAM::SHADOW_MAP:
            glUniform1i(mGeneralParameterLocations[toUT(GENERAL_LIGHTING_PARAM::SHADOW_MAP)], pTexUnit);
            checkOpenGLState;
            break;

        case GENERAL_LIGHTING_PARAM::NORMAL_MAP:
            glUniform1i(mGeneralParameterLocations[toUT(GENERAL_LIGHTING_PARAM::NORMAL_MAP)], pTexUnit);
            checkOpenGLState;
            break;

        default:
            assert(false && "Wrong texture parameter");
            break;
    }
}

void Lighting::materialSpecularIntensity(float pValue)
{
    glUniform1f(mGeneralParameterLocations[toUT(GENERAL_LIGHTING_PARAM::MATERIAL_SPECULAR_INTENSITY)], pValue);
}

void Lighting::materialSpecularPower(float pValue)
{
    glUniform1f(mGeneralParameterLocations[toUT(GENERAL_LIGHTING_PARAM::MATERIAL_SPECULAR_POWER)], pValue);
}

void Lighting::useSampler(bool pActivate)
{
    glUniform1i(mGeneralParameterLocations[toUT(GENERAL_LIGHTING_PARAM::USE_SAMPLER)], pActivate?1:0);
}

void Lighting::useShadowMap(bool pActivate)
{
    glUniform1i(mGeneralParameterLocations[toUT(GENERAL_LIGHTING_PARAM::USE_SHADOW_MAP)], pActivate?1:0);
}

void Lighting::shadowMapSize(unsigned int pFramebufferWidth, unsigned int pFramebufferHeight)
{
    glUniform2f(mGeneralParameterLocations[toUT(GENERAL_LIGHTING_PARAM::SHADOW_MAP_SIZE)], static_cast<float>(pFramebufferWidth), static_cast<float>(pFramebufferHeight));
}

void Lighting::useNormalMap(bool pActivate)
{
    glUniform1i(mGeneralParameterLocations[toUT(GENERAL_LIGHTING_PARAM::USE_NORMAL_MAP)], pActivate?1:0);
}

void Lighting::initUniformLocation(GENERAL_LIGHTING_PARAM pParameter, const char* pName)
{
    mGeneralParameterLocations[toUT(pParameter)] = Program::uniformLocation(pName);
}

void Lighting::initUniformLocation(NORMAL_LIGHTING_PARAM pParameter, const char* pName)
{
    mNormalLightingParameterLocations[toUT(pParameter)] = Program::uniformLocation(pName);
}
