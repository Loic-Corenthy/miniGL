//===============================================================================================//
/*!
 *  \file      BackendGLUT.cpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#include "BackendGLUT.hpp"

#include <string>

#include "Exceptions.hpp"
#include "GLUtils.hpp"
#include "CallbacksInterface.hpp"

using std::string;
using std::to_string;
using miniGL::BackendGLUT;
using miniGL::Exceptions;
using miniGL::CallbacksInterface;

BackendGLUT::BackendGLUT(CallbacksInterface* pCallbacks)
:BaseBackend(pCallbacks)
{
    throw Exceptions("This class is not implemented yet", __FILE__, __LINE__);
}

BackendGLUT::~BackendGLUT(void)
{

}

void BackendGLUT::init(int pWidth, int pHeight,  bool pFullScreen, const std::string & pTitle)
{
    throw Exceptions("This class is not implemented yet", __FILE__, __LINE__);
}

void BackendGLUT::run(void)
{
    throw Exceptions("This class is not implemented yet", __FILE__, __LINE__);
}

void BackendGLUT::swapBuffer(void)
{
    throw Exceptions("This class is not implemented yet", __FILE__, __LINE__);
}

void BackendGLUT::terminate(void)
{
    throw Exceptions("This class is not implemented yet", __FILE__, __LINE__);
}
