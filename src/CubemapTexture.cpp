//===============================================================================================//
/*!
 *  \file      CubemapTexture.cpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#include "CubemapTexture.hpp"

#include <cassert>
#include <iostream>

#include "GLUtils.hpp"
#include "Exceptions.hpp"

using miniGL::CubemapTexture;
using miniGL::Exceptions;
using Magick::Image;
using Magick::Blob;

CubemapTexture::CubemapTexture(void)
:mTypes({GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z})
{
}

CubemapTexture::CubemapTexture(const std::initializer_list<std::string> & pFilenames)
:mTypes({GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z})
{
    assert(pFilenames.size() == 6);

    unsigned int i = 0;
    for(auto lFilename : pFilenames)
        mFilenames[i++] = lFilename;

    mFilenamesSet = true;
}

CubemapTexture::~CubemapTexture(void)
{
    if(mTextureObject != 0)
    {
        glDeleteTextures(1, & mTextureObject); checkOpenGLState;
    }
}

void CubemapTexture::load(const std::initializer_list<std::string> & pFilenames)
{
    // If filenames are provided as parameters, update the list of file names
    if(pFilenames.size() == 6)
    {
        unsigned int i = 0;
        for(auto lFilename : pFilenames)
            mFilenames[i++] = lFilename;

        mFilenamesSet = true;
    }

    // Make sure the filenames were provided either with the contructor or when calling this method
    assert(mFilenamesSet);

    glGenTextures(1, & mTextureObject); checkOpenGLState;
    glBindTexture(GL_TEXTURE_CUBE_MAP, mTextureObject); checkOpenGLState;

    Blob lBlob;

    for(unsigned int i = 0; i < 6; ++i)
    {
        Image* rImage = new Image(mFilenames[i]);

        try
        {
            rImage->write(&lBlob, "RGBA");
            glTexImage2D(mTypes[i], 0, GL_RGBA, static_cast<GLsizei>(rImage->columns()), static_cast<GLsizei>(rImage->rows()), 0, GL_RGBA, GL_UNSIGNED_BYTE, lBlob.data()); checkOpenGLState;
        }
        catch(Magick::Error & error)
        {
            delete rImage;
            throw Exceptions(error.what(), __FILE__, __LINE__);
        }

        delete rImage;
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR); checkOpenGLState;
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR); checkOpenGLState;
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); checkOpenGLState;
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); checkOpenGLState;
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE); checkOpenGLState;
}
