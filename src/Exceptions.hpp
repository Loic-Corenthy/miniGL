//===============================================================================================//
/*!
 *  \file      Exceptions.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

#include <exception>
#include <string>

namespace miniGL
{
    /*!
     *  \brief This class enables the creation of custom exceptions
     *  \details Inherit from std::exception and allow us to tune exception messages
     */
    class Exceptions : public std::exception
    {
    public:
        /*!
         *  \brief Constructor with parameters
         *  @param pMessage is the message that will be available using the what method
         *  @param pFile is the name of the file where the exception is thrown from
         *  @param pLineNumber is the number of the line where the exception is thrown from
         */
        Exceptions(const std::string & pMessage, const std::string & pFile = std::string("file not specified"), int pLineNumber = -1);

        /*!
         *  \brief Copy constructor
         *  @param pExceptions is the object to copy parameters from
         */
        Exceptions(const Exceptions & pExceptions);

        /*!
         *  \brief Copy operator
         *  @param pExceptions is the object to copy parameters from
         *  @return a reference of this
         */
        Exceptions& operator= (const Exceptions & pExceptions);

        /*!
         *  \brief Virtual Destructor
         */
        virtual ~Exceptions(void);

        /*!
         *  \brief Implementation of a virtual function from exception
         *  @return The message, the file and line number when catching the exception
         */
        virtual const char* what(void) const noexcept final;

    private:
        std::string mFile;
        std::string mMessage;
        std::string mOutput;
        int mLineNumber;

    }; // class Exceptions

    inline const char* Exceptions::what() const noexcept
    {
        return mOutput.c_str();
    }

} // namespace miniGL
