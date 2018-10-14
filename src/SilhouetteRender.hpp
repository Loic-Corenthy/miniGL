//===============================================================================================//
/*!
 *  \file      SilhouetteRender.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

#include <GL/glew.h>

#include "Algebra.hpp"
#include "Constants.hpp"
#include "Program.hpp"

namespace miniGL
{
    /*!
     *  \brief This class handles the shaders used to render a "silhouette", i.e. a polyline at the border
     *         between the triangles lit by a light and their neighbors that are not
     *  \details The shaders contain a geometry shader to emit the above mentioned polyline
     */
    class SilhouetteRender : public Program
    {
    public:
        /*!
         *  \brief Destructor
         */
        virtual ~SilhouetteRender(void);

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
        void worldMatrix(const mat4f & pWorld);

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
        GLuint mLightPosLocation = Constants::invalidUniformLocation<GLuint>();

    }; // class SilhouetteRender

} // namespace miniGL
