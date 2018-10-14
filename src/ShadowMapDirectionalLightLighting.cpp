//===============================================================================================//
/*!
 *  \file      ShadowMapDirectionalLightLighting.cpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#include "ShadowMapDirectionalLightLighting.hpp"

#include "Shader.hpp"
#include "EnumClassCast.hpp"
#include "Exceptions.hpp"

using std::string;
using std::shared_ptr;
using miniGL::ShadowMapDirectionalLightLighting;
using miniGL::Exceptions;
using miniGL::Shader;
using miniGL::DirectionalLight;

void ShadowMapDirectionalLightLighting::init(const string & pPathVS, const string & pPathFS)
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
    mLightWVPLocation = Program::uniformLocation("uLightWVP");
    mWorldLocation = Program::uniformLocation("uWorld");
    mColorMapLocation = Program::uniformLocation("uSampler");
    mShadowMapLocation = Program::uniformLocation("uShadowMap");
    mEyeWorldPosLocation = Program::uniformLocation("uEyeWorldPos");
    mUseColorMapLocation = Program::uniformLocation("uUseSampler");
    mMaterialSpecularIntensityLocation = Program::uniformLocation("uMaterialSpecularIntensity");
    mMaterialSpecularPowerLocation = Program::uniformLocation("uMaterialSpecularPower");

    // Directional light parameters
    mDirectionalLightColorLocation = Program::uniformLocation("uDirectionalLight.base.color");
    mDirectionalLightAmbientLocation = Program::uniformLocation("uDirectionalLight.base.ambientIntensity");
    mDirectionalLightDirectionLocation = Program::uniformLocation("uDirectionalLight.direction");
    mDirectionalLightDiffuseLocation = Program::uniformLocation("uDirectionalLight.base.diffuseIntensity");

    // Check if we correctly initialized the uniform variables
    if (!checkUniformLocations())
        throw Exceptions("Not all uniform locations were updated", __FILE__, __LINE__);


    useColorTexture(true);
}

void ShadowMapDirectionalLightLighting::WVP(const mat4f & pWVP)
{
    glUniformMatrix4fv(mWVPLocation, 1, GL_TRUE, const_cast<mat4f&>(pWVP).data());
}

void ShadowMapDirectionalLightLighting::lightWVP(const mat4f & pWVP)
{
    glUniformMatrix4fv(mLightWVPLocation, 1, GL_TRUE, const_cast<mat4f&>(pWVP).data());
}

void ShadowMapDirectionalLightLighting::world(const mat4f & pWorld)
{
    glUniformMatrix4fv(mWorldLocation, 1, GL_TRUE, const_cast<mat4f&>(pWorld).data());
}

void ShadowMapDirectionalLightLighting::eyeWorldPosition(const vec3f & pPos)
{
    glUniform3f(mEyeWorldPosLocation, pPos.x(), pPos.y(), pPos.z());
}

void ShadowMapDirectionalLightLighting::colorTextureUnit(unsigned int pTextureUnit)
{
    glUniform1i(mColorMapLocation, pTextureUnit);
}

void ShadowMapDirectionalLightLighting::useColorTexture(bool pActivate)
{
    glUniform1i(mUseColorMapLocation, pActivate ? 1 : 0);
}

void ShadowMapDirectionalLightLighting::shadowTextureUnit(unsigned int pTextureUnit)
{
    glUniform1i(mShadowMapLocation, pTextureUnit);
}

void ShadowMapDirectionalLightLighting::materialSpecularIntensity(float pValue)
{
    glUniform1f(mMaterialSpecularIntensityLocation, pValue);
}

void ShadowMapDirectionalLightLighting::materialSpecularPower(float pValue)
{
    glUniform1f(mMaterialSpecularPowerLocation, pValue);
}

void ShadowMapDirectionalLightLighting::updateDirectionalLightState(const shared_ptr<DirectionalLight> pLight)
{
    if (pLight != nullptr)
    {
        // Update directional light
        vec3f lColor = pLight->color();
        glUniform3f(mDirectionalLightColorLocation, lColor.x(), lColor.y(), lColor.z());
        glUniform1f(mDirectionalLightAmbientLocation, pLight->ambientIntensity());

        vec3f lDirection = pLight->direction();
        lDirection.normalize();
        glUniform3f(mDirectionalLightDirectionLocation, lDirection.x(), lDirection.y(), lDirection.z());
        glUniform1f(mDirectionalLightDiffuseLocation, pLight->diffuseIntensity());
    }
    else
        throw Exceptions("Trying to set a directional light with a nullptr", __FILE__, __LINE__);
}

bool ShadowMapDirectionalLightLighting::checkUniformLocations(void) const
{
    return (mDirectionalLightColorLocation != Constants::invalidUniformLocation<GLuint>()       &&
            mDirectionalLightAmbientLocation != Constants::invalidUniformLocation<GLuint>()     &&
            mDirectionalLightDirectionLocation != Constants::invalidUniformLocation<GLuint>()   &&
            mDirectionalLightDiffuseLocation != Constants::invalidUniformLocation<GLuint>()     &&
            mWVPLocation != Constants::invalidUniformLocation<GLuint>()                         &&
            mLightWVPLocation != Constants::invalidUniformLocation<GLuint>()                    &&
            mWorldLocation != Constants::invalidUniformLocation<GLuint>()                        &&
            mColorMapLocation != Constants::invalidUniformLocation<GLuint>()                    &&
            mUseColorMapLocation != Constants::invalidUniformLocation<GLuint>()                 &&
            mShadowMapLocation != Constants::invalidUniformLocation<GLuint>()                    &&
            mEyeWorldPosLocation != Constants::invalidUniformLocation<GLuint>()                 &&
            mMaterialSpecularIntensityLocation != Constants::invalidUniformLocation<GLuint>()    &&
            mMaterialSpecularPowerLocation != Constants::invalidUniformLocation<GLuint>()        );
}
