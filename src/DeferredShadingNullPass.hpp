//===============================================================================================//
/*!
 *  \file      DeferredShadingNullPass.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

#include <GL/glew.h>

#include "Algebra.hpp"
#include "Program.hpp"
#include "Constants.hpp"

namespace miniGL
{
    /*!
     *  \brief Pass used only to fill the stencil buffer
     *  \details This class handles the null pass shaders. The vertex and fragment shaders do almost nothing
     *           but allows to fill the stencil buffer
     */
    class DeferredShadingNullPass : public Program
    {
    public:
        /*!
         * \brief Destructor
         */
        virtual ~DeferredShadingNullPass(void);

        /*!
         * \brief Get the location for all the uniform variables used by the light pass
         */
        void init(void);

        /*!
         *  \brief Set the world view projection matrix
         *  @param pWVP is a 4x4 matrix
         */
        void WVP(const mat4f & pWVP);

    private:
        /*!
         *  \brief Implementation of a virtual method from Program
         */
        virtual bool checkUniformLocations(void) const;

    private:
        GLuint mWVPLocation = Constants::invalidUniformLocation<GLuint>();

    }; // class DeferredShadingNullPass

    inline void DeferredShadingNullPass::WVP(const mat4f & pWVP)
    {
        glUniformMatrix4fv(mWVPLocation, 1, GL_TRUE, const_cast<mat4f&>(pWVP).data());
    }

    inline bool DeferredShadingNullPass::checkUniformLocations(void) const
    {
        return mWVPLocation != Constants::invalidUniformLocation<GLuint>();
    }

} // namespace miniGL
