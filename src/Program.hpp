//===============================================================================================//
/*!
 *  \file      Program.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

#include <vector>

#ifdef WIN32
	#include <memory>
#endif

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace miniGL
{
    class Shader;

    /*!
     *  \brief   This base class deals with openGL programs. To be used with shaders built with the Shader class.
     *  \details This class encapsulates the creation of the openGL program, and makes it easier to link shaders
     *           and check for errors.
     */
    class Program
    {
    public:
        /*!
         *  \brief Default constructor. Each program created also gets a handle from openGL associated to the program
         */
        Program(void);

        /*!
         *  \brief Destructor
         */
        virtual ~Program(void);

        /*!
         *  \brief Attach a shader to this program
         *  @param pShader is the shader to attach to this program
         */
        void attachShader(const Shader & pShader);

        /*!
         *  \brief Detach the shader referenced by the ID to this program and delete it. You should do that after linking the program.
         *  @param pShader is the shader to attach to this program
         */
        void detachAndDeleteShader(const Shader & pShader);

        /*!
         *  \brief Call glLinkProgram and check GL_LINK_STATUS
         */
        void link(void);

        /*!
         *  \brief Call glValidateProgram and check GL_VALIDATE_STATUS
         */
        void validate(void);

        /*!
         *  \brief Call glUseProgram
         */
        void use(void) const;

        /*!
         *  \brief Get program ID
         *  @return the id of the handle of this program
         */
        GLuint id(void) const;

        /*!
         *  \brief Get the location of a uniform variable in the memory of the GPU
         *  @param pName is the name of the variable in the one of the shaders that have been loaded to
         *         the program
         *  @return the id of the handle of this variable
         */
        GLint uniformLocation(const char* pName);

        /*!
         *  \brief Check if the program has been linked and validated
         *  @return true if it is okay, false otherwise
         */
        bool isValid(void) const;

        /*!
         *  \brief Get a parameter in the program
         *  @param pParam is the id of the handler of a parameter in the program
         *  @return
         */
        GLint param(GLint pParam) const;

        /*!
         *  \brief Check if all the uniform parameters have been assigned the ID of their location
         *  @return true if all the values are okay, false otherwise
         */
        virtual bool checkUniformLocations(void) const = 0;

    private:
        /*!
         *  \brief Check the info log for errors
         *  @param pStatus is the type of status to check (compile, link, ...)
         */;
        void _checkErrors(GLenum pStatus);

    private:
        GLuint mProgram;
        GLchar mInfoLog[1024];
        bool mProgramIsValid;

    }; // class Program

} // namespace miniGL

