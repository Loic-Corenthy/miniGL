//===============================================================================================//
/*!
 *  \file      Texture.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

#include <string>

#include <GL/glew.h>

#include <Magick++.h>

namespace miniGL
{
    /*!
     *  \brief This class encapsulate the creation of a texture.
     *  \details This class uses the library ImageMagick to load the images.
     */
    class Texture
    {
    public:
        /*!
         *  \brief Default constructor
         */
        Texture(void);

        /*!
         *  \brief Construtor with parameters
         *  @param pTarget is the type of texture: GL_TEXTURE_1D, GL_TEXTURE_2D, etc.
         *  @param pPath is the path to the image that will be used as texture object
         */
        Texture(GLenum pTarget, const std::string & pPath);

        /*!
         *  \brief Destructor
         */
        ~Texture(void);

        /*!
         *  \brief Set the texture target
         *  @param pTarget is the type of texture: GL_TEXTURE_1D, GL_TEXTURE_2D, etc.
         */
        void target(GLenum pTarget);

        /*!
         *  \brief Set the path to the image
         *  @param pPath is the path to the image that will be used as texture object
         */
        void path(const std::string & pPath);

        /*!
         *  \brief Actually load the texture
         *  @param pPath is the path to the image file. It is an optional parameter that would override the path set with the other methods
         */
        bool loadImage(const std::string & pPath = std::string(""));

        /*!
         *  \brief Bind the texture to openGL
         *  @param pTextureUnit is the texture unit, e.g. GL_TEXTURE0, ...
         */
        void bind(GLenum pTextureUnit);

        /*!
         *  \brief Check if an image was already loaded to the texture
         *  @return true if an image was already loaded, false otherwise
         */
        bool isLoaded(void) const noexcept;

    private:
        GLenum mTextureTarget;
        GLuint mTextureObject;
        std::string mPath;
        Magick::Image mImage;
        Magick::Blob mBlob;
        bool mImageLoaded = false;

    }; // class Texture

    inline void Texture::target(GLenum pTarget)
    {
        mTextureTarget = pTarget;
    }

    inline void Texture::path(const std::string & pPath)
    {
        mPath = pPath;
    }

    inline void Texture::bind(GLenum pTextureUnit)
    {
        glActiveTexture(pTextureUnit);
        glBindTexture(mTextureTarget, mTextureObject);
    }

    inline bool Texture::isLoaded() const noexcept
    {
        return mImageLoaded;
    }

} //namespace miniGL

