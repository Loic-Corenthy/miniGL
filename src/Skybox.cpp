//===============================================================================================//
/*!
 *  \file      Skybox.cpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#include "Skybox.hpp"

#include <cassert>

#include "Transform.hpp"
#include "EngineCommon.hpp"

using std::make_unique;
using std::shared_ptr;
using std::string;
using std::initializer_list;
using miniGL::SkyBox;
using miniGL::SkyBoxRender;
using miniGL::CubemapTexture;
using miniGL::MeshAOS;
using miniGL::Transform;
using miniGL::Camera;

SkyBox::SkyBox(shared_ptr<Camera> pCamera)
:mCamera(pCamera)
{
}

SkyBox::~SkyBox(void)
{
}

void SkyBox::init(const initializer_list<string> & pFilenames)
{
    mRenderer = make_unique<SkyBoxRender>();

    mRenderer->init("./Shaders/SkyBox.vert", "./Shaders/SkyBox.frag");
    mRenderer->use();
    mRenderer->textureUnit(COLOR_TEXTURE_UNIT_INDEX);

    mCubemapTexture = make_unique<CubemapTexture>();
    mCubemapTexture->load(pFilenames);

    mBox = make_unique<MeshAOS>();
    mBox->load("./sphere.obj");

    // Validate our program with the mesh (box)
    mBox->bindVAO(0);
    mRenderer->validate();
    mBox->unbindVAO();
}

void SkyBox::render(void)
{
    mRenderer->use();

    GLint lOldCullFaceMode;
    glGetIntegerv(GL_CULL_FACE_MODE, & lOldCullFaceMode);

    GLint lOldDepthMode;
    glGetIntegerv(GL_DEPTH_FUNC, & lOldDepthMode);

//    glEnable(GL_CULL_FACE);

    // Using front face culling makes the skybox disapear -> go normal back face culling
    //glCullFace(GL_FRONT);
    glDepthFunc(GL_LEQUAL);

    Transform lTransformation;

    lTransformation.scaling(100.0f,100.0f,100.0f);

    mat4f lWorld = lTransformation.final();

    mat4f lWVP = mCamera->projection() * mCamera->view() * lWorld;

    mRenderer->WVP(lWVP);
    mCubemapTexture->bind(COLOR_TEXTURE_UNIT);
    mBox->render();

    glCullFace(lOldCullFaceMode);
    glDepthFunc(lOldDepthMode);
//    glDisable(GL_CULL_FACE);
}
