//===============================================================================================//
/*!
 *  \file      Log.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

#include <string>

namespace miniGL
{
    /*!
     *  \brief This class encapsulates the use of a log file.
     *  \details The log class is implemented as a singleton. It is always accessible via the static instance method
     */
    class Log
    {
    public:
        enum class EType
        {
            COMMENT, /*! The message to save in the log file is a simple commment */
            ERROR    /*! The message to save in the log file is an error message */
        };

        enum class EDecoration
        {
            NONE, /*! Output the message as is */
            TITLE /*! Output the message within two lines of "=" */
        };

    public:
        /*!
         *  \brief Default constructor not accessible.
         */
        Log(void) = delete;

        /*!
         *  \brief Copy constructor not accessible.
         */
        Log(const Log & pLog) = delete;

        /*!
         *  \brief Copy operator not accessible.
         */
        Log operator=(const Log & pLog) = delete;

        /*!
         *  \brief Insert current date and time in the log file
         */
        static void restart(void);

        /*!
         *  \brief Write a message in the log file
         *  @param pType is to select between a comment or an error message
         *  @param pMessage is the message that will be written in the log file
         *  @param pNewLine allows to insert a blanck line before the writing the message
         */
        static void write(EType pType, const std::string & pMessage, bool pNewLine = false);

        /*!
         *  \brief Set the file where the messages will be written
         *  @param pFile is the complete path to the log file
         */
        static void file(const std::string & pFile);

        /*!
         *  \brief Globally activate or deactivate messages in the console
         *  @param pActivate is true to turn messages on, false otherwise
         */
        static void activateConsoleMessage(bool pActivate);

        /*!
         *  \brief Output a message in the console
         *  @param pMessage is the message to output
         *  @param pDecoration allows to modify how the message is displayed
         */
        static void consoleMessage(const std::string & pMessage, EDecoration pDecoration = EDecoration::NONE);

    private:
        static std::string mLogFile;
        static bool mActivateConsoleMessages;

    }; // class Log

} // namespace miniGL
