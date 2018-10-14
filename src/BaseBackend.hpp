//===============================================================================================//
/*!
 *  \file      BaseBackend.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

#include <iostream>
#include <string>
#include <tuple>
#include <chrono>

#include <GL/glew.h>

#include "CallbacksInterface.hpp"

namespace miniGL
{
    /*!
     *  \brief Base class for the windowing system.
     *  \details This base class defines the interface for the different backends that will implement
     *           the communication between opengl and the windowing system
     */
    class BaseBackend
    {
    public:
        /*!
         *  \brief Copy contructor, not accessible
         *  @param pWindow is the object to copy parameters from
         */
        BaseBackend(const BaseBackend & pBaseBackend) = delete;

        /*!
         *  \brief Copy assignment operator, not accessible
         *  @param pWindow is the object to copy parameters from
         *  @return a reference on this
         */
        BaseBackend & operator=(const BaseBackend & pBaseBackend) = delete;

        /*!
         *  \brief Destructor
         */
        virtual ~BaseBackend(void);

        /*!
         *  \brief Init opengl context and all other necessary things to create a window where we'll be able to render the images
         *  @param pWidth is the width of the window that will be created
         *  @param pHeight is the height of the window that will be created
         *  @param pTitle is the title displayed at the top of the window
         */
        virtual void init(int pWidth, int pHeight,  bool pFullScreen, const std::string & pTitle) = 0;

        /*!
         * \brief Render images
         */
        virtual void run(void) = 0;

        /*!
         * \brief Swap buffers
         */
        virtual void swapBuffer(void) = 0;

        /*!
         * \brief Clean up and free allocated memory if needed
         */
        virtual void terminate(void) = 0;

        /*!
         *  \brief Calculate the number of frames Per Seconds
         */
        void calculateFPS(void);

        /*!
         *  \brief Get the number of frames per seconds
         *  @return the FPS
         */
        unsigned int FPS(void) const;

        /*!
         *  \brief Get the total running time of the application
         *  @return a value in seconds
         */
        float runningTime(void) const;

        /*!
         *  \brief Get the dimensions of the frame buffer that are passed to the glviewport
         *  @return the width and the height values
         */
        std::tuple<int, int> frameBufferDimensions(void) const;

        /*!
         *  \brief Set the dimensions of the frame buffer
         *  @param pWidth is the width of the framebuffer in pixels
         *  @param pHeight is the height of the framebuffer in pixels
         */
        void frameBufferDimensions(int pWidth, int pHeight);

        /*!
         *  \brief Get the dimensions of the window, used for the mouse coordinates for example.
         *  @return the width and the height values
         */
        std::tuple<int, int> windowDimensions(void) const;

        /*!
         *  \brief Get the color associated to a pixel
         *  @param pX is the horizontal coordinate expressed in window dimensions (or similar to values
         *         returned by the mouse callback method)
         *  @param pY is the vertical coordinate expressed in window dimensions (or similar to values
         *         returned by the mouse callback method)
         *  @return a tuple with the following color information: 0: red, 1:green, 2: blue, 3:alpha
         */
        std::tuple<int, int, int, int> pixelColor(int pX, int pY);

        /*!
         *  \brief Set the default configuration for OpenGL
         */
        void defaultConfiguration(void) const;

    protected:
        /*!
         *  \brief Constructor with callbacks parameter
         *  @param pCallbacks is a pointer on the callbacks used by the application associated to this window
         */
        BaseBackend(CallbacksInterface* pCallbacks);

    protected:
        static CallbacksInterface* mCallbacks;
        std::string mTitle;
        int mWidth = -1;
        int mHeight = -1;
        int mFrameBufferWidth = -1;
        int mFrameBufferHeight = -1;
        std::chrono::high_resolution_clock::time_point mFrameTime;
        std::chrono::high_resolution_clock::time_point mStartTime;
        unsigned int mFrameCount = 0;
        unsigned int mFPS = 0;

    }; // class BaseBackend

} // namespace miniGL
