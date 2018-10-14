//===============================================================================================//
/*!
 *  \file      BaseLight.cpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#include "BaseLight.hpp"

#include <string>

#include "Log.hpp"

using std::string;
using miniGL::BaseLight;
using miniGL::Log;

BaseLight::BaseLight(EType pType)
:mColor(vec3f(0.0f)),
 mAmbientIntensity(0.0f),
 mDiffuseIntensity(0.0f),
 mType(pType)
{
    // Users should not create a light of undefined type
    assert(pType != EType::UNDEFINED);
}

BaseLight::BaseLight(EType pType, const vec3f & pRGB, float pAmbientIntensity, float pDiffuseIntensity)
:mColor(pRGB),
 mAmbientIntensity(pAmbientIntensity),
 mDiffuseIntensity(pDiffuseIntensity),
 mType(pType)
{
    // Users should not create a light of undefined type
    assert(pType != EType::UNDEFINED);
}

BaseLight::~BaseLight()
{
    string lMessage("Destructor ");
    lMessage.append(__FUNCTION__);
    Log::consoleMessage(lMessage);

    lMessage.assign("Deleting ");
    lMessage.append(name());
    Log::consoleMessage(lMessage);
}

void BaseLight::color(const vec3f & pRGB)
{
    mColor = pRGB;
}

void BaseLight::ambientIntensity(float pValue)
{
    mAmbientIntensity = pValue;
}

void BaseLight::diffuseIntensity(float pValue)
{
    mDiffuseIntensity = pValue;
}

const vec3f & BaseLight::color(void) const
{
    return mColor;
}

float BaseLight::ambientIntensity(void) const
{
    return mAmbientIntensity;
}

float BaseLight::diffuseIntensity(void) const
{
    return mDiffuseIntensity;
}

vec3f & BaseLight::color(void)
{
    return mColor;
}

float & BaseLight::ambientIntensity(void)
{
    return mAmbientIntensity;
}

float & BaseLight::diffuseIntensity(void)
{
    return mDiffuseIntensity;
}

void BaseLight::type(BaseLight::EType pType) noexcept
{
    mType = pType;
}

void BaseLight::name(const string & pName)
{
    mName = pName;
}

BaseLight::EType BaseLight::type(void) const noexcept
{
    return mType;
}

string BaseLight::name(void) const
{
    return mName;
}

void BaseLight::reset(void)
{
    resetImplementation();
}

void BaseLight::resetImplementation(void)
{
    mColor.x() = 0.0f;
    mColor.y() = 0.0f;
    mColor.z() = 0.0f;
    mAmbientIntensity = 0.0f;
    mDiffuseIntensity = 0.0f;
}
