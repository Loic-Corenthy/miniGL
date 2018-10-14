//===============================================================================================//
/*!
 *  \file      SSAOBlur.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

#include <GL/glew.h>

#include "Algebra.hpp"
#include "Constants.hpp"
#include "Program.hpp"
#include "IOBuffer.hpp"

namespace miniGL
{
    /*!
     *  \brief   This class handles the shaders that apply a blur to the SSAO technique
     *  \details Very simple blurring technique, sampling 16 points around the origin
     */
    class SSAOBlur : public Program
    {
    public:
        /*!
         *  \brief Destructor
         */
        virtual ~SSAOBlur(void);

        /*!
         *  \brief Load shaders and get uniform IDS.
         */
        void init(void);

        /*!
         *  \brief Bind the
         */
        void bindInputBuffer(IOBuffer & pInputBuffer);

    protected:
        /*!
         *  \brief Implementation of a virtual method from Program
         */
        virtual bool checkUniformLocations(void) const final;

    private:
        GLuint mInputTextureUnitLocation = Constants::invalidUniformLocation<GLuint>();

    }; // class SSAOBlur

} // namespace miniGL
