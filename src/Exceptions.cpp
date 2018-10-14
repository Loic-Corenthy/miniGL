//===============================================================================================//
/*!
 *  \file      Exceptions.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#include "Exceptions.hpp"

#include <stdlib.h>

using std::string;
using std::to_string;
using std::exception;
using miniGL::Exceptions;

Exceptions::Exceptions(const string & pMessage, const string & pFile, int pLineNumber)
:exception(),
mFile(pFile),
mMessage(pMessage),
mLineNumber(pLineNumber)
{
    mOutput += "ERROR\nFile: ";
    mOutput += mFile;
    mOutput += "\nLine: ";
    mOutput += to_string(mLineNumber);
    mOutput += "\nMessage: ";
    mOutput += mMessage;
}

Exceptions::Exceptions(const Exceptions & pExceptions)
:exception(pExceptions),
mFile(pExceptions.mFile),
mMessage(pExceptions.mMessage),
mOutput(pExceptions.mOutput),
mLineNumber(pExceptions.mLineNumber)
{
}

Exceptions & Exceptions::operator=(const Exceptions & pExceptions)
{
    if(this == & pExceptions)
        return *this;

    exception::operator=(pExceptions); // Check this line!

    mFile        = pExceptions.mFile;
    mMessage    = pExceptions.mMessage;
    mOutput        = pExceptions.mOutput;
    mLineNumber = pExceptions.mLineNumber;

    return *this;
}

Exceptions::~Exceptions(void)
{
}


