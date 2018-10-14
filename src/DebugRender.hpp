//===============================================================================================//
/*!
 *  \file      DebugRender.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

#include <GL/glew.h>

#include "Program.hpp"
#include "Algebra.hpp"
#include "Constants.hpp"

namespace miniGL
{
    /*!
     *  \brief This class implements a very simple render for debugging purposes
     *  \details It simply display positions with a dot on screen (of some random color defined
     *           in the fragment shader
     */
    class DebugRender : public Program
    {
    public:
        DebugRender(void);

        /*!
         *  \brief Load and compile the shaders associated to this debug render.
         *         Initialize the id of the uniform variables used in the shaders.
         */
        void init(void);

        /*!
         *  \brief Set the view projection matrix
         *  @param pVP is a 4x4 matrix
         */
        void WVP(const mat4f & pWVP);

       private:
        /*!
         *  \brief Implementation of a virtual method from Program
         */
        virtual bool checkUniformLocations(void) const final;

    private:
        GLuint mWVPLocation = Constants::invalidUniformLocation<GLuint>();

    }; // class DebugRender

    inline void DebugRender::WVP(const mat4f & pWVP)
    {
        glUniformMatrix4fv(mWVPLocation, 1, GL_TRUE, const_cast<mat4f &>(pWVP).data());
    }

    inline bool DebugRender::checkUniformLocations(void) const
    {
        return (mWVPLocation != Constants::invalidUniformLocation<GLuint>());
    }

} // namespace miniGL
