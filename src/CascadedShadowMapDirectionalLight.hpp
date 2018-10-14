//===============================================================================================//
/*!
 *  \file      CascadedShadowMapDirectionalLight.hpp
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
     *  \brief   This class handles the shaders related to the directional light in the cascaded shadow map technique
     *  \details There is only 1 directional light
     */
    class CascadedShadowMapDirectionalLight : public Program
    {
    public:
        /*!
         *  \brief Destructor
         */
        virtual ~CascadedShadowMapDirectionalLight(void);

        /*!
         *  \brief Load and compile the shaders associated to this shadow map technique.
         *         Initialize the id of the uniform variables used in the shaders.
         *  @param pPathVS is the path to the vertex shader
         *  @param pPathFS is the path to the fragment shader
         */
        void init(const std::string & pPathVS = R"(./Shaders/CascadedShadowMapDirectionalLight.vert)",
                  const std::string & pPathFS = R"(./Shaders/CascadedShadowMapDirectionalLight.frag)");

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

    }; // class CascadedShadowMapDirectionalLight

} // namespace miniGL

