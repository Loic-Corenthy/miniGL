//===============================================================================================//
/*!
 *  \file      Camera.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#include "Camera.hpp"

#include <cmath>
#include <iostream>

#include "EnumClassCast.hpp"
#include "Exceptions.hpp"

using std::array;
using miniGL::Camera;
using miniGL::Exceptions;

Camera::Camera(void)
:mView(1.0)
,mProjection(1.0)
,mMouseRotation(1.0)
,mPosition(0.0f)
,mLookAt({0.0f, 0.0f, 1.0f})
,mUp({0.0f, 1.0f, 0.0f})
,mVerticalFoV(0.0f)
,mAspectRatio(0.0f)
,mNearPlane(0.0f)
,mFarPlane(0.0f)
,mFocalDist(0.0f)
,mFrameBufferWidth(0)
,mFrameBufferHeight(0)
,mMousePos(0)
,mMouseRotationSensibility(2.0f)
//,mMouseLeftButtonPressed(false)
,mViewHasChanged(true)
,mProjectionHasChanged(true)
{
    vec3f lHorizontalTarget({mLookAt.x(), 0.0f, mLookAt.z()});
    lHorizontalTarget.normalize();

    if (lHorizontalTarget.z() >= 0.0f)
    {
        const radianf lArcsinHorizontalTarget = asin(lHorizontalTarget.z());

        if (lHorizontalTarget.x() >= 0.0f)
            mAngleH = 360.0f - lArcsinHorizontalTarget.toDegree();
        else
            mAngleH = 180.0f + lArcsinHorizontalTarget.toDegree();
    }
    else
    {
        const radianf lArcsinMinusHorizontalTarget = asin(-lHorizontalTarget.z());

        if (lHorizontalTarget.x() >= 0.0f)
            mAngleH = lArcsinMinusHorizontalTarget.toDegree();
        else
            mAngleH = 90.0f + lArcsinMinusHorizontalTarget.toDegree();
    }

    mAngleV = radianf(asin(mLookAt.y())).toDegree();
    mAngleV = mAngleV * -1.0f;
}

Camera::~Camera(void)
{
}

Camera::Camera(const Camera & pCamera)
{
    mOrthogonalProjection = pCamera.mOrthogonalProjection;
    mOrthogonalProjectionDirections = pCamera.mOrthogonalProjectionDirections;
    mOrthogonalProjectionHasChanged = pCamera.mOrthogonalProjectionHasChanged;

    mView = pCamera.mView;
    mProjection = pCamera.mProjection;

    mMouseRotation = pCamera.mMouseRotation;

    mPosition = pCamera.mPosition;
    mLookAt = pCamera.mLookAt;
    mUp = pCamera.mUp;

    mVerticalFoV = pCamera.mVerticalFoV;
    mAspectRatio = pCamera.mAspectRatio;
    mNearPlane = pCamera.mNearPlane;
    mFarPlane = pCamera.mFarPlane;
    mFocalDist = pCamera.mFocalDist;

    mFrameBufferWidth = pCamera.mFrameBufferWidth;
    mFrameBufferHeight = pCamera.mFrameBufferHeight;
    mWindowWidth = pCamera.mWindowWidth;
    mWindowHeight = pCamera.mWindowHeight;

    mMousePos = pCamera.mMousePos;
    mMouseRotationSensibility = pCamera.mMouseRotationSensibility;

    mAngleH = pCamera.mAngleH;
    mAngleV = pCamera.mAngleV;

    mViewHasChanged = pCamera.mViewHasChanged;
    mProjectionHasChanged = pCamera.mProjectionHasChanged;
}

Camera Camera::operator=(const Camera & pCamera)
{
    if (this != & pCamera)
    {
        mOrthogonalProjection = pCamera.mOrthogonalProjection;
        mOrthogonalProjectionDirections = pCamera.mOrthogonalProjectionDirections;
        mOrthogonalProjectionHasChanged = pCamera.mOrthogonalProjectionHasChanged;

        mView = pCamera.mView;
        mProjection = pCamera.mProjection;

        mMouseRotation = pCamera.mMouseRotation;

        mPosition = pCamera.mPosition;
        mLookAt = pCamera.mLookAt;
        mUp = pCamera.mUp;

        mVerticalFoV = pCamera.mVerticalFoV;
        mAspectRatio = pCamera.mAspectRatio;
        mNearPlane = pCamera.mNearPlane;
        mFarPlane = pCamera.mFarPlane;
        mFocalDist = pCamera.mFocalDist;

        mFrameBufferWidth = pCamera.mFrameBufferWidth;
        mFrameBufferHeight = pCamera.mFrameBufferHeight;
        mWindowWidth = pCamera.mWindowWidth;
        mWindowHeight = pCamera.mWindowHeight;

        mMousePos = pCamera.mMousePos;
        mMouseRotationSensibility = pCamera.mMouseRotationSensibility;

        mAngleH = pCamera.mAngleH;
        mAngleV = pCamera.mAngleV;

        mViewHasChanged = pCamera.mViewHasChanged;
        mProjectionHasChanged = pCamera.mProjectionHasChanged;
    }

    return *this;
}

float Camera::aspectRatio(void) const noexcept
{
    return mAspectRatio;
}

radianf Camera::verticalFoV(void) const noexcept
{
    return mVerticalFoV;
}

float Camera::nearPlane(void) const noexcept
{
    return mNearPlane;
}

float Camera::farPlane(void) const noexcept
{
    return mFarPlane;
}

float Camera::focalDist(void) const noexcept
{
    return mFocalDist;
}

const vec3f & Camera::position(void) const noexcept
{
    return mPosition;
}

const vec3f & Camera::lookAt(void) const noexcept
{
    return mLookAt;
}

const vec3f & Camera::up(void) const noexcept
{
    return mUp;
}

const mat4f & Camera::view(void)
{
    if (mViewHasChanged)
    {
        _updateView();
        mViewHasChanged = false;
    }

    return mView;
}

const mat4f & Camera::projection(void)
{
    if (mProjectionHasChanged)
    {
        _updateProjection();
        mProjectionHasChanged = false;
    }

    return mProjection;
}

mat4f Camera::orthogonalProjection(size_t pIndex)
{
    assert(pIndex < mOrthogonalProjection.size());

    if (mOrthogonalProjectionHasChanged[pIndex])
    {
        _updateOrthogonalProjection(pIndex);
        mOrthogonalProjectionHasChanged[pIndex] = false;
    }

    return mOrthogonalProjection[pIndex];
}

void Camera::verticalFoV(radianf pValue) noexcept
{
    mProjectionHasChanged = true;
    mVerticalFoV = pValue;
}

void Camera::nearPlane(float pValue) noexcept
{
    mProjectionHasChanged = true;
    mNearPlane = pValue;
}

void Camera::farPlane(float pValue) noexcept
{
    mProjectionHasChanged = true;
    mFarPlane = pValue;
}

void Camera::focalDist(float pValue) noexcept
{
    mFocalDist = pValue;
}

void Camera::position(const vec3f & pPosition) noexcept
{
    mViewHasChanged = true;
    mPosition = pPosition;
}

void Camera::lookAt(const vec3f & pLookat)
{
    mViewHasChanged = true;
    mLookAt = pLookat.normalized();
}

void Camera::up(const vec3f & pUp)
{
    mViewHasChanged = true;
    mUp = pUp.normalized();
}

void Camera::frameBufferDimensions(unsigned int pWidth, unsigned int pHeight) noexcept
{
    mFrameBufferWidth = pWidth;
    mFrameBufferHeight = pHeight;

    // Also update the aspect ratio
    mAspectRatio = static_cast<float>(pWidth) / static_cast<float>(pHeight);
    mProjectionHasChanged = true;
}

void Camera::windowDimensions(unsigned int pWidth, unsigned int pHeight) noexcept
{
    mWindowWidth = pWidth;
    mWindowHeight = pHeight;

    mMousePos[0] = mWindowWidth / 2;
    mMousePos[1] = mWindowHeight / 2;
}

void Camera::rotateFromMouse(int pX, int pY)
{
    const int lDeltaX = pX - mMousePos[0];
    const int lDeltaY = pY - mMousePos[1];

    mMousePos[0] = pX;
    mMousePos[1] = pY;

    mAngleH = mAngleH + static_cast<float>(lDeltaX) / 20.0f;
    mAngleV = mAngleV + static_cast<float>(lDeltaY) / 20.0f;

    _updateOrientation();
}

void Camera::rotateFromMouseSensibility(float pValue)
{
    assert(1.0f <= pValue && pValue <= 10.0f && "Mouse sensibility must be between 1 and 10");
    mMouseRotationSensibility = pValue;
}

void Camera::onRender(void)
{
    bool lUpdateOrientation = false;

    if (0 <= mMousePos[0] && mMousePos[0] <= mMargin)
    {
        mAngleH = mAngleH - mEdgeStep;
        lUpdateOrientation = true;
    }
    else if((mWindowWidth - mMargin) <= mMousePos[0] && mMousePos[0] <= mWindowWidth)
    {
        mAngleH = mAngleH + mEdgeStep;
        lUpdateOrientation = true;
    }

    if (0 <= mMousePos[1] && mMousePos[1] <= mMargin)
    {
        if (mAngleV  > -90.0f)
        {
            mAngleV = mAngleV - mEdgeStep;
            lUpdateOrientation = true;
        }
    }
    else if((mWindowHeight - mMargin) <= mMousePos[1] && mMousePos[1] <= mWindowHeight)
    {
        if (mAngleV  < 90.0f)
        {
            mAngleV = mAngleV + mEdgeStep;
            lUpdateOrientation = true;
        }
    }

    if (lUpdateOrientation)
        _updateOrientation();
}

void Camera::orthogonalProjectionParameterCount(size_t pValue)
{
    mat4f lIdentity(1.0f);
    mOrthogonalProjection.assign(pValue, lIdentity);

    array<float, 6> lProjectionDirection = {{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}};
    mOrthogonalProjectionDirections.assign(pValue, lProjectionDirection);

    mOrthogonalProjectionHasChanged.assign(pValue, false);
}

void Camera::orthogonalProjectionParameter(size_t pIndex, EOrthogonalProjection pSide, float pValue)
{
    assert(pIndex < mOrthogonalProjectionDirections.size());

    mOrthogonalProjectionDirections[pIndex][toUT(pSide)] = pValue;
    mOrthogonalProjectionHasChanged[pIndex] = true;
}

void Camera::_updateView(void)
{
    mat4f lTranslation(1.0f);

    lTranslation(0,3) = -mPosition[0];
    lTranslation(1,3) = -mPosition[1];
    lTranslation(2,3) = -mPosition[2];

    mat4f lCoordsTransform(1.0f);

    vec3f N = mLookAt;
    vec3f U = mUp.cross(N);
    vec3f V = N.cross(U);

    lCoordsTransform(0,0) = U.x();   lCoordsTransform(0,1) = U.y();   lCoordsTransform(0,2) = U.z();   lCoordsTransform(0,3) = 0.0f;
    lCoordsTransform(1,0) = V.x();   lCoordsTransform(1,1) = V.y();   lCoordsTransform(1,2) = V.z();   lCoordsTransform(1,3) = 0.0f;
    lCoordsTransform(2,0) = N.x();   lCoordsTransform(2,1) = N.y();   lCoordsTransform(2,2) = N.z();   lCoordsTransform(2,3) = 0.0f;
    lCoordsTransform(3,0) = 0.0f;    lCoordsTransform(3,1) = 0.0f;    lCoordsTransform(3,2) = 0.0f;    lCoordsTransform(3,3) = 1.0f;

    mView = lCoordsTransform * lTranslation;
}

void Camera::_updateProjection(void)
{
    float lTanHalfFOV = tanf(mVerticalFoV * 0.5f);
    float lOneOverRange = 1.0f / (mNearPlane - mFarPlane);

    mProjection(0,0) = 1.0f / (lTanHalfFOV * mAspectRatio);
    mProjection(1,0) = 0.0f;
    mProjection(2,0) = 0.0f;
    mProjection(3,0) = 0.0f;

    mProjection(0,1) = 0.0f;
    mProjection(1,1) = 1.0f / lTanHalfFOV;
    mProjection(2,1) = 0.0f;
    mProjection(3,1) = 0.0f;

    mProjection(0,2) = 0.0f;
    mProjection(1,2) = 0.0f;
    mProjection(2,2) = (-mNearPlane - mFarPlane) * lOneOverRange;
    mProjection(3,2) = 1.0;

    mProjection(0,3) = 0.0f;
    mProjection(1,3) = 0.0f;
    mProjection(2,3) = 2.0f * mFarPlane * mNearPlane * lOneOverRange;
    mProjection(3,3) = 0.0f;
}

void Camera::_updateOrthogonalProjection(size_t pIndex)
{
    const float lRight = mOrthogonalProjectionDirections[pIndex][toUT(EOrthogonalProjection::RIGHT)];
    const float lLeft = mOrthogonalProjectionDirections[pIndex][toUT(EOrthogonalProjection::LEFT)];
    const float lTop = mOrthogonalProjectionDirections[pIndex][toUT(EOrthogonalProjection::TOP)];
    const float lBottom = mOrthogonalProjectionDirections[pIndex][toUT(EOrthogonalProjection::BOTTOM)];
    const float lNear = mOrthogonalProjectionDirections[pIndex][toUT(EOrthogonalProjection::NEAR)];
    const float lFar = mOrthogonalProjectionDirections[pIndex][toUT(EOrthogonalProjection::FAR)];

    if (lRight < lLeft || lTop < lBottom || lFar < lNear)
        throw Exceptions("Error in orthogonal projection parameters", __FILE__, __LINE__);

    mOrthogonalProjection[pIndex](0,0) = 2.0f / (lRight - lLeft); mOrthogonalProjection[pIndex](0,1) = 0.0f;                    mOrthogonalProjection[pIndex](0,2) = 0.0f;                  mOrthogonalProjection[pIndex](0,3) = -(lRight + lLeft) / (lRight - lLeft);
    mOrthogonalProjection[pIndex](1,0) = 0.0f;                    mOrthogonalProjection[pIndex](1,1) = 2.0f / (lTop - lBottom); mOrthogonalProjection[pIndex](1,2) = 0.0f;                  mOrthogonalProjection[pIndex](1,3) = -(lTop + lBottom) / (lTop - lBottom);
    mOrthogonalProjection[pIndex](2,0) = 0.0f;                    mOrthogonalProjection[pIndex](2,1) = 0.0f;                    mOrthogonalProjection[pIndex](2,2) = 2.0f / (lFar - lNear); mOrthogonalProjection[pIndex](2,3) = -(lFar + lNear) / (lFar - lNear);
    mOrthogonalProjection[pIndex](3,0) = 0.0f;                    mOrthogonalProjection[pIndex](3,1) = 0.0f;                    mOrthogonalProjection[pIndex](3,2) = 0.0;                   mOrthogonalProjection[pIndex](3,3) = 1.0f;
}

void Camera::_updateOrientation(void)
{
    const vec3f lVerticalAxis(0.0f, 1.0f, 0.0f);

    // Rotate the view vector by the horizontal angle around the vertical axis
    vec3f lView(1.0f, 0.0f, 0.0f);

    float lSinHalfAngle = sinf(mAngleH.toRadian() * 0.5f);
    float lCosHalfAngle = cosf(mAngleH.toRadian() * 0.5f);

    quatf lRotation(lVerticalAxis.x() * lSinHalfAngle, lVerticalAxis.y() * lSinHalfAngle, lVerticalAxis.z() * lSinHalfAngle, lCosHalfAngle);
    quatf lConjugate = lRotation.conjugated();
    quatf lRotated = lRotation * lView * lConjugate;

    lView.x() = lRotated.x();
    lView.y() = lRotated.y();
    lView.z() = lRotated.z();

    lView.normalize();

    // Rotate the view vector by the vertical angle around the horizontal axis
    vec3f lHorizontalAxis = lVerticalAxis.cross(lView);
    lHorizontalAxis.normalize();

    lSinHalfAngle = sinf(mAngleV.toRadian() * 0.5f);
    lCosHalfAngle = cosf(mAngleV.toRadian() * 0.5f);

    quatf lRotationV(lHorizontalAxis.x() * lSinHalfAngle, lHorizontalAxis.y() * lSinHalfAngle, lHorizontalAxis.z() * lSinHalfAngle, lCosHalfAngle);
    quatf lConjugateV = lRotationV.conjugated();
    quatf lRotatedV = lRotationV * lView * lConjugateV;

    lView.x() = lRotatedV.x();
    lView.y() = lRotatedV.y();
    lView.z() = lRotatedV.z();

    mLookAt = lView;
    mLookAt.normalize();

    mUp = mLookAt.cross(lHorizontalAxis);
    mUp.normalize();

    mViewHasChanged = true;
}
