//===============================================================================================//
/*!
 *  \file      SSAORender.cpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#include "SSAORender.hpp"

#include <chrono>
#include <random>
#include <cassert>

#include "EngineCommon.hpp"
#include "Shader.hpp"
#include "Exceptions.hpp"
#include "GLUtils.hpp"

using std::vector;
using std::chrono::system_clock;
using std::uniform_real_distribution;
using std::default_random_engine;
using miniGL::SSAORender;
using miniGL::Shader;
using miniGL::Exceptions;
using miniGL::IOBuffer;

SSAORender::~SSAORender(void)
{
}

void SSAORender::init(void)
{
    // Create, load and compile a vertex shader
    Shader lVS(Shader::ETYPE::VERTEX);

    lVS.loadText("./Shaders/SSAO.vert");
    lVS.compile();

    // Create, load and compile a fragment shader
    Shader lFS(Shader::ETYPE::FRAGMENT);

    lFS.loadText("./Shaders/SSAO.frag");
    lFS.compile();

    // Load the vertex and fragment shader into our program and link
    attachShader(lVS);
    attachShader(lFS);

    link();

    detachAndDeleteShader(lVS);
    detachAndDeleteShader(lFS);

    mDepthTextureUnitLocation = Program::uniformLocation("uDepthMap");
    mSampleRadiusLocation = Program::uniformLocation("uSampleRadius");
    mProjectionLocation = Program::uniformLocation("uProjection");
    mKernelLocation = Program::uniformLocation("uKernel");
    mAspectRatioLocation = Program::uniformLocation("uAspectRatio");
    mTanHalfFOVLocation = Program::uniformLocation("uTanHalfFOV");

    // Check if we correctly initialized the uniform variables
    if (!checkUniformLocations())
        throw Exceptions("Not all uniform locations were updated", __FILE__, __LINE__);

    // Use our program
    use();

    _generateKernel(); checkOpenGLState;

    glUniform1i(mDepthTextureUnitLocation, DEPTH_TEXTURE_UNIT_INDEX); checkOpenGLState;
}

void SSAORender::bindDepthBuffer(IOBuffer & pBuffer)
{
    pBuffer.bindForReading(DEPTH_TEXTURE_UNIT);
}

void SSAORender::sampleRadius(float pRadius)
{
    glUniform1f(mSampleRadiusLocation, pRadius);
}

void SSAORender::projection(const mat4f & pProjection)
{
    glUniformMatrix4fv(mProjectionLocation, 1, GL_TRUE, const_cast<mat4f &>(pProjection).data());
}

void SSAORender::aspectRatio(float pValue)
{
    assert(0.0f <= pValue && "The aspect ratio must be a positive value");
    glUniform1f(mAspectRatioLocation, pValue);
}

void SSAORender::FOV(radianf pValue)
{
    glUniform1f(mTanHalfFOVLocation, tanf(pValue / 2.0f));
}

bool SSAORender::checkUniformLocations(void) const
{
    return (mDepthTextureUnitLocation != Constants::invalidUniformLocation<GLuint>() &&
            mSampleRadiusLocation != Constants::invalidUniformLocation<GLuint>()     &&
            mProjectionLocation != Constants::invalidUniformLocation<GLuint>()       &&
            mKernelLocation != Constants::invalidUniformLocation<GLuint>()           &&
            mAspectRatioLocation != Constants::invalidUniformLocation<GLuint>()      &&
            mTanHalfFOVLocation != Constants::invalidUniformLocation<GLuint>()       );
}

void SSAORender::_generateKernel(void)
{
    const unsigned int lKernelSize = 64;

    vec3f lKernel[lKernelSize];

    default_random_engine lGenerator;
    lGenerator.seed(static_cast<unsigned int>(system_clock::now().time_since_epoch().count()));

    uniform_real_distribution<float> lDistribution(0, 1.0f);

    for (unsigned int i = 0; i < lKernelSize; ++i)
    {
        float lScale = static_cast<float>(i) / static_cast<float>(lKernelSize);
        vec3f lVector(2.0f * lDistribution(lGenerator) - 1.0f, 2.0f * lDistribution(lGenerator) - 1.0f, 2.0f * lDistribution(lGenerator) - 1.0f);

        // Use an acceleration function so that more points are located closer to the origin
        lVector *= (0.1f + 0.9f * lScale * lScale);

        lKernel[i] = lVector;
    }

    glUniform3fv(mKernelLocation, lKernelSize, reinterpret_cast<const GLfloat* >(lKernel)); checkOpenGLState;
}
