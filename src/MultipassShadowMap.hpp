//===============================================================================================//
/*!
 *  \file      MultipassShadowMap.hpp
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
     *  \brief This class handles the shaders used to render shadows using multipass shadow mapping
     *  \details This render is useful to create shadows with point lights
     */
    class MultipassShadowMap : public Program
    {
    public:
        /*!
         *  \brief Load shaders and get uniform IDS.
         */
        void init(void);

        /*!
         *  \brief Set the world view projection matrix
         *  @param pWVP is a 4x4 matrix
         */
        void WVP(const mat4f & pWVP);

        /*!
         *  \brief Set the world matrix
         *  @param pWorld is a 4x4 matrix
         */
        void world(const mat4f & pWorld);

        /*!
         *  \brief Set the light position
         *  @param pPos is given in world coordinates
         */
        void lightPosition(const vec3f & pPos);

    protected:
        /*!
         *  \brief Implementation of a virtual method from Program
         */
        virtual bool checkUniformLocations(void) const final;

    private:
        GLuint mWVPLocation = Constants::invalidUniformLocation<GLuint>();
        GLuint mWorldLocation = Constants::invalidUniformLocation<GLuint>();
        GLuint mLightWorldPositionLocation = Constants::invalidUniformLocation<GLuint>();

    }; // class MultipassShadowMap

} // namespace miniGL
