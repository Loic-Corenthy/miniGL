//===============================================================================================//
/*!
 *  \file      BackendGLFW.cpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#include "BackendGLFW.hpp"

#include <string>

#include "Exceptions.hpp"
#include "GLUtils.hpp"
#include "CallbacksInterface.hpp"

using std::string;
using std::to_string;
using miniGL::BackendGLFW;
using miniGL::Exceptions;
using miniGL::CallbacksInterface;

double BackendGLFW::mMousePosX = 0.0;
double BackendGLFW::mMousePosY = 0.0;
int BackendGLFW::mMouseButton = 0;
bool BackendGLFW::mMouseButtonPressed = 0;

BackendGLFW::BackendGLFW(CallbacksInterface* pCallbacks)
:BaseBackend(pCallbacks)
{
}

BackendGLFW::~BackendGLFW(void)
{
    glfwDestroyWindow(mWindow);
    glfwTerminate();
}

void BackendGLFW::init(int pWidth, int pHeight,  bool pFullScreen, const std::string & pTitle)
{
    // Save parameters
    mWidth = pWidth;
    mHeight = pHeight;
    mTitle = pTitle;

    // Save GLFW version in log
    string lLogMessage("Starting GLFW version: ");
    lLogMessage.append(glfwGetVersionString());
    mCallbacks->messageCallback(lLogMessage);

    // Init GLFW
    if (!glfwInit())
    {
        mCallbacks->errorCallback(-1, "Cannot init GLFW");
        throw Exceptions("Cannot init GLFW", __FILE__, __LINE__);
    }

    // Set openGL version and require core profile and forward compatibility
    // REQUIRED on Mac OS X
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Define sampling quality (use 16 for better quality)
    glfwWindowHint(GLFW_SAMPLES, 4);

    //    GLFWmonitor* mon = glfwGetPrimaryMonitor();
    //    const GLFWvidmode* vmode = glfwGetVideoMode (mon);
    //    GLFWwindow* window = glfwCreateWindow (
    //                                           vmode->width, vmode->height, "Extended GL Init", mon, NULL
    //                                           );
    //

    // register the error callback method
    glfwSetErrorCallback(_errorCallback);

    GLFWmonitor* rMonitor = pFullScreen? glfwGetPrimaryMonitor() : nullptr;

    // Create a window
    mWindow = glfwCreateWindow(mWidth, mHeight, pTitle.c_str(), rMonitor, nullptr);

    // Check if the window was created correctly
    if(mWindow == nullptr)
    {
        glfwTerminate();
        mCallbacks->errorCallback(-1, "Cannot create window");
        throw Exceptions("Cannot create window", __FILE__, __LINE__);
    }

    // Update the width and height of the window. If the dimensions passed as parameters are bigger than the size of the screen, the size of the window is updated accordingly
    glfwGetWindowSize(mWindow, & mWidth, &mHeight);

    // Update the actual values set for the frame buffer
    glfwGetFramebufferSize(mWindow, &mFrameBufferWidth, &mFrameBufferHeight);

    // Must make the context current before initialising glew
    glfwMakeContextCurrent(mWindow);

    // Must define glewExperimental to true, otherwise, openGL function calls won't work
    glewExperimental = GL_TRUE; checkOpenGLState;

    GLenum lResult = glewInit();
    if (lResult != GLEW_OK)
    {
        string lErrorMessage = reinterpret_cast<const char*>(glewGetErrorString(lResult));
        _errorCallback(static_cast<int>(lResult), lErrorMessage.c_str());
        throw Exceptions(lErrorMessage, __FILE__, __LINE__);
    }

    checkOpenGLState; // Check glewInit

    // Get renderer info
    const GLubyte* lRenderer = glGetString(GL_RENDERER);
    lLogMessage.assign("Renderer: ");
    lLogMessage.append(reinterpret_cast<const char*>(lRenderer));
    mCallbacks->messageCallback(lLogMessage);

    checkOpenGLState;

    // Get GLSL version info
    const GLubyte* lGLSLVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
    lLogMessage.assign("GLSL version: ");
    lLogMessage.append(reinterpret_cast<const char*>(lGLSLVersion));
    mCallbacks->messageCallback(lLogMessage);

    checkOpenGLState;

    // Get OpenGL version info
    const GLubyte* lVersion = glGetString(GL_VERSION);
    lLogMessage.assign("OpenGL version: ");
    lLogMessage.append(reinterpret_cast<const char*>(lVersion));
    mCallbacks->messageCallback(lLogMessage);

    checkOpenGLState;

    GLUtils::currentState();

    // Set clear color
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // Call method from base class
    BaseBackend::defaultConfiguration();
    checkOpenGLState;

    GLUtils::currentState();

    // Wait 1 frame between each swap ?
    glfwSwapInterval(1);

    // Callback method for keyboard events
    glfwSetKeyCallback(mWindow, _keyCallback); checkOpenGLState;

    // Callback for mouse events
    glfwSetCursorPosCallback(mWindow, _cursorPosCallback); checkOpenGLState;
    glfwSetMouseButtonCallback(mWindow, _mouseButtonCallback); checkOpenGLState;

    // Callback for resize events
    glfwSetFramebufferSizeCallback(mWindow, _frameBufferSizeCallback); checkOpenGLState;
}

void BackendGLFW::run(void)
{
    while (!glfwWindowShouldClose(mWindow))
    {
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        mCallbacks->pickingPhaseCallback();
        mCallbacks->renderPhaseCallBack();

        swapBuffer();

        calculateFPS();
        string lTitle = mTitle;
        lTitle.append(" running @ ");
        lTitle.append(to_string(mFPS));
        lTitle.append(" fps.");
        glfwSetWindowTitle(mWindow, lTitle.c_str());
    }
}


void BackendGLFW::swapBuffer(void)
{
    glfwSwapBuffers(mWindow);
}

void BackendGLFW::terminate(void)
{

}

void BackendGLFW::cursorPosition(int pX, int pY)
{
    mMousePosX = static_cast<double>(pX);
    mMousePosY = static_cast<double>(pY);
    glfwSetCursorPos(mWindow, mMousePosX, mMousePosY);
}

void BackendGLFW::_errorCallback(int pError, const char* pDescription)
{
    mCallbacks->errorCallback(pError, pDescription);
}

void BackendGLFW::_keyCallback(GLFWwindow* pWindow, int pKey, int pScancode, int pAction, int pMods)
{
    if (pKey == GLFW_KEY_ESCAPE && pAction == GLFW_PRESS)
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
    else
        mCallbacks->keyboardCallBack(pKey, pScancode, pAction, pMods);
}

void BackendGLFW::_cursorPosCallback(GLFWwindow* pWindow, double pX, double pY)
{
    mMousePosX = pX;
    mMousePosY = pY;
    mCallbacks->passiveMouseCallBack(static_cast<int>(mMousePosX), static_cast<int>(mMousePosY));
}

void BackendGLFW::_frameBufferSizeCallback(GLFWwindow *pWindow, int pWidth, int pHeight)
{
    // Update dimensions of the frame buffer
    mCallbacks->frameBufferSizeCallback(pWidth, pHeight);
    glViewport(0, 0, pWidth, pHeight);
}

void BackendGLFW::_mouseButtonCallback(GLFWwindow* pWindow, int pButton, int pAction, int pMods)
{
    mMouseButton = pButton;
    mMouseButtonPressed = (pAction == GLFW_PRESS);
    mCallbacks->mouseCallBack(mMouseButton, mMouseButtonPressed);
}
