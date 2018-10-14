//===============================================================================================//
/*!
 *  \file      SSAOLighting.cpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#include "SSAOLighting.hpp"

#include <cassert>

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
using miniGL::SSAOLighting;
using miniGL::DirectionalLight;
using miniGL::PointLight;
using miniGL::SpotLight;
using miniGL::Constants;
using miniGL::toUT;
using miniGL::Shader;
using miniGL::Exceptions;
using miniGL::IOBuffer;

SSAOLighting::~SSAOLighting(void)
{
}

void SSAOLighting::init(unsigned int pPointLightCount, unsigned int pSpotLightCount)
{
    // Create, load and compile a vertex shader
    Shader lVS(Shader::ETYPE::VERTEX);

    lVS.loadText(R"(./Shaders/SSAOLighting.vert)");
    lVS.compile();

    // Create, load and compile a fragment shader
    Shader lFS(Shader::ETYPE::FRAGMENT);

    lFS.loadText(R"(./Shaders/SSAOLighting.frag)");
    lFS.compile();

    // Load the vertex and fragment shader into our program, link and validate
    attachShader(lVS);
    attachShader(lFS);

    link();

    // Use our program
    use();

    detachAndDeleteShader(lVS);
    detachAndDeleteShader(lFS);

    LightingBase::pointLights(pPointLightCount);
    LightingBase::spotLights(pSpotLightCount);

    // Init the location of uniform variables
    LightingBase::initLightParametersLocations();

    // Init uniform locations
    mSamplerLocation = Program::uniformLocation("uSampler");
    mUseSamplerLocation = Program::uniformLocation("uUseSampler");
    mEyeWorldPosLocation = Program::uniformLocation("uEyeWorldPos");
    mMaterialSpecularIntensityLocation = Program::uniformLocation("uMaterialSpecularIntensity");
    mMaterialSpecularPowerLocation = Program::uniformLocation("uMaterialSpecularPower");

    mWVPLocation = Program::uniformLocation("uWVP");
    mWorldLocation = Program::uniformLocation("uWorld");
    mShaderTypeLocation = Program::uniformLocation("uShaderType");
    mScreenSizeLocation = Program::uniformLocation("uScreenSize");
    mAmbientOcculusionTextureLocation = Program::uniformLocation("uAOMap");

    // Use index 0 for the color texture (i.e. the sampler)
    glUniform1i(mSamplerLocation, COLOR_TEXTURE_UNIT_INDEX); checkOpenGLState;

    // Use index 3 for ambient occulusion texture
    glUniform1i(mAmbientOcculusionTextureLocation, AMBIENT_OCCULUSION_TEXTURE_UNIT);

    // Check if we correctly initialized the uniform variables
    if (!checkUniformLocations())
        throw Exceptions("Not all uniform locations were updated", __FILE__, __LINE__);

    // By default, use the sampler
    useSampler(true);
}

void SSAOLighting::WVP(const mat4f & pWVP)
{
    glUniformMatrix4fv(mWVPLocation, 1, GL_TRUE, const_cast<mat4f&>(pWVP).data());
}

void SSAOLighting::world(const mat4f & pWorld)
{
    glUniformMatrix4fv(mWorldLocation, 1, GL_TRUE, const_cast<mat4f&>(pWorld).data());
}

void SSAOLighting::shaderType(int pShaderType)
{
    glUniform1i(mShaderTypeLocation, pShaderType);
}

void SSAOLighting::bindAOBuffer(IOBuffer & pBuffer)
{
    pBuffer.bindForReading(AMBIENT_OCCULUSION_TEXTURE_UNIT_INDEX);
}

void SSAOLighting::screenSize(unsigned int pWidth, unsigned int pHeight)
{
    glUniform2f(mScreenSizeLocation, static_cast<float>(pWidth), static_cast<float>(pHeight));
}

void SSAOLighting::useSampler(bool pActivate)
{
    glUniform1i(mUseSamplerLocation, pActivate?1:0);
}

bool SSAOLighting::checkUniformLocations(void) const
{
    bool lRes = true;

    lRes &= mSamplerLocation != Constants::invalidUniformLocation<GLuint>();
    lRes &= mUseSamplerLocation != Constants::invalidUniformLocation<GLuint>();
    lRes &= mEyeWorldPosLocation != Constants::invalidUniformLocation<GLuint>();
    lRes &= mMaterialSpecularIntensityLocation != Constants::invalidUniformLocation<GLuint>();
    lRes &= mMaterialSpecularPowerLocation != Constants::invalidUniformLocation<GLuint>();
    lRes &= mWVPLocation != Constants::invalidUniformLocation<GLuint>();
    lRes &= mWorldLocation != Constants::invalidUniformLocation<GLuint>();
    lRes &= mShaderTypeLocation != Constants::invalidUniformLocation<GLuint>();
    lRes &= mScreenSizeLocation != Constants::invalidUniformLocation<GLuint>();
    lRes &= mAmbientOcculusionTextureLocation != Constants::invalidUniformLocation<GLuint>();

    return lRes;
}
