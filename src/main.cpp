//===============================================================================================//
/**
 *  \file      main.cpp
 *  \brief     File where we define our main function
 *  \details   The main function creates, initializes, and runs an application
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#include <string>
#include <iostream>
#include <array>

#include "Application.hpp"
#include "Algebra.hpp"
#include "Angle.hpp"
#include "Constants.hpp"

using namespace std;

int main(int argc, const char * argv[])
{
    // Create the application
    miniGL::Application lApp;

    // Define parameters of the window
    unsigned int lWidth = 1280, lHeight = 800;

    // Define the title of the application
    string lTitle("Mini GL");

    // Return value for the main function
    int lRes = 0;

    // Init and run the application
    try
    {
        lApp.init(lWidth, lHeight, false, lTitle);
        lApp.run();
    }
    catch (std::exception & e)
    {
        cerr << "=======================================" << endl;
        cerr << "Exception caught at main function level" << endl;
        cerr << "=======================================" << endl;
        cerr << e.what() << endl;
        lRes = -1;
    }

    // Return 0 if application ran correctly, -1 otherwise
    return lRes;
}
