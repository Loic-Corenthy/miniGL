//===============================================================================================//
/*!
 *  \file      ShadowMap.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

#include <GL/glew.h>

#include "Program.hpp"
#include "Algebra.hpp"

namespace miniGL
{
    /*!
     *  \brief This class implements a simple shadow rendering
     *  \details This class handles the shaders used to compute the shadow of objects in the scene
     */
    class ShadowMap : public Program
    {
    public:
        /*!
         *  \brief Default constructor
         */
        ShadowMap(void);

        /*!
         *  \brief Destructor
         */
        ~ShadowMap(void);

        /*!
         *  \brief Load and compile the shaders associated to this shadow map technique.
         *         Initialize the id of the uniform variables used in the shaders.
         *  @param pPathVS is the path to the vertex shader
         *  @param pPathFS is the path to the fragment shader
         */
        void init(const std::string & pPathVS, const std::string & pPathFS);

        /*!
         *  \brief Set world view projection matrix
         *  @param pWVP is a 4x4 matrix
         */
        void setWVP(const mat4f & pWVP);

        /*!
         *  \brief Set the texture unit
         *  @param pTextureUnit is the handle of the texture that will be used to render the shadow
         */
        void setTextureUnit(unsigned int pTextureUnit);

    private:
        /*!
         *  \brief Implementation of a virtual method from Program
         */
        virtual bool checkUniformLocations(void) const final;

        /*!
         *  \brief Get the id of the uniform WVP matrix in the shader
         */
        void initWVPUniformLocation(const char* pName);

        /*!
         *  \brief Get the id of the uniform texture in the shader
         */
        void initTextureUniformLocation(const char* pName);

    private:
        GLuint mWVPLocation;
        GLuint mTextureLocation;

    }; //class ShadowMap

} // namespace miniGL

