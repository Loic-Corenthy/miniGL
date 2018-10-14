//===============================================================================================//
/*!
 *  \file      ShadowMapDirectionalLight.hpp
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
     *  \brief   This class handles the shaders that manipulate the directional light
     *  \details The main purpose is to compute the position of the light in world space to be able
                 to later compute the orthographic projection.
     */
    class ShadowMapDirectionalLight : public Program
    {
    public:
        /*!
         *  \brief Destructor
         */
        virtual ~ShadowMapDirectionalLight(void);

        /*!
         *  \brief Load and compile the shaders associated to this shadow map technique.
         *         Initialize the id of the uniform variables used in the shaders.
         *  @param pPathVS is the path to the vertex shader
         *  @param pPathFS is the path to the fragment shader
         */
        void init(const std::string & pPathVS = R"(./Shaders/ShadowMapDirectionalLight.vert)",
                  const std::string & pPathFS = R"(./Shaders/ShadowMapDirectionalLight.frag)");

        /*!
         *  \brief Set world view projection matrix
         *  @param pWVP is a 4x4 matrix
         */
        void WVP(const mat4f & pWVP);

    private:
        /*!
         *  \brief Implementation of a virtual method from Program
         */
        virtual bool checkUniformLocations(void) const final;

    private:
        GLuint mWVPLocation = Constants::invalidUniformLocation<GLuint>();

    }; // class ShadowMapDirectionalLight

} // namespace miniGL
