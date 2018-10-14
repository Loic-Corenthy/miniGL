//===============================================================================================//
/*!
 *  \file      PickingRender.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

#include <string>

#include <GL/glew.h>

#include "Program.hpp"
#include "CallbacksRender.hpp"
#include "Algebra.hpp"
#include "Constants.hpp"

namespace miniGL
{
    /*!
     *  \brief   This class manages the shaders for the 3D picking technique
     *  \details Contain the picking render and the simple color render
     */
    class PickingRender : public Program, public CallbacksRender
    {
    public:
        /*!
         *  \brief Load and compile the shaders associated to this 3d picking technique.
         *         Initialize the id of the uniform variables used in the shaders.
         *  @param pPathVS is the path to the vertex shader
         *  @param pPathFS is the path to the fragment shader
         */
        void init(const std::string & pPathVS = std::string(R"(./Shaders/Picking.vert)"), const std::string & pPathFS = std::string(R"(./Shaders/Picking.frag)"));

        /*!
         *  \brief Set the world view projection matrix
         *  \param pWVP is a 4x4 matrix
         */
        void WVP(const mat4f & pWVP);

        /*!
         *  \brief Set the index of the object that is being rendered
         *  \param pObjectIndex is the current index
         */
        void objectIndex(unsigned int pObjectIndex);

        /*!
         *  \brief Virtual method from CallbacksRender
         */
        virtual void drawStartCallback(unsigned int pDrawIndex) final;

    private:
        /*!
         *  \brief Implementation of a virtual method from Program
         */
        virtual bool checkUniformLocations(void) const final;

    private:
        GLuint mWVPLocation = Constants::invalidUniformLocation<GLuint>();
        GLuint mDrawIndexLocation = Constants::invalidUniformLocation<GLuint>();
        GLuint mObjectIndexLocation = Constants::invalidUniformLocation<GLuint>();

    }; // class PickingRender

} // namespace miniGL
