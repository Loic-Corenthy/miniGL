//===============================================================================================//
/*!
 *  \file      BackendGLFW.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

#include "BaseBackend.hpp"

#include <GLFW/glfw3.h>

namespace miniGL
{
    /*!
     *  \brief Use GLFW as window backend
     *  \details See http://www.glfw.org/
     */
    class BackendGLFW : public BaseBackend
    {
    public:
        /*!
         *  \brief Constructor with callbacks parameter
         *  @param pCallbacks is a pointer on the callbacks used by the application associated to this window
         */
        BackendGLFW(CallbacksInterface* pCallbacks);

        /*!
         *  \brief Copy contructor, not accessible
         *  @param pWindow is the object to copy parameters from
         */
        BackendGLFW(const BackendGLFW & pBackendGLFW) = delete;

        /*!
         *  \brief Copy assignment operator, not accessible
         *  @param pWindow is the object to copy parameters from
         *  @return a reference on this
         */
        BackendGLFW & operator=(const BackendGLFW & pBackendGLFW) = delete;

        /*!
         *  \brief Destructor
         */
        ~BackendGLFW(void);

        /*!
         * \brief Set the position of the mouse cursor
         * @param pX is the x coordinate of the cursor in window space
         * @param pY is the y coordinate of the cursor in window space
         */
        void cursorPosition(int pX, int pY);

        /*!
         *  \brief Init opengl context and all other necessary things to create a window where we'll be able to render the images
         *  @param pWidth is the width of the window that will be created
         *  @param pHeight is the height of the window that will be created
         *  @param pTitle is the title displayed at the top of the window
         */
        virtual void init(int pWidth, int pHeight,  bool pFullScreen, const std::string & pTitle) final;

        /**
         * \brief Render image and swap buffers
         */
        virtual void run(void) final;

        /*!
         * \brief Implementation of a virtual method from BaseBackend
         */
        virtual void swapBuffer(void) final;

        /*!
         * \brief Implementation of a virtual method from BaseBackend
         */
        virtual void terminate(void) final;

    private:
        static void _errorCallback(int pError, const char* pDescription);
        static void _keyCallback(GLFWwindow* pWindow, int pKey, int pScancode, int pAction, int pMods);
        static void _cursorPosCallback(GLFWwindow* pWindow, double pX, double pY);
        static void _frameBufferSizeCallback(GLFWwindow* pWindow, int pWidth, int pHeight);
        static void _mouseButtonCallback(GLFWwindow* pWindow, int pButton, int pAction, int pMods);

    private:
        GLFWwindow* mWindow = nullptr;

        static double mMousePosX;
        static double mMousePosY;
        static int mMouseButton;
        static bool mMouseButtonPressed;

    }; // class BackendGLFW

} // namespace miniGL
