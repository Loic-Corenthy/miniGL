//===============================================================================================//
/*!
 *  \file      DirectionalLight.cpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#include "DirectionalLight.hpp"

#include <string>

#include "Log.hpp"

using std::string;
using miniGL::BaseLight;
using miniGL::DirectionalLight;
using miniGL::Log;

DirectionalLight::DirectionalLight(void)
:BaseLight(BaseLight::EType::DIRECTIONAL),
 mDirection(vec3f(0.0f))
{
    name("directional light");
}

DirectionalLight::DirectionalLight(const vec3f & pRGB,
                                   const vec3f & pDirection,
                                   float pAmbientIntensity,
                                   float pDiffuseIntensity)
:BaseLight(BaseLight::EType::DIRECTIONAL, pRGB, pAmbientIntensity, pDiffuseIntensity),
 mDirection(pDirection)
{
    name("directional light");
}

DirectionalLight::~DirectionalLight(void)
{
    string lMessage("Destructor ");
    lMessage.append(__FUNCTION__);
    Log::consoleMessage(lMessage);

    lMessage.assign("Deleting ");
    lMessage.append(name());
    Log::consoleMessage(lMessage);
}

void DirectionalLight::direction(const vec3f & pDirection)
{
    mDirection = pDirection;
}

const vec3f & DirectionalLight::direction(void) const
{
    return mDirection;
}

vec3f & DirectionalLight::direction(void)
{
    return mDirection;
}

void DirectionalLight::resetImplementation(void)
{
    BaseLight::resetImplementation();
    mDirection.x() = 0.0f;
    mDirection.y() = 0.0f;
    mDirection.z() = 0.0f;
}
