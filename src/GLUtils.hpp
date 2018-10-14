//===============================================================================================//
/*!
 *  \file      GLUtils.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

#include <string>
#include <iostream>

#include <GL/glew.h>

namespace miniGL
{
    /*!
     *  \brief This class defines helper methods to check the current state of opengl
     *  \details The methods are static methods
     */
    class GLUtils
    {
    public:
        /*!
         *  \brief Check the current status of openGL
         *  @param pFile is the file where the error occured (if the status is not GL_NO_ERROR
         *  @param pLineNumber is the line where the error occured (if the status is not GL_NO_ERROR
         *  @param pLog specifies if the error message should be logged (true) or simply outputed in the console (false)
         */
        static void check(const std::string & pFile = std::string("file not specified"), int pLineNumber = -1, bool pLog = true);

        /*!
         *  \brief Get the ID of the framebuffer that is currently bound for reading
         *  @return the id associated to GL_READ_FRAMEBUFFER_BINDING
         */
        static GLint currentReadFBO(void);

        /*!
         *  \brief Get the ID of the framebuffer that is currently bound for writing
         *  @return the id associated to GL_DRAW_FRAMEBUFFER_BINDING
         */
        static GLint currentWriteFBO(void);

        /*!
         *  \brief Display a message in the console with the current state of several openGL parameters
         */
        static void currentState(void);

    private:
        static GLint mCurrentReadFBO;
        static GLint mCurrentWriteFBO;
        static GLboolean mDepthTestIsEnabled;
        static GLboolean mDepthTestWriteMaskIsEnabled;
        static GLboolean mStencilTestIsEnabled;
        static GLboolean mBlendIsEnabled;
        static GLboolean mCullFaceIsEnabled;
        static GLboolean mTextureCubeMapIsEnabled;
        static GLboolean mRasterizerDiscardIsEnabled;

    }; // class GLUtils

} // namespace miniGL

//#define REGISTER_METHOD_CALL(METHOD_NAME) Log::instance()->write(Log::EType::COMMENT, METHOD_NAME);
//#define REGISTER_METHOD_CALL(METHOD_NAME) std::cout << METHOD_NAME << std::endl;
#define REGISTER_METHOD_CALL(METHOD_NAME)

#define checkOpenGLState miniGL::GLUtils::check(__FILE__, __LINE__)
