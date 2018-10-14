//===============================================================================================//
/*!
 *  \file      Shader.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace miniGL
{
    /*!
     *  \brief This class encapsulate the operations associated to shaders. It works with the Program class
     *  \details This class encapsulate the creation of the openGL program, and make it easier to link shaders
     *           and check for errors.
     */
    class Shader
    {
        friend class Program;

    public:
        enum class ETYPE : size_t
        {
            UNDEFINED,  /*! Define the shader as a generic shader, created by default */
            VERTEX,     /*! Define the shader as a vertex shader */
            TESSELLATION_CONTROL,
            TESSELLATION_EVALUATION,
            GEOMETRY,   /*! Define the shader as a geometry shader */
            FRAGMENT    /*! Define the shader as a fragment shader */
        };

    public:
        /*!
         *  \brief Default constructor. This shader is of type UNDEFINED!
         */
        Shader(void);

        /*!
         *  \brief Constructor with type parameter
         *  @param pType can be VERTEX, GEOMETRY or FRAGMENT
         */
        Shader(ETYPE pType);

        /*!
         *  \brief Copy constructor, make inaccessible
         *  @param pShader is the object to copy parameters from
         */
        Shader(const Shader & pShader) = delete;

        /*!
         *  \brief Copy operator, make inaccessible
         *  @param pShader is the object to copy parameters from
         *  @return a reference on this
         */
        Shader & operator=(const Shader & pShader) = delete;

        /*!
         *  \brief Destructor
         */
        ~Shader(void);

        /*!
         *  \brief Load the file containing the shader
         *  @param pPath is the path and the name of the file containing the text of the shader
         */
        void loadText(const char* pPath);

        /*!
         *  \brief Compile the shader
         */
        void compile(void);

        /*!
         *  \brief Set the shader type
         *  @param pType can be VERTEX, GEOMETRY or FRAGMENT
         */
        void type(ETYPE pType);

        /*!
         *  \brief Get the shader type
         *  @return VERTEX, GEOMETRY or FRAGMENT
         */
        ETYPE type(void) const noexcept;

        /*!
         *  \brief Get shader object ID
         *  @return the id associated to the shader
         */
        GLuint objectID(void) const noexcept;

        /*!
         *  \brief Check if the shader has already been compiled
         *  @return true if it is the case, false otherwise
         */
        bool isCompiled(void) const noexcept;

    private:
        /*!
         *  \brief Create the opengl object corresponding to the shader type
         */
        void _createObject(void);

    private:
        GLuint mObjID;
        std::string mPath;
        std::string mText;
        ETYPE mType;
        bool mTextLoaded;
        bool mIsCompiled;
        bool mIsAttachedToProgram;

    }; // class Shader

} // namespace miniGL
