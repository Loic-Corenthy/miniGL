//===============================================================================================//
/*!
 *  \file      Log.cpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#include "Log.hpp"

#include <iostream>
#include <fstream>
#include <cassert>
#include <chrono>
#include <ctime>

#include "Exceptions.hpp"

using std::cout;
using std::cerr;
using std::endl;
using std::ofstream;
using std::string;
using std::to_string;
using std::time_t;
using std::ctime;
using std::chrono::system_clock;
using miniGL::Log;
using miniGL::Exceptions;

string Log::mLogFile("");
bool Log::mActivateConsoleMessages = true;

void Log::restart(void)
{
    ofstream lFile(mLogFile, ofstream::app);

    if(lFile)
    {
        // Insert current date and time
        auto lCurrentTime = system_clock::now();
        time_t lDate = system_clock::to_time_t(lCurrentTime);
        lFile << endl << endl << "------------------------------------------------" << endl;
        lFile << "Local time: " << ctime(&lDate)  << "------------------------------------------------" << endl;

        lFile.close();
    }
    else
        throw Exceptions("Cannot open log file", __FILE__, __LINE__);
}

void Log::write(EType pType, const std::string & pMessage, bool pNewLine)
{
    ofstream lFile(mLogFile, ofstream::app);

    if(lFile)
    {
        if (pNewLine)
            lFile << endl;

        switch(pType)
        {
            case EType::COMMENT:
                lFile << pMessage << endl;
                cout << pMessage << endl;
                break;

            case EType::ERROR:
                lFile << "ERROR: " << pMessage << endl;
                cerr << pMessage << endl;
                break;

            default:
                assert(false && "Unknown message type");
                break;
        }

        lFile.close();
    }
    else
        throw Exceptions("Cannot open log file", __FILE__, __LINE__);
}

void Log::file(const std::string & pFile)
{
    mLogFile = pFile;
}

void Log::activateConsoleMessage(bool pActivate)
{
    mActivateConsoleMessages = pActivate;
}

void Log::consoleMessage(const std::string & pMessage, EDecoration pDecoration)
{
    if (mActivateConsoleMessages)
    {
        switch (pDecoration)
        {
            case EDecoration::TITLE:
            {
                string line(pMessage.size() + 15,'=');
                cout << endl;
                cout << '\t' << line << '\n';
                cout << '\t' << "Current scene: " << pMessage << '\n';
                cout << '\t' << line << '\n' << endl;
            } break;

            case EDecoration::NONE:
            default:
                cout << pMessage << endl;
                break;
        }
    }
}
