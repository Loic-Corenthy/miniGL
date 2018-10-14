//===============================================================================================//
/*!
 *  \file      BaseBackend.cpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#include "BaseBackend.hpp"

#include <cassert>

using std::tuple;
using std::make_tuple;
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using miniGL::CallbacksInterface;
using miniGL::BaseBackend;

CallbacksInterface* BaseBackend::mCallbacks = nullptr;

BaseBackend::BaseBackend(CallbacksInterface* pCallbacks)
{
    // Keep a pointer on the callbacks from the application that will be used during the rendering
    assert(pCallbacks && "Pointer to callbacks is not valid");
    mCallbacks = pCallbacks;

    mStartTime = high_resolution_clock::now();
    mFrameTime = mStartTime;
}

BaseBackend::~BaseBackend(void)
{
    // Do NOT delete the pointer to mCallbacks
}



void BaseBackend::calculateFPS(void)
{
    mFrameCount++;

    std::chrono::high_resolution_clock::time_point lTime = std::chrono::high_resolution_clock::now();

    if (std::chrono::duration_cast<std::chrono::seconds>(lTime - mFrameTime ).count() >= 1)
    {
        mFrameTime = lTime;
        mFPS = mFrameCount;
        mFrameCount = 0;
    }
}

unsigned int BaseBackend::FPS(void) const
{
    return mFPS;
}


float BaseBackend::runningTime(void) const
{
    return static_cast<float>(duration_cast<std::chrono::milliseconds>(high_resolution_clock::now() - mStartTime ).count() * 0.001);
}

tuple<int, int> BaseBackend::frameBufferDimensions(void) const
{
    return make_tuple(mFrameBufferWidth, mFrameBufferHeight);
}

void BaseBackend::frameBufferDimensions(int pWidth, int pHeight)
{
    mFrameBufferWidth = pWidth;
    mFrameBufferHeight = pHeight;
}

tuple<int, int> BaseBackend::windowDimensions(void) const
{
    return make_tuple(mWidth, mHeight);
}

tuple<int, int, int, int> BaseBackend::pixelColor(int pX, int pY)
{
    const float ratioX = static_cast<float>(mFrameBufferWidth) / static_cast<float>(mWidth);
    const float ratioY = static_cast<float>(mFrameBufferHeight) / static_cast<float>(mHeight);

    int lX = static_cast<int>(static_cast<float>(pX) * ratioX);
    int lY = static_cast<int>(static_cast<float>(pX) * ratioY);

    unsigned char pixel[4];
    glReadPixels(lX, mFrameBufferHeight - lY, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);

    return make_tuple(static_cast<int>(pixel[0]), static_cast<int>(pixel[1]), static_cast<int>(pixel[2]), static_cast<int>(pixel[3]));
}

void BaseBackend::defaultConfiguration(void) const
{
    // Default global configuration
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
}
