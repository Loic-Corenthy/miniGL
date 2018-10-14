//===============================================================================================//
/*!
 *  \file      RandomTexture.cpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#include "RandomTexture.hpp"

#include <vector>
#include <chrono>
#include <random>

#include "Algebra.hpp"
#include "GLUtils.hpp"

using std::vector;
using std::chrono::system_clock;
using std::uniform_real_distribution;
using std::default_random_engine;
using miniGL::RandomTexture;

RandomTexture::~RandomTexture(void)
{
    if (mTextureObject != 0)
        glDeleteTextures(1, & mTextureObject);
}

void RandomTexture::init(unsigned int pSize)
{
    default_random_engine lGenerator;
    lGenerator.seed(static_cast<unsigned int>(system_clock::now().time_since_epoch().count()));

    uniform_real_distribution<float> lDistribution(0, 1.0f);

    vector<vec3f> lRandomData(pSize, vec3f(0.0f));

    for (auto & it : lRandomData)
    {
        it.x() = lDistribution(lGenerator);
        it.y() = lDistribution(lGenerator);
        it.z() = lDistribution(lGenerator);
    }

    glGenTextures(1, & mTextureObject); checkOpenGLState;
    glBindTexture(GL_TEXTURE_1D, mTextureObject); checkOpenGLState;
    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB, pSize, 0, GL_RGB, GL_FLOAT, lRandomData.data()); checkOpenGLState;
    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); checkOpenGLState;
    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); checkOpenGLState;
    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT); checkOpenGLState;

    mSize = pSize;
}

void RandomTexture::bind(GLenum pTextureUnit)
{
    glActiveTexture(pTextureUnit);
    glBindTexture(GL_TEXTURE_1D, mTextureObject);
}


unsigned int RandomTexture::size(void) const noexcept
{
    return mSize;
}
