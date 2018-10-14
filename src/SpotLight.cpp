//===============================================================================================//
/*!
 *  \file      PointLight.cpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#include "SpotLight.hpp"

#include <string>

#include "Log.hpp"

using std::string;
using miniGL::BaseLight;
using miniGL::PointLight;
using miniGL::SpotLight;
using miniGL::Log;

SpotLight::SpotLight(void)
:PointLight(),
 mDirection(vec3f(0.0f)),
 mCutoff(20.0f)
{
    // The spot light needs to overrite the type as the point light constructor will set its type to point
    type(BaseLight::EType::SPOT);
    name("spot light");
}

SpotLight::SpotLight(const vec3f & pRGB, const vec3f & pPos, const vec3f & pDir, float pAmbInt, float pDiffInt, float pConstAtt, float pLinAtt, float pExpAtt, float pCutoff)
:PointLight(pRGB, pPos, pAmbInt, pDiffInt, pConstAtt,pLinAtt, pExpAtt),
 mDirection(pDir),
 mCutoff(pCutoff)
{
    type(BaseLight::EType::SPOT);
    name("spot light");
}

SpotLight::~SpotLight()
{
    string lMessage("Destructor ");
    lMessage.append(__FUNCTION__);
    Log::consoleMessage(lMessage);

    lMessage.assign("Deleting ");
    lMessage.append(name());
    Log::consoleMessage(lMessage);
}

void SpotLight::direction(const vec3f & pDir)
{
    mDirection = pDir;
}

void SpotLight::cutoff(degreef pAngle)
{
    //mCutoff = std::min(90.0f, std::max(0.0f, pAngle));
    if (pAngle < 0.0f)
        mCutoff = 0.0f;
    else if (90.0f < pAngle)
        mCutoff = 90.0f;
    else
        mCutoff = pAngle;
}

const vec3f & SpotLight::direction(void) const
{
    return mDirection;
}

degreef SpotLight::cutoff(void) const
{
    return mCutoff;
}

vec3f & SpotLight::direction(void)
{
    return mDirection;
}

degreef & SpotLight::cutoff(void)
{
    return mCutoff;
}

void SpotLight::resetImplementation(void)
{
    PointLight::resetImplementation();
    mDirection.x() = 0.0f;
    mDirection.y() = 0.0f;
    mDirection.z() = 0.0f;
    mCutoff = 0.0f;
}
