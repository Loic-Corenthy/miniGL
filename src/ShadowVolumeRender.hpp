//===============================================================================================//
/*!
 *  \file      ShadowVolumeRender.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

#include "Algebra.hpp"
#include "Constants.hpp"
#include "Program.hpp"

namespace miniGL
{
    /*!
     *  \brief This class handles the shaders used to render shadows using volume shadows
     *  \details This type of shadows is useful for point lights
     */
    class ShadowVolumeRender : public Program
    {
    public:
        /*!
         *  \brief Virtual destructor
         */
        virtual ~ShadowVolumeRender(void);

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
        GLuint mLightPosLocation = Constants::invalidUniformLocation<GLuint>();

    }; // class ShadowVolumeRender

} // namespace miniGL
