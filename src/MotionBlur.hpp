//===============================================================================================//
/*!
 *  \file      MotionBlur.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

#include <GL/glew.h>

#include "Program.hpp"
#include "Constants.hpp"

namespace miniGL
{
    /*!
     *  \brief This class handles the shaders used to create motion blur
     *  \details It stores colors and motion vectors in textures
     */
    class MotionBlur : public Program
    {
    public:
        /*!
         *  \brief Load shaders and get uniform IDS.
         */
        void init(void);

        /*!
         *  \brief Set the texture unit for the color texture
         */
        void colorTextureUnit(unsigned int pTextureUnit);

        /*!
         *  \brief Set the texture unit for the motion texture
         */
        void motionTextureUnit(unsigned int pTextureUnit);

    protected:
        /*!
         *  \brief Implementation of a virtual method from Program
         */
        virtual bool checkUniformLocations(void) const final;

    private:
        GLuint mColorTextureLocation = Constants::invalidUniformLocation<GLuint>();
        GLuint mMotionTextureLocation = Constants::invalidUniformLocation<GLuint>();

    }; // class MotionBlur

    inline void MotionBlur::colorTextureUnit(unsigned int pTextureUnit)
    {
        glUniform1i(mColorTextureLocation, pTextureUnit);
    }

    inline void MotionBlur::motionTextureUnit(unsigned int pTextureUnit)
    {
        glUniform1i(mMotionTextureLocation, pTextureUnit);
    }

    inline bool MotionBlur::checkUniformLocations(void) const
    {
        return (mColorTextureLocation != Constants::invalidUniformLocation<GLuint>()  &&
                mMotionTextureLocation != Constants::invalidUniformLocation<GLuint>() );
    }

} // namespace miniGL
