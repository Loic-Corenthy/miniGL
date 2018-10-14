//===============================================================================================//
/*!
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#include "DeferredShadingLightPass.hpp"

#include "Constants.hpp"
#include "Exceptions.hpp"

using miniGL::DeferredShadingLightPass;
using miniGL::Constants;
using miniGL::Exceptions;

DeferredShadingLightPass::DeferredShadingLightPass(void)
:Program()
{
    mWVPLocation = Constants::invalidUniformLocation<GLuint>();
    mPositionTextureUnitLocation = Constants::invalidUniformLocation<GLuint>();
    mNormalTextureUnitLocation = Constants::invalidUniformLocation<GLuint>();
    mColorTextureUnitLocation = Constants::invalidUniformLocation<GLuint>();
    mEyeWorldPosLocation = Constants::invalidUniformLocation<GLuint>();
    mMaterialSpecularIntensityLocation = Constants::invalidUniformLocation<GLuint>();
    mMaterialSpecularPowerLocation = Constants::invalidUniformLocation<GLuint>();
    mScreenSizeLocation = Constants::invalidUniformLocation<GLuint>();
}

DeferredShadingLightPass::~DeferredShadingLightPass(void)
{
}

void DeferredShadingLightPass::init(void)
{
    mWVPLocation = Program::uniformLocation("uWVP");
    mPositionTextureUnitLocation = Program::uniformLocation("uPositionMap");
    mNormalTextureUnitLocation = Program::uniformLocation("uNormalMap");
    mColorTextureUnitLocation = Program::uniformLocation("uColorMap");
    mEyeWorldPosLocation = Program::uniformLocation("uEyeWorldPos");
    mMaterialSpecularIntensityLocation = Program::uniformLocation("uMaterialSpecularIntensity");
    mMaterialSpecularPowerLocation = Program::uniformLocation("uMaterialSpecularPower");
    mScreenSizeLocation = Program::uniformLocation("uScreenSize");

    // Check if we correctly initialized the uniform variables
    if (!DeferredShadingLightPass::checkUniformLocations())
        throw Exceptions("Not all uniform locations were updated", __FILE__, __LINE__);
}

bool DeferredShadingLightPass::checkUniformLocations(void) const
{
    return (mWVPLocation != Constants::invalidUniformLocation<GLuint>()                             &&
            mPositionTextureUnitLocation != Constants::invalidUniformLocation<GLuint>()            &&
            mNormalTextureUnitLocation != Constants::invalidUniformLocation<GLuint>()            &&
            mColorTextureUnitLocation != Constants::invalidUniformLocation<GLuint>()            &&
            mEyeWorldPosLocation != Constants::invalidUniformLocation<GLuint>()                     &&
            mMaterialSpecularIntensityLocation != Constants::invalidUniformLocation<GLuint>()    &&
            mMaterialSpecularPowerLocation != Constants::invalidUniformLocation<GLuint>()         &&
            mScreenSizeLocation != Constants::invalidUniformLocation<GLuint>());
}

