//===============================================================================================//
/*!
 *  \file      RandomTexture.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

#include <GL/glew.h>

namespace miniGL
{
    /*!
     *  \brief   This class provides random data from within shaders
     *  \details Create a 1D texture with GL_RGB internal format and floating point data type
     */
    class RandomTexture
    {
    public:
        /*!
         *  \brief Destructor
         */
        ~RandomTexture(void);

        /*!
         *  \brief Initialize the random texture
         *  \param pSize is the number of "pixels" in the texture, i.e. the number of points with RGB
         *         float values
         */
        void init(unsigned int pSize);

        /*!
         *  \brief Bind the texture to openGL
         *  @param pTextureUnit is the texture unit, e.g. GL_TEXTURE0, ...
         */
        void bind(GLenum pTextureUnit);

        /*!
         *  \brief Get the number of "pixels" in the texture, i.e. the number of points with RGB float values
         *  @return the value used in the init method
         */
        unsigned int size(void) const noexcept;

    private:
        GLuint mTextureObject = 0;
        unsigned int mSize = 0;

    }; // class RandomTexture

} // namespace miniGL
