//===============================================================================================//
/*!
 *  \file      SimpleColorRender.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

#include <string>

#include <GL/glew.h>

#include "Program.hpp"
#include "Algebra.hpp"
#include "Constants.hpp"

namespace miniGL
{
    /*!
     *  \brief   This class manages the shaders for the simple color technique
     *  \details Allows to draw a triangle with a specific color (for triangle selection)
     */
    class SimpleColorRender : public Program
    {
    public:
        /*!
         *  \brief Load and compile the shaders associated to this simple color technique.
         *         Initialize the id of the uniform variables used in the shaders.
         *  @param pPathVS is the path to the vertex shader
         *  @param pPathFS is the path to the fragment shader
         */
        void init(const std::string & pPathVS = std::string(R"(./Shaders/SimpleColor.vert)"), const std::string & pPathFS = std::string(R"(./Shaders/SimpleColor.frag)"));

        /*!
         *  \brief Set the world view projection matrix
         *  \param pWVP is a 4x4 matrix
         */
        void WVP(const mat4f & pWVP);

        /*!
         *  \brief Set the color used to draw the triangle
         *  \param pRed is the red component of the color, value in [0,1]
         *  \param pGreen is the green component of the color, value in [0,1]
         *  \param pBlue is the blue component of the color, value in [0,1]
         *  \param pAlpha is the transparency component of the color, value in [0,1]
         */
        void color(float pRed, float pGreen, float pBlue, float pAlpha = 1.0f);

    private:
        /*!
         *  \brief Implementation of a virtual method from Program
         */
        virtual bool checkUniformLocations(void) const final;

    private:
        GLuint mWVPLocation = Constants::invalidUniformLocation<GLuint>();
        GLuint mColorLocation = Constants::invalidUniformLocation<GLuint>();

    }; // class SimpleColorRender

} // namespace miniGL
