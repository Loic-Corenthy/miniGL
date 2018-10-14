//===============================================================================================//
/*!
 *  \file      TessellationLighting.cpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#include "TessellationLighting.hpp"

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
using std::shared_ptr;
using std::to_string;
using miniGL::TessellationLighting;
using miniGL::Constants;
using miniGL::toUT;
using miniGL::Shader;
using miniGL::Exceptions;

TessellationLighting::TessellationLighting(TessellationLighting::EMode pMode)
:LightingBase(),
 mMode(pMode)
{
    // Init all the location indices to invalid location
    for(auto & param : mGeneralParameterLocations)
        param = Constants::invalidUniformLocation<GLuint>();
}

TessellationLighting::~TessellationLighting(void)
{
}

bool TessellationLighting::checkUniformLocations(void) const
{
    bool lRes = true;

    // Check that all the location indices are set to a valid value
    switch (mMode)
    {
        case EMode::DEFAULT:
            for(size_t i = 0; i < mGeneralParameterLocations.size(); ++i)
            {
                if(i == toUT(GENERAL_LIGHTING_PARAM::TESSELLATION_LEVEL))
                   continue;

                lRes &= (mGeneralParameterLocations[i] != Constants::invalidUniformLocation<GLuint>());
            }
            break;

        case EMode::POINT_NORMAL:
            for(size_t i = 0; i < mGeneralParameterLocations.size(); ++i)
            {
                if(i == toUT(GENERAL_LIGHTING_PARAM::MAX_DISTANCE_TO_CAMERA_COEFF) || i == toUT(GENERAL_LIGHTING_PARAM::DISPLACEMENT_MAP) || i == toUT(GENERAL_LIGHTING_PARAM::DISPLACEMENT_FACTOR))
                    continue;

                lRes &= (mGeneralParameterLocations[i] != Constants::invalidUniformLocation<GLuint>());
            }
            break;

        default:
            assert(false && "Wrong mode in TessellationLighting");
            break;
    }


    return lRes;
}

void TessellationLighting::init(unsigned int pPointLightCount, unsigned int pSpotLightCount, const string & pPathVS, const string & pPathCS, const string & pPathES, const string & pPathFS)
{
    // Create, load and compile a vertex shader
    Shader lVS(Shader::ETYPE::VERTEX);

    lVS.loadText(pPathVS.c_str());
    lVS.compile();

    // Create, load and compile a control shader
    Shader lCS(Shader::ETYPE::TESSELLATION_CONTROL);

    lCS.loadText(pPathCS.c_str());
    lCS.compile();

    // Create, load and compile a vertex shader
    Shader lES(Shader::ETYPE::TESSELLATION_EVALUATION);

    lES.loadText(pPathES.c_str());
    lES.compile();

    // Create, load and compile a fragment shader
    Shader lFS(Shader::ETYPE::FRAGMENT);

    lFS.loadText(pPathFS.c_str());
    lFS.compile();

    // Load the vertex and fragment shader into our program, link and validate
    attachShader(lVS);
    attachShader(lCS);
    attachShader(lES);
    attachShader(lFS);

    link();

    detachAndDeleteShader(lVS);
    detachAndDeleteShader(lCS);
    detachAndDeleteShader(lES);
    detachAndDeleteShader(lFS);

    // Use our program
    use();

    LightingBase::pointLights(pPointLightCount);
    LightingBase::spotLights(pSpotLightCount);

    // Init the location of uniform variables
    LightingBase::initLightParametersLocations();

    // General parameters
    initUniformLocation(GENERAL_LIGHTING_PARAM::VP, "uVP");
    initUniformLocation(GENERAL_LIGHTING_PARAM::WORLD_MATRIX, "uWorld");
    initUniformLocation(GENERAL_LIGHTING_PARAM::SAMPLER, "uSampler");

    switch (mMode)
    {
        case EMode::DEFAULT:
            initUniformLocation(GENERAL_LIGHTING_PARAM::MAX_DISTANCE_TO_CAMERA_COEFF, "uMaxDistanceToCameraCoeff");
            initUniformLocation(GENERAL_LIGHTING_PARAM::DISPLACEMENT_MAP, "uDisplacementMap");
            initUniformLocation(GENERAL_LIGHTING_PARAM::DISPLACEMENT_FACTOR, "uDisplacementFactor");
            break;

        case EMode::POINT_NORMAL:
            initUniformLocation(GENERAL_LIGHTING_PARAM::TESSELLATION_LEVEL, "uTessellationLevel");
            break;

        default:
            assert(false && "Wrong mode in TessellationLighting");
            break;
    }


    initUniformLocation(GENERAL_LIGHTING_PARAM::EYE_WORLD_POS, "uEyeWorldPos");
    initUniformLocation(GENERAL_LIGHTING_PARAM::MATERIAL_SPECULAR_INTENSITY, "uMaterialSpecularIntensity");
    initUniformLocation(GENERAL_LIGHTING_PARAM::MATERIAL_SPECULAR_POWER, "uMaterialSpecularPower");

    // Use of sampler (color texture)
    initUniformLocation(GENERAL_LIGHTING_PARAM::USE_SAMPLER, "uUseSampler");

    // Check if we correctly initialized the uniform variables
    if (!checkUniformLocations())
        throw Exceptions("Not all uniform locations were updated", __FILE__, __LINE__);

    // By default, use the color texture
    useSampler(true);

    textureUnit(TessellationLighting::GENERAL_LIGHTING_PARAM::SAMPLER, COLOR_TEXTURE_UNIT_INDEX);
    textureUnit(TessellationLighting::GENERAL_LIGHTING_PARAM::DISPLACEMENT_MAP, DISPLACEMENT_TEXTURE_UNIT_INDEX);

    // By default, set the max distance to the camera to 10
    if(mMode == EMode::DEFAULT)
        maxDistanceToCameraCoeff(10.0f);
}

void TessellationLighting::VP(const mat4f & pVP)
{
    glUniformMatrix4fv(mGeneralParameterLocations[toUT(GENERAL_LIGHTING_PARAM::VP)], 1, GL_TRUE, const_cast<mat4f&>(pVP).data());
}

void TessellationLighting::worldMatrix(const mat4f & pWorld)
{
    glUniformMatrix4fv(mGeneralParameterLocations[toUT(GENERAL_LIGHTING_PARAM::WORLD_MATRIX)], 1, GL_TRUE, const_cast<mat4f&>(pWorld).data());
}

void TessellationLighting::eyeWorldPosition(const vec3f & pEyeWorldPos)
{
    glUniform3f(mGeneralParameterLocations[toUT(GENERAL_LIGHTING_PARAM::EYE_WORLD_POS)], pEyeWorldPos.x(), pEyeWorldPos.y(), pEyeWorldPos.z());
}

void TessellationLighting::textureUnit(GENERAL_LIGHTING_PARAM pParam, unsigned int pTexUnit)
{
    switch (pParam)
    {
        case GENERAL_LIGHTING_PARAM::SAMPLER:
            glUniform1i(mGeneralParameterLocations[toUT(GENERAL_LIGHTING_PARAM::SAMPLER)], pTexUnit);
            checkOpenGLState;
            break;

        case GENERAL_LIGHTING_PARAM::DISPLACEMENT_MAP:
            glUniform1i(mGeneralParameterLocations[toUT(GENERAL_LIGHTING_PARAM::DISPLACEMENT_MAP)], pTexUnit);
            checkOpenGLState;
            break;

        default:
            assert(false && "Wrong texture parameter");
            break;
    }
}

void TessellationLighting::materialSpecularIntensity(float pValue)
{
    glUniform1f(mGeneralParameterLocations[toUT(GENERAL_LIGHTING_PARAM::MATERIAL_SPECULAR_INTENSITY)], pValue);
}

void TessellationLighting::materialSpecularPower(float pValue)
{
    glUniform1f(mGeneralParameterLocations[toUT(GENERAL_LIGHTING_PARAM::MATERIAL_SPECULAR_POWER)], pValue);
}

void TessellationLighting::useSampler(bool pActivate)
{
    glUniform1i(mGeneralParameterLocations[toUT(GENERAL_LIGHTING_PARAM::USE_SAMPLER)], pActivate?1:0);
}

void TessellationLighting::displacementFactor(float pValue)
{
    assert(mMode == EMode::DEFAULT);
    glUniform1f(mGeneralParameterLocations[toUT(GENERAL_LIGHTING_PARAM::DISPLACEMENT_FACTOR)], pValue);
}

void TessellationLighting::maxDistanceToCameraCoeff(float pValue)
{
    assert(mMode == EMode::DEFAULT);
    glUniform1f(mGeneralParameterLocations[toUT(GENERAL_LIGHTING_PARAM::MAX_DISTANCE_TO_CAMERA_COEFF)], pValue);
}

void TessellationLighting::tessellationLevel(float pValue)
{
    assert(mMode == EMode::POINT_NORMAL);
    glUniform1f(mGeneralParameterLocations[toUT(GENERAL_LIGHTING_PARAM::TESSELLATION_LEVEL)], pValue);
}

GLint TessellationLighting::initUniformLocation(GENERAL_LIGHTING_PARAM pParameter, const char* pName)
{
    GLint lID = Program::uniformLocation(pName);
    mGeneralParameterLocations[toUT(pParameter)] = lID;
    return lID;
}

