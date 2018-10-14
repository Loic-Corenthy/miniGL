//===============================================================================================//
/*!
 *  \file      PointLight.cpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#include "PointLight.hpp"

#include <string>

#include "Log.hpp"

using std::string;
using miniGL::BaseLight;
using miniGL::PointLight;
using miniGL::Log;

PointLight::PointLight(void)
:BaseLight(BaseLight::EType::POINT),
 mPosition(vec3f(0.0f)),
 mAttenuation(vec3f(1.0f,0.0f,0.0f))
{
    name("point light");
}

PointLight::PointLight(const vec3f & pRGB, const vec3f & pPos, float pAmbInt, float pDiffInt, float pConstAtt, float pLinearAtt, float pExpAtt)
:BaseLight(BaseLight::EType::POINT, pRGB,pAmbInt,pDiffInt),
 mPosition(pPos),
 mAttenuation(vec3f({pConstAtt, pLinearAtt, pExpAtt}))
{
    name("point light");
}

PointLight::~PointLight(void)
{
    string lMessage("Destructor ");
    lMessage.append(__FUNCTION__);
    Log::consoleMessage(lMessage);

    lMessage.assign("Deleting ");
    lMessage.append(name());
    Log::consoleMessage(lMessage);
}

void PointLight::position(const vec3f & pPos)
{
    mPosition = pPos;
}

void PointLight::attenuation(ATTENUATION_TYPE pType, float pValue)
{
    mAttenuation[toUT(pType)] = pValue;
}

const vec3f & PointLight::position(void) const
{
    return mPosition;
}

float PointLight::attenuation(ATTENUATION_TYPE pType) const
{
    return mAttenuation[toUT(pType)];
}

vec3f & PointLight::position(void)
{
    return mPosition;
}

float & PointLight::attenuation(ATTENUATION_TYPE pType)
{
    return mAttenuation[toUT(pType)];
}

void PointLight::resetImplementation(void)
{
    BaseLight::resetImplementation();
    mPosition.x() = 0.0f;
    mPosition.y() = 0.0f;
    mPosition.z() = 0.0f;
    mAttenuation.x() = 1.0f;
    mAttenuation.y() = 0.0f;
    mAttenuation.z() = 0.0f;
}
