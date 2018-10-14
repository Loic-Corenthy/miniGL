//===============================================================================================//
/*!
 *  \file      Texture.cpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#include "Texture.hpp"

#include <cassert>

#include "GLUtils.hpp"
#include "Log.hpp"

using std::string;
using miniGL::Texture;
using miniGL::Log;

Texture::Texture()
:mTextureTarget(-1),
 mTextureObject(-1),
 mPath(),
 mImage()
{

}

Texture::Texture(GLenum pTarget,
                 const string & pPath)
:mTextureTarget(pTarget),
 mTextureObject(-1),
 mPath(pPath),
 mImage()
{

}

Texture::~Texture(void)
{
    string lMessage("Destructor ");
    lMessage.append(__FUNCTION__);
    Log::consoleMessage(lMessage);

    glDeleteTextures(1, &mTextureObject); checkOpenGLState;

    lMessage.assign("Deleting ");
    lMessage.append(mPath);
    Log::consoleMessage(lMessage);
}

bool Texture::loadImage(const string & pPath)
{
    if (!pPath.empty())
        mPath = pPath;

    try
    {
        mImage.read(mPath);
        mImage.write(&mBlob, "RGBA");
    }
    catch (Magick::Error & error)
    {
        string lMessage("Loading texture ");
        lMessage.append(mPath);
        lMessage.append(" : ");
        lMessage.append(error.what());
        Log::write(Log::EType::ERROR, lMessage);
        return false;
    }

    glGenTextures(1, &mTextureObject); checkOpenGLState;
    glBindTexture(mTextureTarget, mTextureObject); checkOpenGLState;

    switch (mTextureTarget)
    {
        case GL_TEXTURE_1D:
            assert(false && "Not implemented yet");
            break;

        case GL_TEXTURE_2D:
            glTexImage2D(mTextureTarget, 0, GL_RGBA, static_cast<GLsizei>(mImage.columns()), static_cast<GLsizei>(mImage.rows()), 0, GL_RGBA, GL_UNSIGNED_BYTE, mBlob.data());
            glGenerateMipmap(mTextureTarget);
            checkOpenGLState;
            break;

        case GL_TEXTURE_3D:
            assert(false && "Not implemented yet");
            break;

        default:
            assert(false && "Wrong texture target");
            break;
    }

    glTexParameterf(mTextureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR); checkOpenGLState;
    glTexParameterf(mTextureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR); checkOpenGLState;
    glBindTexture(mTextureTarget, 0); checkOpenGLState;

    mImageLoaded = true;

    return true;
}


