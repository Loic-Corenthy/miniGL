//===============================================================================================//
/*!
 *  \file      ParticleSystem.cpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#include "ParticleSystem.hpp"

#include <vector>

#include "GLUtils.hpp"
#include "EngineCommon.hpp"

using std::vector;
using std::make_unique;
using miniGL::ParticleSystem;

ParticleSystem::~ParticleSystem(void)
{
    if (mParticleBuffers[0] != 0)
    {
        glDeleteBuffers(2, mParticleBuffers);
        checkOpenGLState;
    }

    if (mTransformFeedback[0] != 0)
    {
        glDeleteTransformFeedbacks(2, mTransformFeedback);
        checkOpenGLState;
    }
}

void ParticleSystem::init(unsigned int pParticleCount, const vec3f & pLauncherPos)
{
    mParticleCount = pParticleCount;

    // Create
    mRenderer.init("./Shaders/ParticleUpdate.vert", "./Shaders/ParticleUpdate.geom");

    mRenderer.use();

    mRenderer.randomTextureUnit(RANDOM_TEXTURE_UNIT_INDEX);
    mRenderer.launcherLifetime(100.0f);
    mRenderer.shellLifetime(10000.0f);
    mRenderer.secondaryShellLifetime(7000.0f);
    mRenderer.launcherInitialPosition(pLauncherPos);

    vector<vec3f> lParticlesPositions(pParticleCount);
    vector<float> lParticlesType(pParticleCount);
    vector<vec3f> lParticlesVelocities(pParticleCount);
    vector<float> lParticlesLifeTime(pParticleCount);

    lParticlesPositions[0] = pLauncherPos;
    lParticlesType[0] = 0.0f;
    lParticlesVelocities[0] = vec3f(0.0f, 0.0001f, 0.0f);
    lParticlesLifeTime[0] = 0.0f;

    glGenVertexArrays(2, mVAO);
    glGenBuffers(8, mParticleBuffers); checkOpenGLState;

    for (unsigned int i = 0; i < 2; ++i)
    {
        glBindVertexArray(mVAO[i]);

        glBindBuffer(GL_ARRAY_BUFFER, mParticleBuffers[2*i]);
        glBufferData(GL_ARRAY_BUFFER, lParticlesPositions.size() * sizeof(vec3f), lParticlesPositions.data(), GL_DYNAMIC_COPY);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, mParticleBuffers[2*i+1]);
        glBufferData(GL_ARRAY_BUFFER, lParticlesType.size() * sizeof(float), lParticlesType.data(), GL_DYNAMIC_COPY);
        glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, mParticleBuffers[2*i+2]);
        glBufferData(GL_ARRAY_BUFFER, lParticlesVelocities.size() * sizeof(vec3f), lParticlesVelocities.data(), GL_DYNAMIC_COPY);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, mParticleBuffers[2*i+3]);
        glBufferData(GL_ARRAY_BUFFER, lParticlesLifeTime.size() * sizeof(float), lParticlesLifeTime.data(), GL_DYNAMIC_COPY);
        glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(3);
    }

    glBindVertexArray(0);

    glGenQueries(1, &mQuery);
    glGenQueries(1, &mQuerySavedToTranformFeedback);

    mRandomTexture.init(mParticleCount);
    mRandomTexture.bind(RANDOM_TEXTURE_UNIT);

    mBillbording.init(R"(./Shaders/Billboard.vert)", R"(./Shaders/Billboard.geom)", R"(./Shaders/Billboard.frag)");
    mBillbording.use();
    mBillbording.textureUnit(COLOR_TEXTURE_UNIT_INDEX);
    mBillbording.size(0.2f);

    mBillbording.activateCutoff(Billboard::ECutoff::MAX, true);
    mBillbording.cutoff(Billboard::ECutoff::MAX, 0.5);

    mTexture = make_unique<Texture>(GL_TEXTURE_2D, "fireworks_red.png");
    mTexture->loadImage();
}

void ParticleSystem::render(int pDeltaTime, const mat4f & pVP, const vec3f & pCameraPos)
{
    mTime += pDeltaTime;

    _update(pDeltaTime);

    _render(pVP, pCameraPos);

    mCurrentVB = mCurrentTFB;
    mCurrentTFB = 1 - mCurrentTFB;
}

void ParticleSystem::_update(int pDeltaTime)
{
    mRenderer.use();
    mRenderer.time(mTime % mRandomTexture.size());
    mRenderer.deltaTime(pDeltaTime);

    mRandomTexture.bind(RANDOM_TEXTURE_UNIT);

    glEnable(GL_RASTERIZER_DISCARD);

    glBindVertexArray(mVAO[mCurrentVB]);

    // Note that whenever we bind a transform feedback object the number of vertices in the buffer becomes zero because we called glBindBufferBase() on that buffer while the transform feedback object was originally bound (see the initialization part) with the parameter zero as the offset. OpenGL remembers that so we don't need to call glBindBufferBase() again. It simply happens behind the scenes when the transform feedback object is bound.

    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, mParticleBuffers[2*mCurrentTFB]);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, mParticleBuffers[2*mCurrentTFB+1]);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, mParticleBuffers[2*mCurrentTFB+2]);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 3, mParticleBuffers[2*mCurrentTFB+3]);


    glBeginQuery(GL_PRIMITIVES_GENERATED, mQuery);
    glBeginQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, mQuerySavedToTranformFeedback);

    glBeginTransformFeedback(GL_POINTS);

    if (mIsFirstRender)
    {
        glDrawArrays(GL_POINTS, 0, 1);
        mIsFirstRender = false;
    }
    else
        glDrawArrays(GL_POINTS, 0, mVerticesToDraw);

    glEndTransformFeedback();


    glEndQuery(GL_PRIMITIVES_GENERATED);
    glEndQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN);

    glGetQueryObjectuiv(mQuery, GL_QUERY_RESULT, &mVerticesToRender);
    glGetQueryObjectuiv(mQuerySavedToTranformFeedback, GL_QUERY_RESULT, &mVerticesSaved);

    mVerticesToDraw = mVerticesToRender;

    glBindVertexArray(0);
}

void ParticleSystem::_render(const mat4f & pVP, const vec3f & pCameraPos)
{
    mBillbording.use();
    mBillbording.eyeWorldPosition(pCameraPos);
    mBillbording.VP(pVP);

    glDisable(GL_RASTERIZER_DISCARD);

    // Save the current orientation
    GLint lFrontFaceOrientation;
    glGetIntegerv(GL_FRONT_FACE, & lFrontFaceOrientation);

    // Use the clockwise orientation to render the billboards
    glFrontFace(GL_CW);

    glBindVertexArray(mVAO[mCurrentTFB]);

    mTexture->bind(COLOR_TEXTURE_UNIT);

    glBindBuffer(GL_ARRAY_BUFFER, mParticleBuffers[mCurrentTFB]);

    glDrawArrays(GL_POINTS, 0, mVerticesToDraw);

    glBindVertexArray(0);

    // Restore the original orientation
    glFrontFace(lFrontFaceOrientation);
}
