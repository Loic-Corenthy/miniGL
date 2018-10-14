//===============================================================================================//
/*!
 *  \file      DeferredShadingGeometryPass.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 *  \date      28/12/16
 */
//===============================================================================================//

#pragma once

#include <array>

#include <GL/glew.h>

#include "Algebra.hpp"
#include "EnumClassCast.hpp"
#include "Constants.hpp"
#include "Program.hpp"

namespace miniGL
{
    /*!
     *  \brief   This class handles the "Geometry pass" in the deferred shading technique.
     *  \details The geometry pass sends the different attributes of the vertices (e.g. position, texture coordinates, diffuse component, normals) to different buffers (the G buffer).
                 The main point of this pass is that all the pixels that have failed the depth test have been dropped  and
                 what is left in the G buffer are only the pixels for which lighting must be calculated.
     */
    class DeferredShadingGeometryPass : public Program
    {
    public:
        /*!
         * \brief Initialize the geometry buffer: get handles for the fbo, the depth texture and the different textures for the VS parameters
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
        void worldMatrix(const mat4f & pWorld);

        /*!
         *  \brief Set the unit of the color texture
         *  @param pTextureUnit is in {0,1,2,3}
         */
        void colorTextureUnit(unsigned int pTextureUnit);

    private:
        /*!
         *  \brief Implementation of a virtual method from Program
         */
        virtual bool checkUniformLocations(void) const;

    private:
        GLuint mWVPLocation = Constants::invalidUniformLocation<GLuint>();
        GLuint mWorldLocation = Constants::invalidUniformLocation<GLuint>();
        GLuint mColorTextureLocation = Constants::invalidUniformLocation<GLuint>();

    }; // class DeferredShadingGeometryPass

} // namespace miniGL
