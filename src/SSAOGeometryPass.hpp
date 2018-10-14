//===============================================================================================//
/*!
 *  \file      SSAOGeometryPass.hpp
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
     *  \brief This class handles the shaders for the geometry pass in the SSAO rendering technique
     *  \details The geometry pass doesn't compute anything apart from filling the Z buffer. We will reconstruct
     *           the coordinates of the points in the scene in view space from the Z coordinate in the SSAO shader
     */
    class SSAOGeometryPass : public Program
    {
    public:
        /*!
         *  \brief Destructor
         */
        virtual ~SSAOGeometryPass(void);

        /*!
         *  \brief Load shaders and get uniform IDS.
         */
        void init(void);

        /*!
         *  \brief Set the world view projection matrix
         *  @param pWVP is a 4x4 matrix
         */
        void WVP(const mat4f & pWVP);

    protected:
        /*!
         *  \brief Implementation of a virtual method from Program
         */
        virtual bool checkUniformLocations(void) const final;

    private:
        GLuint mWVPLocation = Constants::invalidUniformLocation<GLuint>();

    }; // class SSAOGeometryPass

} // namespace miniGL
