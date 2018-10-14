//===============================================================================================//
/*!
 *  \file      ParticleSystemRender.cpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#include "ParticleSystemRender.hpp"

#include "Shader.hpp"
#include "Exceptions.hpp"
#include "GLUtils.hpp"

using std::string;
using miniGL::ParticleSystemRender;
using miniGL::Exceptions;

void ParticleSystemRender::init(const string & pPathVS, const string & pPathGS)
{
    // Create, load and compile a vertex shader
    Shader lVS(Shader::ETYPE::VERTEX);

    lVS.loadText(pPathVS.c_str());
    lVS.compile();

    // Create, load and compile a geometry shader
    Shader lGS(Shader::ETYPE::GEOMETRY);

    lGS.loadText(pPathGS.c_str());
    lGS.compile();

    // Load the vertex, geometry and fragment shader into our program, link and validate
    attachShader(lVS);
    attachShader(lGS);

    /*! \remark To specify the attributes that go into the buffer we have to call glTransformFeedbackVaryings()
                BEFORE THE TECHNIQUE PROGRAM IS LINKED!!!! */
    const GLchar* lVaryings[4] = {"position1", "type1" , "velocity1", "age1"};
    glTransformFeedbackVaryings(Program::id(), 4, lVaryings, GL_SEPARATE_ATTRIBS);

    link(); checkOpenGLState;


    detachAndDeleteShader(lVS);
    detachAndDeleteShader(lGS);

    mDeltaTimeLocation = uniformLocation("uDeltaTime");
    mRandomTextureLocation = uniformLocation("uRandomTexture");
    mTimeLocation = uniformLocation("uTime");
    mLauncherLifetimeLocation = uniformLocation("uLauncherLifetime");
    mShellLifetimeLocation = uniformLocation("uShellLifetime");
    mSecondaryShellLifetimeLocation = uniformLocation("uSecondaryShellLifetime");
    mInitialLauncherPosition = uniformLocation("uInitialLauncherPosition");

    if(!checkUniformLocations())
        throw Exceptions("Not all uniform locations were updated", __FILE__, __LINE__);

}

void ParticleSystemRender::launcherLifetime(float pValue)
{
    glUniform1f(mLauncherLifetimeLocation, pValue);
}

void ParticleSystemRender::launcherInitialPosition(const vec3f & pPosition)
{
    glUniform3f(mInitialLauncherPosition, pPosition.x(), pPosition.y(), pPosition.z());
}

void ParticleSystemRender::shellLifetime(float pValue)
{
    glUniform1f(mShellLifetimeLocation, pValue);
}

void ParticleSystemRender::secondaryShellLifetime(float pValue)
{
    glUniform1f(mSecondaryShellLifetimeLocation, pValue);
}

void ParticleSystemRender::deltaTime(unsigned int pValue)
{
    glUniform1f(mDeltaTimeLocation, static_cast<float>(pValue));
}

void ParticleSystemRender::time(int pValue)
{
    glUniform1f(mTimeLocation, static_cast<float>(pValue));
}

void ParticleSystemRender::randomTextureUnit(unsigned int pTexUnit)
{
    glUniform1i(mRandomTextureLocation, pTexUnit);
}

bool ParticleSystemRender::checkUniformLocations(void) const
{
    return (mDeltaTimeLocation != Constants::invalidUniformLocation<GLuint>()               &&
            mRandomTextureLocation != Constants::invalidUniformLocation<GLuint>()           &&
            mTimeLocation != Constants::invalidUniformLocation<GLuint>()                    &&
            mLauncherLifetimeLocation != Constants::invalidUniformLocation<GLuint>()        &&
            mShellLifetimeLocation != Constants::invalidUniformLocation<GLuint>()           &&
            mSecondaryShellLifetimeLocation != Constants::invalidUniformLocation<GLuint>()  &&
            mInitialLauncherPosition  != Constants::invalidUniformLocation<GLuint>()        );
}
