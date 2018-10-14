//===============================================================================================//
/*!
 *  \file      CascadedShadowMapDirectionalLightLighting.cpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#include "CascadedShadowMapDirectionalLightLighting.hpp"

#include "Shader.hpp"
#include "EnumClassCast.hpp"
#include "Exceptions.hpp"
#include "EngineCommon.hpp"
#include "GLUtils.hpp"

using std::string;
using std::shared_ptr;
using miniGL::CascadedShadowMapDirectionalLightLighting;
using miniGL::Exceptions;
using miniGL::Shader;
using miniGL::DirectionalLight;

void CascadedShadowMapDirectionalLightLighting::init(const string & pPathVS, const string & pPathFS)
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
    mLightWVPLocation[0] = Program::uniformLocation("uLightWVP[0]");
    mLightWVPLocation[1] = Program::uniformLocation("uLightWVP[1]");
    mLightWVPLocation[2] = Program::uniformLocation("uLightWVP[2]");
    mWorldLocation = Program::uniformLocation("uWorld");
    mColorMapLocation = Program::uniformLocation("uSampler");
    mShadowMapLocation[0] = Program::uniformLocation("uShadowMap[0]");
    mShadowMapLocation[1] = Program::uniformLocation("uShadowMap[1]");
    mShadowMapLocation[2] = Program::uniformLocation("uShadowMap[2]");
    mEyeWorldPosLocation = Program::uniformLocation("uEyeWorldPos");
    mUseColorMapLocation = Program::uniformLocation("uUseSampler");
    mMaterialSpecularIntensityLocation = Program::uniformLocation("uMaterialSpecularIntensity");
    mMaterialSpecularPowerLocation = Program::uniformLocation("uMaterialSpecularPower");
    mCascadeEndClipSpaceLocation[0] = Program::uniformLocation("uCascadeEndClipSpace[0]");
    mCascadeEndClipSpaceLocation[1] = Program::uniformLocation("uCascadeEndClipSpace[1]");
    mCascadeEndClipSpaceLocation[2] = Program::uniformLocation("uCascadeEndClipSpace[2]");

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

void CascadedShadowMapDirectionalLightLighting::WVP(const mat4f & pWVP)
{
    glUniformMatrix4fv(mWVPLocation, 1, GL_TRUE, const_cast<mat4f&>(pWVP).data());
}

void CascadedShadowMapDirectionalLightLighting::lightWVP(size_t pCascadeIndex, const mat4f & pWVP)
{
    assert(pCascadeIndex < mLightWVPLocation.size());
    glUniformMatrix4fv(mLightWVPLocation[pCascadeIndex], 1, GL_TRUE, const_cast<mat4f&>(pWVP).data());
}

void CascadedShadowMapDirectionalLightLighting::cascadeEncClipSpace(size_t pCascadeIndex, float pEnd)
{
    glUniform1f(mCascadeEndClipSpaceLocation[pCascadeIndex], pEnd);
}

void CascadedShadowMapDirectionalLightLighting::world(const mat4f & pWorld)
{
    glUniformMatrix4fv(mWorldLocation, 1, GL_TRUE, const_cast<mat4f&>(pWorld).data());
}

void CascadedShadowMapDirectionalLightLighting::eyeWorldPosition(const vec3f & pPos)
{
    glUniform3f(mEyeWorldPosLocation, pPos.x(), pPos.y(), pPos.z());
}

void CascadedShadowMapDirectionalLightLighting::colorTextureUnit(unsigned int pTextureUnit)
{
    glUniform1i(mColorMapLocation, pTextureUnit);
}

void CascadedShadowMapDirectionalLightLighting::useColorTexture(bool pActivate)
{
    glUniform1i(mUseColorMapLocation, pActivate ? 1 : 0);
}

void CascadedShadowMapDirectionalLightLighting::shadowTextureUnit(void)
{
    glUniform1i(mShadowMapLocation[0], CASCADE_SHADOW_TEXTURE_UNIT0_INDEX); checkOpenGLState;
    glUniform1i(mShadowMapLocation[1], CASCADE_SHADOW_TEXTURE_UNIT1_INDEX); checkOpenGLState;
    glUniform1i(mShadowMapLocation[2], CASCADE_SHADOW_TEXTURE_UNIT2_INDEX); checkOpenGLState;
}

void CascadedShadowMapDirectionalLightLighting::materialSpecularIntensity(float pValue)
{
    glUniform1f(mMaterialSpecularIntensityLocation, pValue);
}

void CascadedShadowMapDirectionalLightLighting::materialSpecularPower(float pValue)
{
    glUniform1f(mMaterialSpecularPowerLocation, pValue);
}

void CascadedShadowMapDirectionalLightLighting::updateDirectionalLightState(const shared_ptr<DirectionalLight> pLight)
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

bool CascadedShadowMapDirectionalLightLighting::checkUniformLocations(void) const
{
    bool lRes = true;

    for (size_t i = 0; i < mCascadeEndClipSpaceLocation.size(); ++i)
    {
        lRes &= mCascadeEndClipSpaceLocation[i] != Constants::invalidUniformLocation<GLuint>();
        lRes &= mLightWVPLocation[i] != Constants::invalidUniformLocation<GLuint>();
        lRes &= mShadowMapLocation[i]  != Constants::invalidUniformLocation<GLuint>();
    }

    return (lRes                                                                                &&
            mDirectionalLightColorLocation != Constants::invalidUniformLocation<GLuint>()       &&
            mDirectionalLightAmbientLocation != Constants::invalidUniformLocation<GLuint>()     &&
            mDirectionalLightDirectionLocation != Constants::invalidUniformLocation<GLuint>()   &&
            mDirectionalLightDiffuseLocation != Constants::invalidUniformLocation<GLuint>()     &&
            mWVPLocation != Constants::invalidUniformLocation<GLuint>()                         &&
            mWorldLocation != Constants::invalidUniformLocation<GLuint>()                       &&
            mColorMapLocation != Constants::invalidUniformLocation<GLuint>()                    &&
            mUseColorMapLocation != Constants::invalidUniformLocation<GLuint>()                 &&
            mEyeWorldPosLocation != Constants::invalidUniformLocation<GLuint>()                 &&
            mMaterialSpecularIntensityLocation != Constants::invalidUniformLocation<GLuint>()   &&
            mMaterialSpecularPowerLocation != Constants::invalidUniformLocation<GLuint>()       );
}
