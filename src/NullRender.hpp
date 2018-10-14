//===============================================================================================//
/*!
 *  \file      NullRender.hpp
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
     *  \brief This class handles shaders that renders nothing
     *  \details This render is to used when we actually don't need to render something on
     *           screen but to the stencil buffer, depth buffer, etc.
     */
    class NullRender : public Program
    {
    public:
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

    }; // class NullRender

} // namespace miniGL
