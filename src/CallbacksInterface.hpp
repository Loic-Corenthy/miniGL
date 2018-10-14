//===============================================================================================//
/*!
 *  \file      CallbacksInterface.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

#include <string>

namespace miniGL
{
    /*!
     *  \brief This class mainly declares methods that the application will inherit and define
     *  \details This class exposes the interface that can be used by the windowing system to interact
     *           with the application
     */
    class CallbacksInterface
    {
    public:

        /*!
         *  \brief Destructor
         */
        virtual ~CallbacksInterface(void);

        /*!
         *  \brief Interface for the keyboard calls
         *  @param pKey is the key pressed on the keyboard
         *  @param pScanCode
         *  @param pAction differentiates between a key pressed, released, etc.
         *  @param pMods
         */
        virtual void keyboardCallBack(int pKey, int pScancode, int pAction, int pMods) = 0;

        /*!
         *  \brief Interface for the passive mouse calls
         *  @param pX is the x coordinate of the position of the mouse on screen
         *  @param pY is the y coordinate of the position of the mouse on screen
         */
        virtual void passiveMouseCallBack(int pX, int pY) = 0;

        /*!
         *  \brief Interface for the mouse calls
         *  @param pButton indicates a left or right button click
         *  @param pState indicates the state of the button: true if pressed, false otherwise
         */
        virtual void mouseCallBack(int pButton, bool pIsPressed) = 0;

        /*!
         *  \brief Interface for the picking process that can be implemented before rendering the scene
         */
        virtual void pickingPhaseCallback(void) = 0;

        /*!
         *  \brief Interface for the rendering of the scene
         */
        virtual void renderPhaseCallBack(void) = 0;

        /**
         * \brief Interface for the callback that will be used to handle error messages
         * @param pError is a number describing the type of error
         * @param pDescription describes the error with a sentence
         */
        virtual void errorCallback(int pError, const char* pDescription) = 0;

        /**
         * \brief Interface for the callback that will be used to send messages to the application
         * @param pMessage is the message
         */
        virtual void messageCallback(const std::string & pMessage) const = 0;

        /**
         * \brief Interface for the callback that will be used when the window is resized
         * @param pWidth is the new width of the framebuffer
         * @param pHeight is the new height of the framebuffer
         */
        virtual void frameBufferSizeCallback(int pWidth, int pHeight) = 0;

    }; // class CallbacksInterface

} // namespace miniGL
