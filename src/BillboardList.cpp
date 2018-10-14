//===============================================================================================//
/*!
 *  \file      BillboardList.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#include "BillboardList.hpp"

#include <vector>
#include <iostream>

#include "GLUtils.hpp"
#include "EngineCommon.hpp"

using std::vector;
using std::string;
using std::shared_ptr;
using std::make_unique;
using miniGL::BillboardList;
using miniGL::MeshBase;
using miniGL::Camera;

BillboardList::~BillboardList(void)
{
    if (mVAO != 0)
    {
        glBindVertexArray(mVAO);
        glDisableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        if (mVertexBuffer != Constants::invalidBufferIndex<GLuint>())
            glDeleteBuffers(1, & mVertexBuffer);

        glBindVertexArray(0);
        glDeleteVertexArrays(1, & mVAO);
    }
}

void BillboardList::init(const std::string & pTextureFilename, const vec3f & pOrigin, int pRow, int pCol, float pSize)
{
    mBillboardTexture = make_unique<Texture>(GL_TEXTURE_2D, pTextureFilename);
    mBillboardTexture->loadImage();

    mRow = pRow;
    mCol = pCol;
    _createPositionBuffer(pOrigin, pSize);

    mRenderer.init(string(R"(./Shaders/Billboard.vert)"),
                   string(R"(./Shaders/Billboard.geom)"),
                   string(R"(./Shaders/Billboard.frag)"));

    mRenderer.activateCutoff(Billboard::ECutoff::MAX, true);
    mRenderer.cutoff(Billboard::ECutoff::MAX, 0.9f);
    mRenderer.size(pSize);
}

void BillboardList::render(void)
{
    mRenderer.use();

    const auto lVP = mCamera->projection() * mCamera->view();

    mRenderer.VP(lVP);
    mRenderer.eyeWorldPosition(mCamera->position());

    mBillboardTexture->bind(COLOR_TEXTURE_UNIT);

    // Save the current orientation
    GLint lFrontFaceOrientation;
    glGetIntegerv(GL_FRONT_FACE, & lFrontFaceOrientation);

    // Use the clockwise orientation to render the billboards
    glFrontFace(GL_CW);

    glBindVertexArray(mVAO);

    auto lTotalPts = static_cast<GLsizei>(mRow * mCol);
    glDrawArrays(GL_POINTS, 0, lTotalPts);

    glBindVertexArray(0);

    // Restore the original orientation
    glFrontFace(lFrontFaceOrientation);
}

void BillboardList::camera(shared_ptr<Camera> pCamera)
{
    mCamera = pCamera;
}

void BillboardList::_createPositionBuffer(const vec3f & pOrigin, float pSize)
{
    vector<vec3f> lPositions(mRow * mCol, vec3f(0.0f));

    const auto lSeparation = pSize / 2.0f;

    for (unsigned int j = 0 ; j < mRow ; j++)
    {
        for (unsigned int i = 0 ; i < mCol ; i++)
        {
            vec3f Pos({static_cast<float>(i) * lSeparation, 0.0f, static_cast<float>(j) * lSeparation});
            lPositions[j * mCol + i] = pOrigin + Pos;
        }
    }

    glGenVertexArrays(1, & mVAO);
    glBindVertexArray(mVAO);

    glGenBuffers(1, &mVertexBuffer); checkOpenGLState;
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer); checkOpenGLState;
    glBufferData(GL_ARRAY_BUFFER, lPositions.size() * sizeof(vec3f), lPositions.data(), GL_STATIC_DRAW); checkOpenGLState;

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindVertexArray(0);
}
