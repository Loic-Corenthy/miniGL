//===============================================================================================//
/*!
 *  \file      CubemapTexture.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

#include <array>
#include <string>
#include <GL/glew.h>
#include <Magick++.h>

namespace miniGL
{
    /*!
     *  \brief   This class takes care of the different textures of the cubemap
     *  \details Helper class to load textures from file to the cubemap and bind them when needed.
     */
    class CubemapTexture
    {
    public:
        /*!
         *  \brief Default constructor
         */
        CubemapTexture(void);

        /*!
         *  \brief Constructor
         *  @param pFilenames is a list of the 6 paths to the different textures.
         *         The textures should be ordered as follow: +X, -X, +Y, -Y, +Z, -Z.
         */
        CubemapTexture(const std::initializer_list<std::string> & pFilenames);

        /*!
         *  \brief Copy constructor not accessible.
         */
        CubemapTexture(const CubemapTexture & pCubemapTexture) = delete;

        /*!
         *  \brief Copy operator not accessible.
         */
        CubemapTexture & operator=(const CubemapTexture & pCubemapTexture) = delete;

        /*!
         *  \brief Destructor
         */
        ~CubemapTexture(void);

        /*!
         *  \brief Load the different textures from file to the cubemap
         *  @param pFilenames is a list of the 6 paths to the different textures.
         *         The textures should be ordered as follow: +X, -X, +Y, -Y, +Z, -Z.
         */
        void load(const std::initializer_list<std::string> & pFilenames = {});

        /*!
         *  \brief Bind the texture unit in order to use it in a shader
         */
        void bind(GLenum pTextureUnit);

    private:
        std::array<std::string, 6> mFilenames;
        const std::array<GLenum, 6> mTypes;
        GLuint mTextureObject = 0;
        bool mFilenamesSet = false;

    }; // class CubemapTexture

    inline void CubemapTexture::bind(GLenum pTextureUnit)
    {
        glActiveTexture(pTextureUnit);
        glBindTexture(GL_TEXTURE_CUBE_MAP, mTextureObject);
    }

} // namespace miniGL
