//===============================================================================================//
/*!
 *  \file      ProgramGLFX.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

#include <vector>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace miniGL
{
    /*!
     *  \brief   This base class deals with openGL programs. To be used with shaders built with the GLFX library.
     *  \details This class encapsulates the creation of the openGL program and handles the use of an effect
     *           from glfx
     */
    class ProgramGLFX
    {
    public:
        /*!
         *  \brief Default constructor. Each program created also gets a handle from openGL associated to the program
         */
        ProgramGLFX(void);

        /*!
         *  \brief Destructor
         */
        virtual ~ProgramGLFX(void);

        /*!
         *  \brief Load an effect from a .glsl file
         *  @param pFile is the path
         */
        void effectFile(const std::string & pFile);

        /*!
         *  \brief Compile a program
         *  @param pProgram
         */
        void compileProgram(const std::string & pProgram);

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
         *  \brief Check if all the uniform parameters of all the lights have been assigned the ID of their location
         *  @return true if all the values are okay, false otherwise
         */
        virtual bool checkUniformLocations(void) const = 0;

    private:
        std::string mEffectFile;
        int mEffect = 0;
        int mProgram = 0;
        bool mProgramIsValid = false;
    }; // class ProgramGLFX

} // namespace miniGL
