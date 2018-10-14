//===============================================================================================//
/*!
 *  \file      Transform.cpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#include "Transform.hpp"

#include <cmath>

using miniGL::Transform;

mat4f Transform::mIdentity = mat4f(1.0);

Transform::Transform(void)
:mScaling(mIdentity)
,mRotation(mIdentity)
,mTranslation(mIdentity)
,mFinal(mIdentity)
{
}

void Transform::scaling(float pFactorX, float pFactorY, float pFactorZ)
{
    mScaling(0,0) = pFactorX;
    mScaling(1,1) = pFactorY;
    mScaling(2,2) = pFactorZ;

    mUpdated = true;
}

void Transform::rotation(degreef pAngleX, degreef pAngleY, degreef pAngleZ)
{
    mat4f lRotX, lRotY, lRotZ;

    const float lX = pAngleX.toRadian();
    const float lY = pAngleY.toRadian();
    const float lZ = pAngleZ.toRadian();

    lRotX(0,0) = 1.0f;         lRotX(0,1) = 0.0f;         lRotX(0,2) = 0.0f;         lRotX(0,3) = 0.0f;
    lRotX(1,0) = 0.0f;         lRotX(1,1) = cosf(lX);     lRotX(1,2) = -sinf(lX);    lRotX(1,3) = 0.0f;
    lRotX(2,0) = 0.0f;         lRotX(2,1) = sinf(lX);     lRotX(2,2) = cosf(lX);     lRotX(2,3) = 0.0f;
    lRotX(3,0) = 0.0f;         lRotX(3,1) = 0.0f;         lRotX(3,2) = 0.0f;         lRotX(3,3) = 1.0f;

    lRotY(0,0) = cosf(lY);     lRotY(0,1) = 0.0f;         lRotY(0,2) = -sinf(lY);    lRotY(0,3) = 0.0f;
    lRotY(1,0) = 0.0f;         lRotY(1,1) = 1.0f;         lRotY(1,2) = 0.0f;         lRotY(1,3) = 0.0f;
    lRotY(2,0) = sinf(lY);     lRotY(2,1) = 0.0f;         lRotY(2,2) = cosf(lY);     lRotY(2,3) = 0.0f;
    lRotY(3,0) = 0.0f;         lRotY(3,1) = 0.0f;         lRotY(3,2) = 0.0f;         lRotY(3,3) = 1.0f;

    lRotZ(0,0) = cosf(lZ);     lRotZ(0,1) = -sinf(lZ);    lRotZ(0,2) = 0.0f;         lRotZ(0,3) = 0.0f;
    lRotZ(1,0) = sinf(lZ);     lRotZ(1,1) = cosf(lZ);     lRotZ(1,2) = 0.0f;         lRotZ(1,3) = 0.0f;
    lRotZ(2,0) = 0.0f;         lRotZ(2,1) = 0.0f;         lRotZ(2,2) = 1.0f;         lRotZ(2,3) = 0.0f;
    lRotZ(3,0) = 0.0f;         lRotZ(3,1) = 0.0f;         lRotZ(3,2) = 0.0f;         lRotZ(3,3) = 1.0f;

    mRotation = lRotZ * lRotY * lRotX;

    mUpdated = true;
}

void Transform::translation(float pX, float pY, float pZ)
{
    mTranslation(0,3) = pX;
    mTranslation(1,3) = pY;
    mTranslation(2,3) = pZ;

    mUpdated = true;
}

void Transform::scaling(const mat4f & pScaleMatrix) noexcept
{
    mScaling = pScaleMatrix;

    mUpdated = true;
}

void Transform::rotation(const mat4f & pRotationMatrix) noexcept
{
    mRotation = pRotationMatrix;

    mUpdated = true;
}

void Transform::translation(const mat4f & pTranslationMatrix) noexcept
{
    mTranslation = pTranslationMatrix;

    mUpdated = true;
}

mat4f Transform::scaling(void) const noexcept
{
    return mScaling;
}

mat4f Transform::rotation(void) const noexcept
{
    return mRotation;
}

mat4f Transform::translation(void) const noexcept
{
    return mTranslation;
}

mat4f Transform::final(void) const noexcept
{
    if (mUpdated)
    {
        mFinal = mTranslation * mRotation * mScaling;
        mUpdated = false;
    }

    return mFinal;
}
