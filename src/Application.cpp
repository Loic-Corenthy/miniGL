//===============================================================================================//
/*!
 *  \file      Application.cpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#include "Application.hpp"

#include <iostream>
#include <chrono>
#include <random>
#include <algorithm>
#include <limits>
#include <tuple>

#include "Algebra.hpp"
#include "Angle.hpp"
#include "Shader.hpp"
#include "Exceptions.hpp"
#include "Log.hpp"
#include "GLUtils.hpp"
#include "EngineCommon.hpp"
#include "MeshAOS.hpp"
#include "MeshSOA.hpp"
#include "BackendGLFW.hpp"
#include "Program.hpp"

using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::to_string;
using std::tuple;
using std::make_unique;
using std::make_shared;
using std::static_pointer_cast;
using std::get;
using std::vector;
using std::array;
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::system_clock;
using std::uniform_real_distribution;
using std::default_random_engine;
using std::numeric_limits;
using std::make_unique;
using miniGL::Application;
using miniGL::Exceptions;
using miniGL::Shader;
using miniGL::Transform;
using miniGL::MeshAOS;
using miniGL::MeshSOA;
using miniGL::Log;
using miniGL::Lighting;
using miniGL::ShadowMap;
using miniGL::ShadowMapFBO;
using miniGL::SkyBox;
using miniGL::ParticleSystem;
using miniGL::Skinning;
using miniGL::SilhouetteRender;
using miniGL::BackendGLFW;
using miniGL::Program;

Application::Application(void)
{
    mWindow = make_unique<BackendGLFW>(this);
}

Application::~Application(void)
{
    mWindow->terminate();
}

void Application::init(unsigned int pWindowWidth, unsigned int pWindowHeight, bool pFullScreen, const string & pTitle)
{
    // Restart the log before each use of the program
    Log::file(mLogFile);
    Log::restart();
    Log::activateConsoleMessage(true);

    // Init the glfw window with the parameters from the main function
    mWindow->init(pWindowWidth, pWindowHeight, pFullScreen, pTitle);
    _saveGLParametersInLogFile();

    // Setup the camera
    auto lFrameBufferDims = mWindow->frameBufferDimensions();
    degreef lCameraFOV = 60.0f;
    mCamera = make_shared<Camera>();
    mCamera->verticalFoV(lCameraFOV.toRadian());
    mCamera->frameBufferDimensions(get<0>(lFrameBufferDims), get<1>(lFrameBufferDims));

    auto lWindowDims = mWindow->windowDimensions();
    mCamera->windowDimensions(get<0>(lWindowDims), get<1>(lWindowDims));

    mCamera->nearPlane(1.0f);
    mCamera->farPlane(200.0f);
    mCamera->up(vec3f(0.0f, 1.0f, 0.0f));

    mMeshOrientation.y() = 1.0f;

    // Init the different effects shown in the scene
    try
    {
        _loadMeshes();
        _createLights(4u, 4u);
        _initCurrentRenderingTechnique();

        // Initialise the UI rendering. Not part of _initRenderingTechniques since the UI is always active
        _initATB();
    }
    catch (const Exceptions & e)
    {
        Log::write(Log::EType::ERROR, e.what());
        string lMessage("Exception during initialization:\n");
        lMessage.append(e.what());
        throw Exceptions(lMessage, __FILE__, __LINE__);
    }
}

void Application::run(void)
{
    // The main loop of the application is the loop in the window
    mWindow->run();
}

void Application::keyboardCallBack(int pKey, int pScancode, int pAction, int pMods)
{
    if(pKey == GLFW_KEY_SPACE && pAction == GLFW_PRESS)
    {
        mDisplayCurrentPixelColor = !mDisplayCurrentPixelColor;
    }
    else if(pKey == GLFW_KEY_UP)
    {
        vec3f lTranslation = mCamera->lookAt() * mCameraStep;
        mCamera->position(mCamera->position() + lTranslation);
    }
    else if (pKey == GLFW_KEY_DOWN)
    {
        vec3f lTranslation = mCamera->lookAt() * mCameraStep;
        mCamera->position(mCamera->position() - lTranslation);
    }
    else if (pKey == GLFW_KEY_LEFT)
    {
        vec3f lLeft = mCamera->lookAt().cross(mCamera->up());
        lLeft *= mCameraStep;
        mCamera->position(mCamera->position() + lLeft);
    }
    else if (pKey == GLFW_KEY_RIGHT)
    {
        vec3f lRight = mCamera->up().cross(mCamera->lookAt());
        lRight *= mCameraStep;
        mCamera->position(mCamera->position() + lRight);
    }
    else if (pKey == GLFW_KEY_M)
    {
        vec3f lTranslation = mCamera->up() * mCameraStep;
        mCamera->position(mCamera->position() - lTranslation);
    }
    else if (pKey == GLFW_KEY_N)
    {
        vec3f lTranslation = mCamera->up() * mCameraStep;
        mCamera->position(mCamera->position() + lTranslation);
    }
    else if (pKey == GLFW_KEY_LEFT_CONTROL)
    {
        if(pAction == GLFW_PRESS)
            mCtrlKeyPressed = true;
        else if(pAction == GLFW_RELEASE)
            mCtrlKeyPressed = false;
    }
    else if (pKey == GLFW_KEY_EQUAL && (pAction == GLFW_PRESS || pAction == GLFW_REPEAT) && mTessellation)
    {
        auto lDisplacementFactor = mTessellation->displacementFactor();
        lDisplacementFactor += 0.1f;
        mTessellation->displacementFactor(lDisplacementFactor);
        cout << "displacement factor " << lDisplacementFactor << endl;
    }
    else if (pKey == GLFW_KEY_MINUS && (pAction == GLFW_PRESS || pAction == GLFW_REPEAT) && mTessellation)
    {
        auto lDisplacementFactor = mTessellation->displacementFactor();

        if (lDisplacementFactor >= 0.1f)
            lDisplacementFactor -= 0.1f;

        mTessellation->displacementFactor(lDisplacementFactor);
        cout << "displacement factor " << lDisplacementFactor << endl;
    }
    else if (pKey == GLFW_KEY_T && (pAction == GLFW_PRESS || pAction == GLFW_REPEAT) && mTessellationPN)
    {
        auto lTessellationLevel = mTessellationPN->tessellationLevel(0);
        lTessellationLevel += 0.1f;
        mTessellationPN->tessellationLevel(0, lTessellationLevel);
        cout << "tessellation level " << lTessellationLevel << endl;
    }
    else if (pKey == GLFW_KEY_Y && (pAction == GLFW_PRESS || pAction == GLFW_REPEAT) && mTessellationPN)
    {
        auto lTessellationLevel = mTessellationPN->tessellationLevel(0);

        if (lTessellationLevel >= 0.1f)
            lTessellationLevel -= 0.1f;

        mTessellationPN->tessellationLevel(0, lTessellationLevel);
        cout << "tessellation level " << lTessellationLevel << endl;
    }
    else if (pKey == GLFW_KEY_Z && pAction == GLFW_PRESS)
    {
        mIsWireframe = !mIsWireframe;

        if (mIsWireframe)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            cout << "wireframe" << endl;
        }
        else
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            cout << "plain triangles" << endl;
        }
    }
    else if (pKey == GLFW_KEY_L && pAction == GLFW_PRESS && mSSAOTechnique)
    {
        switch (mSSAOTechnique->shaderType())
        {
            case SSAOTechnique::EShaderType::SSAO:
                mSSAOTechnique->shaderType(SSAOTechnique::EShaderType::NO_SSAO);
                break;

            case SSAOTechnique::EShaderType::NO_SSAO:
                mSSAOTechnique->shaderType(SSAOTechnique::EShaderType::ONLY_AO);
                break;

            case SSAOTechnique::EShaderType::ONLY_AO:
                mSSAOTechnique->shaderType(SSAOTechnique::EShaderType::SSAO);
                break;
        }

    }

    if (pKey != GLFW_KEY_UP && pKey != GLFW_KEY_DOWN && pKey != GLFW_KEY_LEFT && pKey != GLFW_KEY_RIGHT)
    {
        mATB.keyboardCallback(pKey);
    }
}

void Application::passiveMouseCallBack(int pX, int pY)
{
    mMousePos.x() = pX;
    mMousePos.y() = pY;

    mCamera->onRender();

    // Transform mouse position from framebuffer coordinates to window coordinates and send to GUI
    auto lFrameBufferDims = mWindow->frameBufferDimensions();
    auto lWindowDimensions = mWindow->windowDimensions();

    auto lX = static_cast<double>(mMousePos.x()) * static_cast<double>(get<0>(lFrameBufferDims)) / static_cast<double>(get<0>(lWindowDimensions));
    auto lY = static_cast<double>(mMousePos.y()) * static_cast<double>(get<1>(lFrameBufferDims)) / static_cast<double>(get<1>(lWindowDimensions));

    bool lMouseMovementOverGUI = mATB.passiveMouseCallback(static_cast<int>(lX), static_cast<int>(lY));

    // If the GUI didn't process the mouse event, forward it to the "main window" to handle other actions
    if (!lMouseMovementOverGUI)
    {
        if (mMouseButtonIsPressed)
            mCamera->rotateFromMouse(mMousePos.x(), mMousePos.y());

        if (mDisplayCurrentPixelColor)
        {
            auto color = mWindow->pixelColor(mMousePos.x(), mMousePos.y());

            cout << "x: " << mMousePos.x() << " y: " << mMousePos.y() << endl;
            cout << "R: " << get<0>(color) << endl;
            cout << "G: " << get<1>(color) << endl;
            cout << "B: " << get<2>(color) << endl;
            cout << endl;
        }
    }
}

void Application::mouseCallBack(int pButton, bool pIsPressed)
{
    mMouseButtonIsPressed = pIsPressed;

    mATB.mouseCallback(pButton, pIsPressed);
}

void Application::pickingPhaseCallback(void)
{
    if (mPickingOn)
        mPicking3D->pickingPhase(mMeshes);
}

void Application::renderPhaseCallBack(void)
{
    if (mATB.autoRotateActive())
        mRotationAngle += mATB.meshRotationIncrement();

    if (mRotationAngle > 360.0f)
        mRotationAngle -= 360.0f;

    if (mATB.shouldResetMeshOrientation())
    {
        mMeshOrientation.x() = 0.0f;
        mMeshOrientation.y() = 1.0f;
        mMeshOrientation.z() = 0.0f;
        mMeshOrientation.w() = 0.0f;
    }

    // Check if a new rendering technique was selected from the UI
    mPreviousRenderingTechniqueIndex = mCurrentRenderingTechniqueIndex;
    mCurrentRenderingTechniqueIndex = mATB.currentRenderingTechniqueIndex();

    // If we change technique or resize the window, free the memory for the previous one and load the new one
    if (mPreviousRenderingTechniqueIndex != mCurrentRenderingTechniqueIndex || mWindowWasResized)
    {
        _deactivateRenderingTechniques();
        _initCurrentRenderingTechnique();

        if (mWindowWasResized)
            mWindowWasResized = false;
    }

    // Render the current technique
    switch (mCurrentRenderingTechniqueIndex)
    {
        case  0: _render3DPicking(); break;
        case  1: _renderBillboarding(); break;
        case  2: _renderParticleSystem(); break;
        case  3: _renderSimpleLighting(); break;
        case  4: _renderTessellation(); break;
        case  5: _renderTessellationPN(); break;
        case  6: _renderInstancedRendering(); break;
        case  7: _renderGLFXExample(); break;
        case  8: _renderDeferredShading(); break;
        case  9: _renderSkeletalAnimation();break; //Use the same following method to render skeletal animation with motion blur
        case 10: _renderSkeletalAnimation();break;
        case 11: _renderSilhouetteDetection();break;
        case 12: _renderShadowVolume();break;
        case 13: _renderMultipassShadowMapping();break;
        case 14: _renderSSAO();break;
        case 15: _renderShadowMapDirectionalLight(); break;
        case 16: _renderCascadedShadowMapping(); break;
        case 17: mSkyBox->render(); break; // We must render the skybox as the last element before the UI in the render call
        case 18: _renderMultipleExamples(); break;

        default:
            assert(false && "Undefined rendering technique index");
            break;
    }

    // Render the UI on top of the other rendering technique
    mATB.render();
}

void Application::errorCallback(int pError, const char* pDescription)
{
    Log::write(Log::EType::ERROR, pDescription);
}

void Application::messageCallback(const string & pMessage) const
{
    Log::write(Log::EType::COMMENT, pMessage);
}

void Application::frameBufferSizeCallback(int pWidth, int pHeight)
{
    mWindowWasResized = true;

    mWindow->frameBufferDimensions(pWidth, pHeight);

    // Resize the UI accordingly
    mATB.resizeWindow(pWidth, pHeight);
}

void Application::_validateShaderWithMesh(Program* pProgram, const std::string & pName)
{
    mMeshes[pName].mesh->bindVAO(0);
    pProgram->validate();
    mMeshes[pName].mesh->unbindVAO();
}

void Application::_loadMeshes(void)
{
    // Used in _initShadowMapping and _initBumpMapping
    _createMesh<MeshAOS>(string("spaceShip"), string(R"(./phoenix_ugv.md2)"), GL_CW); // Orientation is clockwise on this mesh

    // When using a normal map, the mesh MUST be loaded with the option to compute the tangent space!
    _createMesh<MeshAOS>(string("cube"), string(R"(./box.obj)"), GL_CW, MeshBase::EOptions::COMPUTE_TANGENT_SPACE);

    _createMesh<MeshAOS>(string("spider"), string(R"(./spider.obj)"), GL_CW);

    _createMesh<MeshAOS>(string("terrain"), string(R"(./quad2.obj)"), GL_CCW); // Orientation is counter clockwise on this mesh

    _createMesh<MeshAOS>(string("monkey"), string(R"(./monkey.obj)"), GL_CW);

    // Used in _initSimpleLighting
    _createMesh<MeshSOA>(string("jeep"), string(R"(./jeep.obj)"), GL_CW);
    _createMesh<MeshSOA>(string("helicopter"), string(R"(./hheli.obj)"), GL_CW);

    // Used in _initInstancedRendering
    _createMesh<MeshSOA>(string("spider - instanced rendering"), string(R"(./spider.obj)"), GL_CCW, MeshBase::EOptions::INSTANCE_RENDERING);

    // Used in the GLFX example
    _createMesh<MeshSOA>(string("letter g"), string(R"(./g.obj)"), GL_CCW);
    _createMesh<MeshSOA>(string("letter l"), string(R"(./l.obj)"), GL_CCW);
    _createMesh<MeshSOA>(string("letter f"), string(R"(./f.obj)"), GL_CCW);
    _createMesh<MeshSOA>(string("letter x"), string(R"(./x.obj)"), GL_CCW);

    // Used in the deferred shading example
    _createMesh<MeshSOA>(string("box"), string(R"(./box.obj)"), GL_CW);

    // Used in the skeletal animation example
    /*! \todo  To use skinning with the MeshAOS class, the location indices need to be changed in vertex shader (skinning.vert) */
    _createMesh<MeshSOA>(string("bobLamp"), string(R"(./boblampclean.md5mesh)"), GL_CW);

    // Used in the silhouette detection example
    _createMesh<MeshSOA>(string("cubeWithAdjacencies"), string(R"(./box.obj)"), GL_CW, MeshBase::EOptions::ADJACENCIES);

    // Used in the Multipass Shadow Mapping example
    _createMesh<MeshAOS>(string("quad_r"), string(R"(./quad_r.obj)"), GL_CW);
    _createMesh<MeshAOS>(string("sphere"), string(R"(./sphere.obj)"), GL_CW);

    // Used in _initShadowMapDirectionalLight
    _createMesh<MeshSOA>(string("Dragon"), string(R"(./dragon.obj)"), GL_CW);
    _createMesh<MeshSOA>(string("Buddha"), string(R"(./buddha.obj)"), GL_CW);
    _createMesh<MeshSOA>(string("Bunny"), string(R"(./bunny.obj)"), GL_CW);
    _createMesh<MeshAOS>(string("quad"), string(R"(./quad.obj)"), GL_CW);
}

void Application::_createLights(unsigned int pPointLightCount, unsigned int pSpotLightCount)
{
    // Reserve memory in the container before pushing back
    mLights.reserve(1 + pPointLightCount + pSpotLightCount);

    // Create only 1 directional light
    mLights.push_back(make_shared<DirectionalLight>());

    // Create point lights
    for (unsigned int i = 0; i < pPointLightCount; ++i)
    {
        mLights.push_back(make_shared<PointLight>());
        auto lName = mLights.back()->name();
        lName.append(" [" + to_string(i) + "]");
        mLights.back()->name(lName);
    }

    // Create spot lights
    for (unsigned int i = 0; i < pSpotLightCount; ++i)
    {
        mLights.push_back(make_shared<SpotLight>());
        auto lName = mLights.back()->name();
        lName.append(" [" + to_string(i) + "]");
        mLights.back()->name(lName);
    }
}

void Application::_checkLightsType(void)
{
    // Check directional light
    assert(mLights[0]->type() == BaseLight::EType::DIRECTIONAL && "Ensure the first light is a directional light");

    // Check point lights
    assert(mLights[1]->type() == BaseLight::EType::POINT && mLights[2]->type() == BaseLight::EType::POINT && mLights[3]->type() == BaseLight::EType::POINT && mLights[4]->type() == BaseLight::EType::POINT && "Ensure lights 5 to 8 are a spot lights");

    // Check spot lights
    assert(mLights[5]->type() == BaseLight::EType::SPOT && mLights[6]->type() == BaseLight::EType::SPOT && mLights[7]->type() == BaseLight::EType::SPOT && mLights[8]->type() == BaseLight::EType::SPOT && "Ensure lights 5 to 8 are a spot lights");
}

void Application::_initCurrentRenderingTechnique(void)
{
    switch (mCurrentRenderingTechniqueIndex)
    {
        case  0: _init3DPicking(); break;
        case  1: _initBillboarding(); break;
        case  2: _initParticleSystem(); break;
        case  3: _initSimpleLighting(); break;
        case  4: _initTessellation(); break;
        case  5: _initTessellationPN(); break;
        case  6: _initInstancedRendering(); break;
        case  7: _initGLFXExample(); break;
        case  8: _initDeferredShading(); break;
        case  9: _initSkeletalAnimation(); break; //Use the same following method to render skeletal animation with motion blur
        case 10: _initSkeletalAnimationWithMotionBlur();   break;
        case 11: _initSilhouetteDetection(); break;
        case 12: _initShadowVolume(); break;
        case 13: _initMultipassShadowMapping(); break;
        case 14: _initSSAO(); break;
        case 15: _initShadowMapDirectionalLight(); break;
        case 16: _initCascadedShadowMapping(); break;
        case 17: _initSkybox(); break;
        case 18: _initMultipleExamples(); break;

        default:
            assert(false && "Undefined rendering technique index");
            break;
    }
}

void Application::_deactivateRenderingTechniques(void)
{
    // Remove all transforms set by the different init functions
    for (auto & mesh : mMeshes)
        mesh.second.transform.clear();

    // Reset all the lights
    for (auto light : mLights)
        light->reset();

    switch (mPreviousRenderingTechniqueIndex)
    {
        case 0:
            mPicking3D.reset();
            mSimpleLightingWithShadow.reset();
            mPickingOn = false;
            break;

        case 1: mBillboardList.reset(); break;
        case 2: mParticleSystem.reset(); break;
        case 3: mSimpleLightingWithShadow.reset(); break;
        case 4: mTessellation.reset(); break;
        case 5: mTessellationPN.reset(); break;
        case 6: mInstancedLighting.reset(); break;
        case 7: mGLFXTechnique.reset(); break;
        case 8: mDeferredShading.reset(); break;
        case 9: mSkinningTechnique.reset(); break;
        case 10: mSkinningTechnique.reset(); break;

        case 11:
            mSilhouetteTechnique.reset();
            mSimpleLightingWithShadow.reset();
            break;

        case 12: mShadowVolumeTechnique.reset(); break;
        case 13: mMultipassShadowMapTechnique.reset(); break;
        case 14: mSSAOTechnique.reset(); break;
        case 15: mShadowMapDirectionalLightTechnique.reset(); break;
        case 16: mCascadedShadowMapDirectionalLightTechnique.reset(); break;
        case 17: mSkyBox.reset(); break;
        case 18:
            mPicking3D.reset();
            mInstancedLighting.reset();
            mBillboardList.reset();
            mSimpleLightingWithShadow.reset();
            mGLFXTechnique.reset();
            mTessellation.reset();
            mTessellationPN.reset();
            mParticleSystem.reset();
            mSkyBox.reset();
            mPickingOn = false;
            break;

        default:
            assert(false && "Undefined rendering technique index");
            break;
    }

    mWindow->defaultConfiguration();
}

void Application::_initSimpleLighting(void)
{
    // Display the name of the technique that is initialized
    Log::consoleMessage("Simple lighting", Log::EDecoration::TITLE);

    // Display the state of some configuration parameters of openGL
    GLUtils::currentState();

    _checkLightsType();

    // Not using the directional light

    // Point lights
    for (unsigned int i = 1; i < 5; ++i)
    {
        static_pointer_cast<PointLight>(mLights[i])->ambientIntensity(0.0f);
        static_pointer_cast<PointLight>(mLights[i])->diffuseIntensity(1.0f);
        static_pointer_cast<PointLight>(mLights[i])->attenuation(SpotLight::ATTENUATION_TYPE::CONSTANT, 0.0f);
        static_pointer_cast<PointLight>(mLights[i])->attenuation(SpotLight::ATTENUATION_TYPE::LINEAR, 0.1f);
    }

    static_pointer_cast<PointLight>(mLights[1])->color(vec3f(0.0f, 1.0f, 0.0f));
    static_pointer_cast<PointLight>(mLights[1])->position(vec3f(-10.0f, 5.0f, -10.0f));

    static_pointer_cast<PointLight>(mLights[2])->color(vec3f(0.0f, 0.0f, 1.0f));
    static_pointer_cast<PointLight>(mLights[2])->position(vec3f(10.0f, 5.0f, -10.0f));

    static_pointer_cast<PointLight>(mLights[3])->color(vec3f(1.0f, 0.5f, 1.0f));
    static_pointer_cast<PointLight>(mLights[3])->position(vec3f(-10.0f, 5.0f, 10.0f));

    static_pointer_cast<PointLight>(mLights[4])->color(vec3f(1.0f, 0.8f, 0.23f));
    static_pointer_cast<PointLight>(mLights[4])->position(vec3f(10.0f, 5.0f, 10.0f));

    // Spot lights
    static_pointer_cast<SpotLight>(mLights[5])->ambientIntensity(0.1f);
    static_pointer_cast<SpotLight>(mLights[5])->diffuseIntensity(0.9f);
    static_pointer_cast<SpotLight>(mLights[5])->color(vec3f(1.0f, 1.0f, 1.0f));
    static_pointer_cast<SpotLight>(mLights[5])->position(vec3f(-30.0f, 35.0f, 17.0f));
    static_pointer_cast<SpotLight>(mLights[5])->direction(vec3f(0.73f, -0.64f, -0.24f));
    static_pointer_cast<SpotLight>(mLights[5])->attenuation(SpotLight::ATTENUATION_TYPE::LINEAR, 0.02f);
    static_pointer_cast<SpotLight>(mLights[5])->cutoff(40.0f);

    // Setup the camera
    mCamera->position(vec3f(0.0f, 19.0f, -37.0f));
    mCamera->lookAt(vec3f(0.0f, -0.34f, 0.94f));
    mCamera->up(vec3f(0.0f, 1.0f, 0.0f));

    // Configure meshes to illustrate the shadow with shadow map
    const auto lFloorMeshName = string("quad");
    auto lFloorMeshIterator = mMeshes.find(lFloorMeshName);

    if (lFloorMeshIterator != mMeshes.end())
    {
        lFloorMeshIterator->second.transform.emplace_back();
        lFloorMeshIterator->second.transform.back().scaling(20.0f, 20.0f, 20.0f);
        lFloorMeshIterator->second.transform.back().translation(0.0f, -1.0f, 1.0f);
        lFloorMeshIterator->second.transform.back().rotation(90.0f, 0.0f, 0.0f);
    }

    const auto lMeshName = string("spaceShip");
    auto lMeshIterator = mMeshes.find(lMeshName);

    if (lMeshIterator != mMeshes.end())
    {
        lMeshIterator->second.transform.emplace_back();
        lMeshIterator->second.transform.back().scaling(0.1f, 0.1f, 0.1f);
        lMeshIterator->second.transform.back().translation(5.0f, 0.0f, 3.0f);
    }

    // Create cube to illustrate bump mapping
    const auto lCubeMeshName = string("cube");
    auto lCubeMeshIterator = mMeshes.find(lCubeMeshName);

    if (lCubeMeshIterator != mMeshes.end())
    {
        lCubeMeshIterator->second.transform.emplace_back();
        lCubeMeshIterator->second.transform.back().translation(7.0f, 3.0f, -6.0f);
    }

    const string lJeepMeshName("jeep");
    auto lJeepMeshIterator = mMeshes.find(lJeepMeshName);

    if (lJeepMeshIterator != mMeshes.end())
    {
        lJeepMeshIterator->second.transform.emplace_back();
        lJeepMeshIterator->second.transform.back().translation(-9.0f, 0.5f, 6.0f);
        lJeepMeshIterator->second.transform.back().scaling(0.01f, 0.01f, 0.01f);
    }

    const string lHelicopterMeshName("helicopter");
    auto lHelicopterMeshIterator = mMeshes.find(lHelicopterMeshName);

    if (lHelicopterMeshIterator != mMeshes.end())
    {
        lHelicopterMeshIterator->second.transform.emplace_back();
        lHelicopterMeshIterator->second.transform.back().translation(-6.0f, 4.0f, -9.0f);
        lHelicopterMeshIterator->second.transform.back().scaling(0.04f, 0.04f, 0.04f);
    }

    // Create and initialize the simple lighting technique
    mSimpleLightingWithShadow = make_unique<SimpleLightingWithShadow>();
    mSimpleLightingWithShadow->init(4u, mWindow->frameBufferDimensions());
    mSimpleLightingWithShadow->camera(mCamera);
    mSimpleLightingWithShadow->floor(lFloorMeshName);
    mSimpleLightingWithShadow->addMeshToRender(lMeshName);
    mSimpleLightingWithShadow->addMeshToRender(lCubeMeshName);
    mSimpleLightingWithShadow->addMeshToRender(lJeepMeshName);
    mSimpleLightingWithShadow->addMeshToRender(lHelicopterMeshName);
    mSimpleLightingWithShadow->lightToUseDuringRender(1, 2, 3, 4, 5);
}

void Application::_initSkybox(void)
{
    // Display the name of the technique that is initialized
    Log::consoleMessage("Skybox", Log::EDecoration::TITLE);

    // Display the state of some configuration parameters of openGL
    GLUtils::currentState();

    // Setup the camera
    mCamera->position(vec3f(0.0f, 14.0f, -93.0f));
    mCamera->lookAt(vec3f(0.03f, -0.1f, 1.0f));
    mCamera->up(vec3f(0.0f, 1.0f, 0.0f));

    mSkyBox = make_unique<SkyBox>(mCamera);
    mSkyBox->init({"./sp3right.jpg", "./sp3left.jpg", "./sp3top.jpg", "./sp3bot.jpg", "./sp3front.jpg", "./sp3back.jpg"});
}

void Application::_initBillboarding(void)
{
    // Display the name of the technique that is initialized
    Log::consoleMessage("Billboarding", Log::EDecoration::TITLE);

    // Display the state of some configuration parameters of openGL
    GLUtils::currentState();

    // Setup the camera
    mCamera->position(vec3f(21.0f, 7.0f, -20.0f));
    mCamera->lookAt(vec3f(0.03f, -0.1f, 1.0f));
    mCamera->up(vec3f(0.0f, 1.0f, 0.0f));

    mBillboardList = make_unique<BillboardList>();
    mBillboardList->init(string(R"(./monster_hellknight.jpg)"), vec3f({20.0f, 0.0f, -6.0f}), 4, 4, 7.0f);
    mBillboardList->camera(mCamera);
}

void Application::_initParticleSystem(void)
{
    // Display the name of the technique that is initialized
    Log::consoleMessage("Particle System", Log::EDecoration::TITLE);

    // Display the state of some configuration parameters of openGL
    GLUtils::currentState();

    // Setup the camera
    mCamera->position(vec3f(6.0f, 5.0f, -8.0f));
    mCamera->lookAt(vec3f(0.03f, -0.1f, 1.0f));
    mCamera->up(vec3f(0.0f, 1.0f, 0.0f));

    mParticleSystem = make_unique<ParticleSystem>();
    mParticleSystem->init(500, vec3f(8.0f, 0.0f, 7.0f));

    mCurrentTime = high_resolution_clock::now();
}

void Application::_init3DPicking(void)
{
    // Display the name of the technique that is initialized
    Log::consoleMessage("3D picking", Log::EDecoration::TITLE);

    // Display the state of some configuration parameters of openGL
    GLUtils::currentState();

    // Point lights (the position are set at render time to move the point lights around)
    static_pointer_cast<PointLight>(mLights[1])->ambientIntensity(0.0f);
    static_pointer_cast<PointLight>(mLights[1])->diffuseIntensity(1.0f);
    static_pointer_cast<PointLight>(mLights[1])->color(vec3f(0.0f, 1.0f, 0.0f));
    static_pointer_cast<PointLight>(mLights[1])->attenuation(SpotLight::ATTENUATION_TYPE::LINEAR, 0.1f);

    static_pointer_cast<PointLight>(mLights[2])->ambientIntensity(0.0f);
    static_pointer_cast<PointLight>(mLights[2])->diffuseIntensity(1.0f);
    static_pointer_cast<PointLight>(mLights[2])->color(vec3f(0.0f, 0.0f, 1.0f));
    static_pointer_cast<PointLight>(mLights[2])->attenuation(SpotLight::ATTENUATION_TYPE::LINEAR, 0.1f);

    static_pointer_cast<PointLight>(mLights[3])->ambientIntensity(0.0f);
    static_pointer_cast<PointLight>(mLights[3])->diffuseIntensity(1.0f);
    static_pointer_cast<PointLight>(mLights[3])->color(vec3f(1.0f, 0.5f, 1.0f));
    static_pointer_cast<PointLight>(mLights[3])->attenuation(SpotLight::ATTENUATION_TYPE::LINEAR, 0.1f);

    static_pointer_cast<PointLight>(mLights[4])->ambientIntensity(0.0f);
    static_pointer_cast<PointLight>(mLights[4])->diffuseIntensity(1.0f);
    static_pointer_cast<PointLight>(mLights[4])->color(vec3f(1.0f, 0.8f, 0.23f));
    static_pointer_cast<PointLight>(mLights[4])->attenuation(SpotLight::ATTENUATION_TYPE::LINEAR, 0.1f);

    // Spot lights
    /*! \bug Shadows are not computed correctly with a spot light and multiple point lights */
    static_pointer_cast<SpotLight>(mLights[5])->ambientIntensity(0.0f);
    static_pointer_cast<SpotLight>(mLights[5])->diffuseIntensity(1.0f); // Not using the spot light
    static_pointer_cast<SpotLight>(mLights[5])->color(vec3f(1.0f, 1.0f, 1.0f));
    static_pointer_cast<SpotLight>(mLights[5])->position(vec3f(-10.0f, 15.0f, -10.0f));
    static_pointer_cast<SpotLight>(mLights[5])->direction(vec3f(0.21f, -1.0f, 0.34f));
    static_pointer_cast<SpotLight>(mLights[5])->attenuation(SpotLight::ATTENUATION_TYPE::LINEAR, 0.01f);
    static_pointer_cast<SpotLight>(mLights[5])->cutoff(40.0f);

    // Setup the camera
    mCamera->position(vec3f(-2.0f, 7.0f, -23.0f));
    mCamera->lookAt(vec3f(0.03f, -0.1f, 1.0f));
    mCamera->up(vec3f(0.0f, 1.0f, 0.0f));

    const auto lSpiderMeshName = string("spider");
    auto lSpiderMeshIterator = mMeshes.find(lSpiderMeshName);

    if (lSpiderMeshIterator != mMeshes.end())
    {
        Transform lTransform;
        lTransform.scaling(0.02f, 0.02f, 0.02f);

        const degreef lRotationY = 90.0f;
        lTransform.rotation(0.0f, lRotationY, 0.0f);
        lTransform.translation(-6.0f, 0.8f, -5.0f);

        lSpiderMeshIterator->second.transform.push_back(lTransform);

        lTransform.translation(-2.0f, 0.8f, -5.0f);
        lSpiderMeshIterator->second.transform.push_back(lTransform);
    }

    // Configure meshes to illustrate the shadow with shadow map
    const auto lFloorMeshName = string("quad");
    auto lFloorMeshIterator = mMeshes.find(lFloorMeshName);

    if (lFloorMeshIterator != mMeshes.end())
    {
        lFloorMeshIterator->second.transform.emplace_back();
        lFloorMeshIterator->second.transform.back().scaling(20.0f, 20.0f, 20.0f);
        lFloorMeshIterator->second.transform.back().translation(0.0f, 0.0f, 1.0f);
        lFloorMeshIterator->second.transform.back().rotation(90.0f, 0.0f, 0.0f);
    }

    mPicking3D = make_unique<Picking3D>();
    mPicking3D->init(mWindow->frameBufferDimensions(), mWindow->windowDimensions());
    mPicking3D->camera(mCamera);
    mPicking3D->meshToRender(lSpiderMeshName);

    mSimpleLightingWithShadow = make_unique<SimpleLightingWithShadow>();
    mSimpleLightingWithShadow->init(4u, mWindow->frameBufferDimensions());
    mSimpleLightingWithShadow->camera(mCamera);
    mSimpleLightingWithShadow->floor(lFloorMeshName);
    mSimpleLightingWithShadow->addMeshToRender(lSpiderMeshName);
    mSimpleLightingWithShadow->lightToUseDuringRender(1, 2, 3, 4, 5);

    mPickingOn = true;
}

void Application::_initTessellation(void)
{
    // Display the name of the technique that is initialized
    Log::consoleMessage("Tessellation", Log::EDecoration::TITLE);

    // Display the state of some configuration parameters of openGL
    GLUtils::currentState();

    _checkLightsType();

    static_pointer_cast<PointLight>(mLights[1])->ambientIntensity(0.1f);
    static_pointer_cast<PointLight>(mLights[1])->diffuseIntensity(1.0f);
    static_pointer_cast<PointLight>(mLights[1])->color(vec3f(0.35f, 0.64f, 0.82f));
    static_pointer_cast<PointLight>(mLights[1])->position(vec3f(-25.0f, 7.0f, 0.0f));
    static_pointer_cast<PointLight>(mLights[1])->attenuation(SpotLight::ATTENUATION_TYPE::LINEAR, 0.001f);

    // Setup the camera
    mCamera->position(vec3f(-25.0f, 7.0f, -20.0f));
    mCamera->lookAt(vec3f(0.03f, -0.1f, 1.0f));
    mCamera->up(vec3f(0.0f, 1.0f, 0.0f));

    const auto lMeshName = string("terrain");
    auto lMeshIterator = mMeshes.find(lMeshName);

    if (lMeshIterator != mMeshes.end())
    {
        Transform lTransform;
        lTransform.scaling(10.0f, 10.0f, 10.0f);
        lTransform.translation(-25.0f, 0.0f, 0.0f);

        lMeshIterator->second.transform.push_back(lTransform);
    }

    mTessellation = make_unique<Tessellation>();
    mTessellation->init(1u, string(R"(./heightmap.jpg)"));
    mTessellation->camera(mCamera);
    mTessellation->addMeshToRender(lMeshName);
    mTessellation->lightToUseDuringRender(1);

//    _validateShaderWithMesh(mTessellationLighting, lName);
}

void Application::_initTessellationPN(void)
{
    // Display the name of the technique that is initialized
    Log::consoleMessage("Tessellation Point Normal", Log::EDecoration::TITLE);

    // Display the state of some configuration parameters of openGL
    GLUtils::currentState();

    _checkLightsType();

    // Set up directional light
    static_pointer_cast<DirectionalLight>(mLights[0])->color(vec3f(0.9f, 0.9f, 0.0f));
    static_pointer_cast<DirectionalLight>(mLights[0])->ambientIntensity(0.1f);
    static_pointer_cast<DirectionalLight>(mLights[0])->diffuseIntensity(1.0f);
    static_pointer_cast<DirectionalLight>(mLights[0])->direction(vec3f(0.65f, -0.41f, -0.64f));

    // Set up point lights
    static_pointer_cast<PointLight>(mLights[1])->ambientIntensity(0.0f);
    static_pointer_cast<PointLight>(mLights[1])->diffuseIntensity(1.0f);
    static_pointer_cast<PointLight>(mLights[1])->color(vec3f(0.3f, 0.8f, 1.0f));
    static_pointer_cast<PointLight>(mLights[1])->position(vec3f(-12.0f, 7.0f, 5.0f));
    static_pointer_cast<PointLight>(mLights[1])->attenuation(SpotLight::ATTENUATION_TYPE::CONSTANT, 0.0f);
    static_pointer_cast<PointLight>(mLights[1])->attenuation(SpotLight::ATTENUATION_TYPE::LINEAR, 0.1f);

    static_pointer_cast<PointLight>(mLights[2])->ambientIntensity(0.0f);
    static_pointer_cast<PointLight>(mLights[2])->diffuseIntensity(1.0f);
    static_pointer_cast<PointLight>(mLights[2])->color(vec3f(0.9f, 0.5f, 1.0f));
    static_pointer_cast<PointLight>(mLights[2])->position(vec3f(11.0f, 12.0f, 12.0f));
    static_pointer_cast<PointLight>(mLights[2])->attenuation(SpotLight::ATTENUATION_TYPE::CONSTANT, 0.0f);
    static_pointer_cast<PointLight>(mLights[2])->attenuation(SpotLight::ATTENUATION_TYPE::LINEAR, 0.1f);

    // Setup the camera
    mCamera->position(vec3f(-4.0f, 16.0f, -18.0f));
    mCamera->lookAt(vec3f(0.0f, -0.1f, 1.0f));
    mCamera->up(vec3f(0.0f, 1.0f, 0.0f));

    const auto lMeshName = string("monkey");
    auto lMeshIterator = mMeshes.find(lMeshName);

    if (lMeshIterator != mMeshes.end())
    {
        Transform lTransform;
        lTransform.scaling(3.0f, 3.0f, 3.0f);
        lTransform.translation(-5.0f, 15.0f, 0.0f);
        lTransform.rotation(-90.0f, 0.0f, 0.0f);

        lMeshIterator->second.transform.push_back(lTransform);

        lTransform.translation(5.0f, 15.0f, 0.0f);

        lMeshIterator->second.transform.push_back(lTransform);
    }

    mTessellationPN = make_unique<TessellationPN>();
    mTessellationPN->init(2u);
    mTessellationPN->camera(mCamera);
    mTessellationPN->addMeshToRender(lMeshName);
    mTessellationPN->addTessellationLevel(1.1f);
    mTessellationPN->addTessellationLevel(1.1f);
    mTessellationPN->lightToUseDuringRender(0, 1, 2);

//    _validateShaderWithMesh(mTessellationPNLighting, lName);
}

void Application::_initInstancedRendering(void)
{
    // Display the name of the technique that is initialized
    Log::consoleMessage("Instanced Rendering", Log::EDecoration::TITLE);

    // Display the state of some configuration parameters of openGL
    GLUtils::currentState();

    _checkLightsType();

    // Set up directional light
    static_pointer_cast<DirectionalLight>(mLights[0])->color(vec3f(1.0f, 0.0f, 1.0f));
    static_pointer_cast<DirectionalLight>(mLights[0])->ambientIntensity(0.2f);
    static_pointer_cast<DirectionalLight>(mLights[0])->diffuseIntensity(0.8f);
    static_pointer_cast<DirectionalLight>(mLights[0])->direction(vec3f({1.0f, -1.0f, 0.0f}));

    // Set up point lights
    static_pointer_cast<PointLight>(mLights[1])->ambientIntensity(0.2f);
    static_pointer_cast<PointLight>(mLights[1])->diffuseIntensity(1.0f);
    static_pointer_cast<PointLight>(mLights[1])->color(vec3f(1.0f, 0.4f, 0.4f));
    static_pointer_cast<PointLight>(mLights[1])->position(vec3f(30.0f, 15.5f, -20.0f));
    static_pointer_cast<PointLight>(mLights[1])->attenuation(SpotLight::ATTENUATION_TYPE::LINEAR, 0.01f);

    static_pointer_cast<PointLight>(mLights[2])->ambientIntensity(0.2f);
    static_pointer_cast<PointLight>(mLights[2])->diffuseIntensity(1.0f);
    static_pointer_cast<PointLight>(mLights[2])->color(vec3f(1.0f, 0.88f, 0.8f));
    static_pointer_cast<PointLight>(mLights[2])->position(vec3f(35.0f, 15.5f, -15.0f));
    static_pointer_cast<PointLight>(mLights[2])->attenuation(SpotLight::ATTENUATION_TYPE::LINEAR, 0.01f);

    // Setup the camera
    mCamera->position(vec3f(45.0f, 8.0f, -4.0f));
    mCamera->lookAt(vec3f(0.14f, -0.27f, 0.95f));
    mCamera->up(vec3f(0.0f, 1.0f, 0.0f));

    const auto lMeshName = string("spider - instanced rendering");
    auto lMeshIterator = mMeshes.find(lMeshName);

    if (lMeshIterator != mMeshes.end())
    {
        Transform lTransform;
        lTransform.scaling(0.005f, 0.005f, 0.005f);
        lTransform.rotation(0.0f, 90.0f, 0.0f);

        lMeshIterator->second.transform.push_back(lTransform);
    }

//    _validateShaderWithMesh(mInstancedLighting, lName);

    // Compute the position of the different instances
    const unsigned int lRows = 50;
    const unsigned int lColumns = 20;
    vector<vec3f> lInstancePositions(lRows * lColumns, vec3f(0.0f));
    vector<vec3f> lInstanceVelocities(lRows * lColumns, vec3f(0.0f));

    default_random_engine lGenerator;
    lGenerator.seed(static_cast<unsigned int>(system_clock::now().time_since_epoch().count()));

    uniform_real_distribution<float> lDistribution(0, 1.0f);

    const vec3f lOrigin(40.0f, 2.0f, 0.0f);

    for (unsigned int i = 0; i < lRows; ++i)
    {
        for (unsigned int j = 0; j < lColumns; ++j)
        {
            const unsigned int lIndex = i * lColumns + j;

            lInstancePositions[lIndex].x() = lOrigin.x() + static_cast<float>(j);
            lInstancePositions[lIndex].y() = lOrigin.y() + lDistribution(lGenerator) * 5.0f;
            lInstancePositions[lIndex].z() = lOrigin.z() + static_cast<float>(i);

            // Set the velocity only on the y axis
            lInstanceVelocities[lIndex].y() = lDistribution(lGenerator);

            // Velocities with odd indices are a negative
            if ((i & 1) == 1)
                lInstanceVelocities[lIndex].y() *= -1.0f;
        }
    }

    mInstancedLighting = make_unique<InstancedLightingTechnique>();
    mInstancedLighting->init(2u, mWindow->frameBufferDimensions());
    mInstancedLighting->camera(mCamera);
    mInstancedLighting->addMeshToRender(lMeshName);
    mInstancedLighting->instancePositions(lInstancePositions);
    mInstancedLighting->instanceVelocities(lInstanceVelocities);
    mInstancedLighting->lightToUseDuringRender(0, 1, 2);
}

void Application::_initGLFXExample(void)
{
    // Display the name of the technique that is initialized
    Log::consoleMessage("GLFX", Log::EDecoration::TITLE);

    // Display the state of some configuration parameters of openGL
    GLUtils::currentState();

    _checkLightsType();

    // Set up directional light
    static_pointer_cast<DirectionalLight>(mLights[0])->color(vec3f(1.0f, 1.0f, 1.0f));
    static_pointer_cast<DirectionalLight>(mLights[0])->ambientIntensity(0.0f);
    static_pointer_cast<DirectionalLight>(mLights[0])->diffuseIntensity(1.0f);
    static_pointer_cast<DirectionalLight>(mLights[0])->direction(vec3f(0.67f, -0.79f, -0.97f));

    // Set up point lights
    for (unsigned int i = 1; i < 4; ++i)
    {
        static_pointer_cast<PointLight>(mLights[i])->ambientIntensity(0.0f);
        static_pointer_cast<PointLight>(mLights[i])->diffuseIntensity(1.0f);
        static_pointer_cast<PointLight>(mLights[i])->attenuation(SpotLight::ATTENUATION_TYPE::CONSTANT, 0.0f);
        static_pointer_cast<PointLight>(mLights[i])->attenuation(SpotLight::ATTENUATION_TYPE::LINEAR, 0.1f);
    }

    static_pointer_cast<PointLight>(mLights[1])->color(vec3f(0.7f, 1.0f, 0.7f));
    static_pointer_cast<PointLight>(mLights[1])->position(vec3f(-2.5f, 13.0f, -2.0f));

    static_pointer_cast<PointLight>(mLights[2])->color(vec3f(1.0f, 0.2f, 1.0f));
    static_pointer_cast<PointLight>(mLights[2])->position(vec3f(2.5f, 13.0f, -2.0f));

    static_pointer_cast<PointLight>(mLights[3])->color(vec3f(0.0f, 0.77f, 0.77f));
    static_pointer_cast<PointLight>(mLights[3])->position(vec3f(11.f, 13.0f, -2.0f));

    // Setup the camera
    mCamera->position(vec3f(-4.0f, 14.0f, -10.0f));
    mCamera->lookAt(vec3f(0.14f, -0.27f, 0.95f));
    mCamera->up(vec3f(0.0f, 1.0f, 0.0f));

    array<vec3f, 4> lPositions = {{vec3f(-5.0f, 10.0f, 6.0f), vec3f( 0.0f, 10.0f, 6.0f), vec3f( 5.0f, 10.0f, 6.0f), vec3f( 10.0f, 10.0f, 6.0f)}};
    array<vec3f, 4> lColors = {{vec3f(1.0f, 0.0f, 0.0f), vec3f(0.0f, 1.0f, 0.0f), vec3f(0.0f, 0.0f, 1.0f), vec3f(0.7f, 0.7f, 0.0f)}};
    array<string, 4> lMeshNames = {{string("letter g"), string("letter l"), string("letter f"), string("letter x")}};

    mGLFXTechnique = make_unique<GLFXTechnique>();
    mGLFXTechnique->init(3u, 0u);
    mGLFXTechnique->camera(mCamera);
    mGLFXTechnique->lightToUseDuringRender(0, 1, 2, 3);

    for (size_t i = 0; i < lPositions.size(); ++i)
    {
        auto lMeshIterator = mMeshes.find(lMeshNames[i]);

        if (lMeshIterator != mMeshes.end())
        {
            Transform lTransform;
            lTransform.scaling(0.2f, 0.2f, 0.2f);
            lTransform.translation(lPositions[i].x(), lPositions[i].y(), lPositions[i].z());

            lMeshIterator->second.transform.push_back(lTransform);

            mGLFXTechnique->addMeshToRender(lMeshNames[i]);
            mGLFXTechnique->addUniformColor(lColors[i].x(), lColors[i].y(), lColors[i].z());
        }
    }
}

void Application::_initDeferredShading(void)
{
    // Display the name of the technique that is initialized
    Log::consoleMessage("Deferred Shading", Log::EDecoration::TITLE);

    // Display the state of some configuration parameters of openGL
    GLUtils::currentState();

    _checkLightsType();

    // Set up directional light
    static_pointer_cast<DirectionalLight>(mLights[0])->color(vec3f(0.0f, 1.0f, 1.0f));
    static_pointer_cast<DirectionalLight>(mLights[0])->ambientIntensity(0.1f);
    static_pointer_cast<DirectionalLight>(mLights[0])->diffuseIntensity(0.5f);
    static_pointer_cast<DirectionalLight>(mLights[0])->direction(vec3f(0.0f, 0.0f, 1.0f));

    // Set up point lights
    // Same attenuation parameters for all the point lights
    for (unsigned int i = 1; i < 4; ++i)
    {
        static_pointer_cast<PointLight>(mLights[i])->ambientIntensity(0.0f);
        static_pointer_cast<PointLight>(mLights[i])->diffuseIntensity(0.9f);
        static_pointer_cast<PointLight>(mLights[i])->attenuation(SpotLight::ATTENUATION_TYPE::CONSTANT, 0.0f);
        static_pointer_cast<PointLight>(mLights[i])->attenuation(SpotLight::ATTENUATION_TYPE::LINEAR, 0.0f);
        static_pointer_cast<PointLight>(mLights[i])->attenuation(SpotLight::ATTENUATION_TYPE::EXPONENTIAL, 0.3f);
    }

    static_pointer_cast<PointLight>(mLights[1])->color(vec3f(1.0f, 0.0f, 1.0f));
    static_pointer_cast<PointLight>(mLights[1])->position(vec3f(0.0f, 1.5f, 5.0f));

    static_pointer_cast<PointLight>(mLights[2])->color(vec3f(1.0f, 0.0f, 0.0f));
    static_pointer_cast<PointLight>(mLights[2])->position(vec3f(2.0f, 0.0f, 5.0f));

    static_pointer_cast<PointLight>(mLights[3])->color(vec3f(1.0f, 1.0f, 0.0f));
    static_pointer_cast<PointLight>(mLights[3])->position(vec3f(0.0f, 0.0f, 3.0f));

    // Set up spot lights
    static_pointer_cast<SpotLight>(mLights[5])->ambientIntensity(0.0f);
    static_pointer_cast<SpotLight>(mLights[5])->diffuseIntensity(0.9f);
    static_pointer_cast<SpotLight>(mLights[5])->color(vec3f(1.0f, 0.51f, 0.0f));
    static_pointer_cast<SpotLight>(mLights[5])->position(vec3f(3.0f, 4.0f, 10.0f));
    static_pointer_cast<SpotLight>(mLights[5])->direction(vec3f(1.0f, -1.0f, 0.0f));
    static_pointer_cast<SpotLight>(mLights[5])->attenuation(SpotLight::ATTENUATION_TYPE::CONSTANT, 0.0f);
    static_pointer_cast<SpotLight>(mLights[5])->attenuation(SpotLight::ATTENUATION_TYPE::LINEAR, 0.0f);
    static_pointer_cast<SpotLight>(mLights[5])->attenuation(SpotLight::ATTENUATION_TYPE::EXPONENTIAL, 0.3f);
    static_pointer_cast<SpotLight>(mLights[5])->cutoff(40.0f);

    // Setup the camera
    mCamera->position(vec3f(-3.5f, 7.4f, -4.68f));
    mCamera->lookAt(vec3f(0.04f, -0.4f, 0.92f));
    mCamera->up(vec3f(0.0f, 1.0f, 0.0f));

//    _validateShaderWithMesh(mDSGeometryPass,string("box"));

    const auto lMeshName = string("box");
    auto lMeshIterator = mMeshes.find(lMeshName);

    if (lMeshIterator != mMeshes.end())
    {
        Transform lTransform;

        lTransform.translation(0.0f, 0.0f, 5.0f);
        lMeshIterator->second.transform.push_back(lTransform);

        lTransform.translation(6.0f, 1.0f, 10.0f);
        lMeshIterator->second.transform.push_back(lTransform);

        lTransform.translation(-5.0f, -1.0f, 12.0f);
        lMeshIterator->second.transform.push_back(lTransform);

        lTransform.translation(4.0f, 4.0f, 15.0f);
        lMeshIterator->second.transform.push_back(lTransform);

        lTransform.translation(-4.0f, 2.0f, 20.0f);
        lMeshIterator->second.transform.push_back(lTransform);
    }

    mDeferredShading = make_unique<DeferredShadingTechnique>();
    mDeferredShading->init(3, 1, mWindow->frameBufferDimensions());
    mDeferredShading->camera(mCamera);
    mDeferredShading->addMeshToRender(lMeshName);
}

void Application::_initSkeletalAnimation(void)
{
    // Display the name of the technique that is initialized
    Log::consoleMessage("Skeletal Animation", Log::EDecoration::TITLE);

    // Display the state of some configuration parameters of openGL
    GLUtils::currentState();

    _checkLightsType();

    // Set up directional light
    static_pointer_cast<DirectionalLight>(mLights[0])->color(vec3f(1.0f));
    static_pointer_cast<DirectionalLight>(mLights[0])->ambientIntensity(0.2f);
    static_pointer_cast<DirectionalLight>(mLights[0])->diffuseIntensity(0.8f);
    static_pointer_cast<DirectionalLight>(mLights[0])->direction(vec3f({1.0f, -1.0f, 0.0f}));

    // Set up point lights
    static_pointer_cast<PointLight>(mLights[1])->ambientIntensity(0.1f);
    static_pointer_cast<PointLight>(mLights[1])->diffuseIntensity(0.9f);
    static_pointer_cast<PointLight>(mLights[1])->color(vec3f(1.0f, 0.5f, 0.0f));
    static_pointer_cast<PointLight>(mLights[1])->position(vec3f(3.0f, 1.0f, 0.0f));
    static_pointer_cast<PointLight>(mLights[1])->attenuation(SpotLight::ATTENUATION_TYPE::LINEAR, 0.1f);

    static_pointer_cast<PointLight>(mLights[2])->ambientIntensity(0.1f);
    static_pointer_cast<PointLight>(mLights[2])->diffuseIntensity(0.9f);
    static_pointer_cast<PointLight>(mLights[2])->color(vec3f(0.0f, 0.5f, 1.0f));
    static_pointer_cast<PointLight>(mLights[2])->position(vec3f(-3.0f, 1.0f, 0.0f));
    static_pointer_cast<PointLight>(mLights[2])->attenuation(SpotLight::ATTENUATION_TYPE::LINEAR, 0.1f);

    // Set up spot lights
    static_pointer_cast<SpotLight>(mLights[5])->ambientIntensity(0.1f);
    static_pointer_cast<SpotLight>(mLights[5])->diffuseIntensity(0.9f);
    static_pointer_cast<SpotLight>(mLights[5])->color(vec3f(1.0f, 1.0f, 1.0f));
    static_pointer_cast<SpotLight>(mLights[5])->position(vec3f(-20.0f, 20.0f, 1.0f));
    static_pointer_cast<SpotLight>(mLights[5])->direction(vec3f(1.0f, -1.0f, 0.0f));
    static_pointer_cast<SpotLight>(mLights[5])->attenuation(SpotLight::ATTENUATION_TYPE::LINEAR, 0.01f);
    static_pointer_cast<SpotLight>(mLights[5])->cutoff(20.0f);

    // Setup the camera
    mCamera->position(vec3f(-1.5f, 5.0f, -4.0f));
    mCamera->lookAt(vec3f(0.03f, -0.1f, 0.99f));
    mCamera->up(vec3f(0.0f, 1.0f, 0.0f));

    const auto lMeshName = string("bobLamp");
    auto lMeshIterator = mMeshes.find(lMeshName);

    if (lMeshIterator != mMeshes.end())
    {
        Transform lTransform;
        lTransform.scaling(0.1f, 0.1f, 0.1f);
        lTransform.translation(0.0f, 0.0f, 6.0f);
        lTransform.rotation(270.0f, 180.0f, 0.0f);

        lMeshIterator->second.transform.push_back(lTransform);
    }

    mSkinningTechnique = make_unique<SkinningTechnique>();
    mSkinningTechnique->init(2, 1, false);
    mSkinningTechnique->camera(mCamera);
    mSkinningTechnique->addMeshToRender(lMeshName);
    mSkinningTechnique->lightToUseDuringRender(0, 1, 2, 5);
}

void Application::_initSilhouetteDetection(void)
{
    // Display the name of the technique that is initialized
    Log::consoleMessage("Silhouette Detection", Log::EDecoration::TITLE);

    // Display the state of some configuration parameters of openGL
    GLUtils::currentState();

    _checkLightsType();

    // Point lights
    for (unsigned int i = 1; i < 5; ++i)
    {
        static_pointer_cast<PointLight>(mLights[i])->ambientIntensity(0.0f);
        static_pointer_cast<PointLight>(mLights[i])->diffuseIntensity(1.0f);
        static_pointer_cast<PointLight>(mLights[i])->attenuation(SpotLight::ATTENUATION_TYPE::CONSTANT, 0.0f);
        static_pointer_cast<PointLight>(mLights[i])->attenuation(SpotLight::ATTENUATION_TYPE::LINEAR, 0.2f);
    }

    static_pointer_cast<PointLight>(mLights[1])->color(vec3f(0.0f, 1.0f, 0.0f));
    static_pointer_cast<PointLight>(mLights[1])->position(vec3f(-10.0f, 3.0f, -10.0f));

    static_pointer_cast<PointLight>(mLights[2])->color(vec3f(0.0f, 0.0f, 1.0f));
    static_pointer_cast<PointLight>(mLights[2])->position(vec3f(10.0f, 3.0f, -10.0f));

    static_pointer_cast<PointLight>(mLights[3])->color(vec3f(1.0f, 0.5f, 1.0f));
    static_pointer_cast<PointLight>(mLights[3])->position(vec3f(-10.0f, 3.0f, 10.0f));

    static_pointer_cast<PointLight>(mLights[4])->color(vec3f(1.0f, 0.8f, 0.23f));
    static_pointer_cast<PointLight>(mLights[4])->position(vec3f(10.0f, 3.0f, 10.0f));

    // Spot lights
    static_pointer_cast<SpotLight>(mLights[5])->ambientIntensity(0.0f);
    static_pointer_cast<SpotLight>(mLights[5])->diffuseIntensity(1.0f);
    static_pointer_cast<SpotLight>(mLights[5])->color(vec3f(1.0f, 1.0f, 1.0f));
    static_pointer_cast<SpotLight>(mLights[5])->position(vec3f(0.0f, 10.0f, 0.0f));
    static_pointer_cast<SpotLight>(mLights[5])->direction(vec3f(0.58f, -0.78f, -0.24f));
    static_pointer_cast<SpotLight>(mLights[5])->attenuation(SpotLight::ATTENUATION_TYPE::CONSTANT, 0.0f);
    static_pointer_cast<SpotLight>(mLights[5])->attenuation(SpotLight::ATTENUATION_TYPE::LINEAR, 0.13f);
    static_pointer_cast<SpotLight>(mLights[5])->cutoff(30.0f);

    // Setup the camera
    mCamera->position(vec3f(0.0f, 12.0f, -16.0f));
    mCamera->lookAt(vec3f(0.14f, -0.55f, 0.83f));
    mCamera->up(vec3f(0.0f, 1.0f, 0.0f));

    // Common parameters to both cubes
    const float lCubeTranslationX = 7.0f, lCubeTranslationY = 3.0f, lTranslationZ = -6.0f;

    // Create normal cube to be renderer with simple lighting with shadows
    const auto lGraphicsCubeMeshName = string("cube");
    auto lGraphicsCubeMeshIterator = mMeshes.find(lGraphicsCubeMeshName);

    if (lGraphicsCubeMeshIterator != mMeshes.end())
    {
        lGraphicsCubeMeshIterator->second.transform.emplace_back();
        lGraphicsCubeMeshIterator->second.transform.back().translation(lCubeTranslationX, lCubeTranslationY, lTranslationZ);
    }

    // Create cube with adjacencies to be able to compute the silhouette in silhouette technique
    const auto lCubeMeshName = string("cubeWithAdjacencies");
    auto lCubeMeshIterator = mMeshes.find(lCubeMeshName);

    if (lCubeMeshIterator != mMeshes.end())
    {
        lCubeMeshIterator->second.transform.emplace_back();
        lCubeMeshIterator->second.transform.back().translation(lCubeTranslationX, lCubeTranslationY, lTranslationZ);
    }

    // Simple lighting with shadows REQUIRES a floor
    const auto lFloorMeshName = string("quad");
    auto lFloorMeshIterator = mMeshes.find(lFloorMeshName);

    if (lFloorMeshIterator != mMeshes.end())
    {
        lFloorMeshIterator->second.transform.emplace_back();
        lFloorMeshIterator->second.transform.back().scaling(20.0f, 20.0f, 20.0f);
        lFloorMeshIterator->second.transform.back().translation(0.0f, -1.0f, 1.0f);
        lFloorMeshIterator->second.transform.back().rotation(90.0f, 0.0f, 0.0f);
    }

    // Create and initialize the simple lighting technique
    mSimpleLightingWithShadow = make_unique<SimpleLightingWithShadow>();
    mSimpleLightingWithShadow->init(4u, mWindow->frameBufferDimensions());
    mSimpleLightingWithShadow->camera(mCamera);
    mSimpleLightingWithShadow->addMeshToRender(lGraphicsCubeMeshName);
    mSimpleLightingWithShadow->floor(lFloorMeshName);
    mSimpleLightingWithShadow->useShadowMap(false);
    mSimpleLightingWithShadow->lightToUseDuringRender(1, 2, 3, 4, 5);

    // Create and initialise the silhouette technique
    mSilhouetteTechnique = make_unique<SilhouetteTechnique>();
    mSilhouetteTechnique->init();
    mSilhouetteTechnique->addMeshToRender(lCubeMeshName);
    mSilhouetteTechnique->camera(mCamera);
    mSilhouetteTechnique->lightIndex(4);
}

void Application::_initShadowVolume(void)
{
    // Display the name of the technique that is initialized
    Log::consoleMessage("Shadow Volume", Log::EDecoration::TITLE);

    // Display the state of some configuration parameters of openGL
    GLUtils::currentState();

    _checkLightsType();

    // Configure only 1 point light
    static_pointer_cast<PointLight>(mLights[1])->ambientIntensity(0.0f);
    static_pointer_cast<PointLight>(mLights[1])->diffuseIntensity(0.9f);
    static_pointer_cast<PointLight>(mLights[1])->color(vec3f(1.0f, 1.0f, 0.0f));
    static_pointer_cast<PointLight>(mLights[1])->position(vec3f(0.0f, 10.0f, 0.0f));
    static_pointer_cast<PointLight>(mLights[1])->attenuation(SpotLight::ATTENUATION_TYPE::CONSTANT, 1.0f);
    static_pointer_cast<PointLight>(mLights[1])->attenuation(SpotLight::ATTENUATION_TYPE::LINEAR, 0.0f);

    // Setup the camera
    mCamera->position(vec3f(1.8f, 9.61f, -13.13f));
    mCamera->lookAt(vec3f(0.1f, -0.67f, 0.73f));
    mCamera->up(vec3f(0.0f, 1.0f, 0.0f));

    // Create cube to illustrate bump mapping
    const auto lCubeAdjacenciesMeshName = string("cubeWithAdjacencies");
    auto lCubeAdjacenciesMeshIterator = mMeshes.find(lCubeAdjacenciesMeshName);

    const float lCubeTranslationX = 7.0f, lCubeTranslationY = 3.0f, lTranslationZ = -6.0f;

    if (lCubeAdjacenciesMeshIterator != mMeshes.end())
    {
        lCubeAdjacenciesMeshIterator->second.transform.emplace_back();
        lCubeAdjacenciesMeshIterator->second.transform.back().translation(lCubeTranslationX, lCubeTranslationY, lTranslationZ);
    }

    // Create cube to illustrate bump mapping
    const auto lCubeMeshName = string("cube");
    auto lCubeMeshIterator = mMeshes.find(lCubeMeshName);

    if (lCubeMeshIterator != mMeshes.end())
    {
        lCubeMeshIterator->second.transform.emplace_back();
        lCubeMeshIterator->second.transform.back().translation(lCubeTranslationX, lCubeTranslationY, lTranslationZ);
    }

    // Configure two meshes to illustrate the shadow with shadow map
    const auto lFloorMeshName = string("quad");
    auto lFloorMeshIterator = mMeshes.find(lFloorMeshName);

    if (lFloorMeshIterator != mMeshes.end())
    {
        lFloorMeshIterator->second.transform.emplace_back();
        lFloorMeshIterator->second.transform.back().scaling(20.0f, 20.0f, 20.0f);
        lFloorMeshIterator->second.transform.back().translation(0.0f, -1.0f, 1.0f);
        lFloorMeshIterator->second.transform.back().rotation(90.0f, 0.0f, 0.0f);
    }

    // Create and initialize the shadow volume technique
    mShadowVolumeTechnique = make_unique<ShadowVolumeTechnique>();
    mShadowVolumeTechnique->init(1u, mWindow->frameBufferDimensions());
    mShadowVolumeTechnique->addMeshWithAdjacenciesToRender(lCubeAdjacenciesMeshName);
    mShadowVolumeTechnique->addMeshToRender(lCubeMeshName);
    mShadowVolumeTechnique->floor(lFloorMeshName);
    mShadowVolumeTechnique->camera(mCamera);
    mShadowVolumeTechnique->pointLightToUseDuringRender(1);
}

void Application::_initSkeletalAnimationWithMotionBlur(void)
{
    // Display the name of the technique that is initialized
    Log::consoleMessage("Skeletal Animation With Motion Blur", Log::EDecoration::TITLE);


    // Display the state of some configuration parameters of openGL
    GLUtils::currentState();

    _checkLightsType();

    // Set up directional light
    static_pointer_cast<DirectionalLight>(mLights[0])->color(vec3f(1.0f));
    static_pointer_cast<DirectionalLight>(mLights[0])->ambientIntensity(0.2f);
    static_pointer_cast<DirectionalLight>(mLights[0])->diffuseIntensity(0.8f);
    static_pointer_cast<DirectionalLight>(mLights[0])->direction(vec3f(1.0f, -1.0f, 0.0f));

    // Set up point lights
    static_pointer_cast<PointLight>(mLights[1])->ambientIntensity(0.1f);
    static_pointer_cast<PointLight>(mLights[1])->diffuseIntensity(0.9f);
    static_pointer_cast<PointLight>(mLights[1])->color(vec3f(1.0f, 0.5f, 0.0f));
    static_pointer_cast<PointLight>(mLights[1])->position(vec3f(3.0f, 1.0f, 0.0f));
    static_pointer_cast<PointLight>(mLights[1])->attenuation(SpotLight::ATTENUATION_TYPE::LINEAR, 0.1f);

    static_pointer_cast<PointLight>(mLights[2])->ambientIntensity(0.1f);
    static_pointer_cast<PointLight>(mLights[2])->diffuseIntensity(0.9f);
    static_pointer_cast<PointLight>(mLights[2])->color(vec3f(0.0f, 0.5f, 1.0f));
    static_pointer_cast<PointLight>(mLights[2])->position(vec3f(-3.0f, 1.0f, 0.0f));
    static_pointer_cast<PointLight>(mLights[2])->attenuation(SpotLight::ATTENUATION_TYPE::LINEAR, 0.1f);

    // Set up spot lights
    static_pointer_cast<SpotLight>(mLights[5])->ambientIntensity(0.1f);
    static_pointer_cast<SpotLight>(mLights[5])->diffuseIntensity(0.9f);
    static_pointer_cast<SpotLight>(mLights[5])->color(vec3f(1.0f, 1.0f, 1.0f));
    static_pointer_cast<SpotLight>(mLights[5])->position(vec3f(-20.0f, 20.0f, 1.0f));
    static_pointer_cast<SpotLight>(mLights[5])->direction(vec3f(1.0f, -1.0f, 0.0f));
    static_pointer_cast<SpotLight>(mLights[5])->attenuation(SpotLight::ATTENUATION_TYPE::LINEAR, 0.01f);
    static_pointer_cast<SpotLight>(mLights[5])->cutoff(20.0f);

    // Setup the camera
    mCamera->position(vec3f(-1.5f, 5.0f, -4.0f));
    mCamera->lookAt(vec3f(0.03f, -0.1f, 0.99f));
    mCamera->up(vec3f(0.0f, 1.0f, 0.0f));

    const auto lMeshName = string("bobLamp");
    auto lMeshIterator = mMeshes.find(lMeshName);

    if (lMeshIterator != mMeshes.end())
    {
        Transform lTransform;
        lTransform.scaling(0.1f, 0.1f, 0.1f);
        lTransform.translation(0.0f, 0.0f, 6.0f);
        lTransform.rotation(270.0f, 180.0f, 0.0f);

        lMeshIterator->second.transform.push_back(lTransform);
    }

    mSkinningTechnique = make_unique<SkinningTechnique>();
    mSkinningTechnique->init(2, 1, true, mWindow->frameBufferDimensions());
    mSkinningTechnique->camera(mCamera);
    mSkinningTechnique->addMeshToRender(lMeshName);
    mSkinningTechnique->lightToUseDuringRender(0, 1, 2, 5);
}

void Application::_initMultipassShadowMapping(void)
{
    // Display the name of the technique that is initialized
    Log::consoleMessage("Multipass Shadow Mapping", Log::EDecoration::TITLE);


    // Display the state of some configuration parameters of openGL
    GLUtils::currentState();

    _checkLightsType();

    // Set up point lights
    static_pointer_cast<PointLight>(mLights[1])->ambientIntensity(0.1f);
    static_pointer_cast<PointLight>(mLights[1])->diffuseIntensity(0.9f);
    static_pointer_cast<PointLight>(mLights[1])->color(vec3f(0.6f, 0.99f, 0.6f));
    static_pointer_cast<PointLight>(mLights[1])->position(vec3f(0.3f, 5.0f, 0.0f));
    static_pointer_cast<PointLight>(mLights[1])->attenuation(SpotLight::ATTENUATION_TYPE::CONSTANT, 1.0f);
    static_pointer_cast<PointLight>(mLights[1])->attenuation(SpotLight::ATTENUATION_TYPE::LINEAR, 0.01f);

    // Setup the camera
    mCamera->position(vec3f(-1.67f, 5.9f, -12.0f));
    mCamera->lookAt(vec3f(0.03f, -0.1f, 0.99f));
    mCamera->up(vec3f(0.0f, 1.0f, 0.0f));

    const auto lMeshName = string("sphere");
    auto lCubeMeshIterator = mMeshes.find(lMeshName);

    if (lCubeMeshIterator != mMeshes.end())
    {
        lCubeMeshIterator->second.transform.emplace_back();
        lCubeMeshIterator->second.transform.back().translation(0.0f, 3.0f, 0.0f);

        lCubeMeshIterator->second.transform.emplace_back();
        lCubeMeshIterator->second.transform.back().translation(0.0f, 5.0f, 3.0f);
    }

    // Configure two meshes to illustrate the shadow with shadow map
    const auto lFloorMeshName = string("quad_r");
    auto lFloorMeshIterator = mMeshes.find(lFloorMeshName);

    if (lFloorMeshIterator != mMeshes.end())
    {
        Transform lTransform;
        lTransform.scaling(10.0f, 10.0f, 10.0f);
        lTransform.rotation(90.0f, 0.0f, 0.0f);
        lFloorMeshIterator->second.transform.push_back(lTransform);

        lTransform.rotation(0.0f, 0.0f, 0.0f);
        lTransform.translation(0.0f, 6.0f, 7.0f);
        lFloorMeshIterator->second.transform.push_back(lTransform);
    }

    mMultipassShadowMapTechnique = make_unique<MultipassShadowMapTechnique>();
    mMultipassShadowMapTechnique->init(1, mWindow->frameBufferDimensions());
    mMultipassShadowMapTechnique->camera(mCamera);
    mMultipassShadowMapTechnique->addMeshToRender(lMeshName);
    mMultipassShadowMapTechnique->floor(lFloorMeshName);
    mMultipassShadowMapTechnique->pointLightIndex(1);
}

void Application::_initSSAO(void)
{
    // Display the name of the technique that is initialized
    Log::consoleMessage("Screen Space Ambient Occlusion", Log::EDecoration::TITLE);


    // Display the state of some configuration parameters of openGL
    GLUtils::currentState();

    _checkLightsType();

    // Set up directional light
    static_pointer_cast<DirectionalLight>(mLights[0])->color(vec3f(1.0f));
    static_pointer_cast<DirectionalLight>(mLights[0])->ambientIntensity(0.2f);
    static_pointer_cast<DirectionalLight>(mLights[0])->diffuseIntensity(0.8f);
    static_pointer_cast<DirectionalLight>(mLights[0])->direction(vec3f(1.0f, 0.0f, 0.0f));

    // Set up point lights
    static_pointer_cast<PointLight>(mLights[1])->ambientIntensity(0.6f);
    static_pointer_cast<PointLight>(mLights[1])->diffuseIntensity(0.5f);
    static_pointer_cast<PointLight>(mLights[1])->color(vec3f(1.0f, 0.5f, 0.0f));
    static_pointer_cast<PointLight>(mLights[1])->position(vec3f(23.0f, 1.0f, 0.0f));
    static_pointer_cast<PointLight>(mLights[1])->attenuation(SpotLight::ATTENUATION_TYPE::LINEAR, 0.1f);

    static_pointer_cast<PointLight>(mLights[2])->ambientIntensity(0.1f);
    static_pointer_cast<PointLight>(mLights[2])->diffuseIntensity(0.9f);
    static_pointer_cast<PointLight>(mLights[2])->color(vec3f(0.0f, 0.5f, 1.0f));
    static_pointer_cast<PointLight>(mLights[2])->position(vec3f(7.0f, 1.0f, 0.0f));
    static_pointer_cast<PointLight>(mLights[2])->attenuation(SpotLight::ATTENUATION_TYPE::LINEAR, 0.1f);

    // Set up spot lights
    static_pointer_cast<SpotLight>(mLights[5])->ambientIntensity(0.1f);
    static_pointer_cast<SpotLight>(mLights[5])->diffuseIntensity(0.9f);
    static_pointer_cast<SpotLight>(mLights[5])->color(vec3f(1.0f, 1.0f, 1.0f));
    static_pointer_cast<SpotLight>(mLights[5])->position(vec3f(-20.0f, 20.0f, 1.0f));
    static_pointer_cast<SpotLight>(mLights[5])->direction(vec3f(1.0f, -1.0f, 0.0f));
    static_pointer_cast<SpotLight>(mLights[5])->attenuation(SpotLight::ATTENUATION_TYPE::LINEAR, 0.01f);
    static_pointer_cast<SpotLight>(mLights[5])->cutoff(20.0f);

    // Setup the camera
    mCamera->position(vec3f(-22.42f, 24.6f, -34.6f));
    mCamera->lookAt(vec3f(0.33f, -0.39f, 0.86f));
    mCamera->up(vec3f(0.0f, 1.0f, 0.0f));

    const auto lMeshName = string("jeep");
    auto lMeshIterator = mMeshes.find(lMeshName);

    if (lMeshIterator != mMeshes.end())
    {
        lMeshIterator->second.transform.emplace_back();
        lMeshIterator->second.transform.back().scaling(0.05f, 0.05f, 0.05f);
        lMeshIterator->second.transform.back().rotation(0.0f, 180.0f, 0.0f);
    }

    mSSAOTechnique = make_unique<SSAOTechnique>();
    mSSAOTechnique->camera(mCamera);
    mSSAOTechnique->init(2, 1, mWindow->frameBufferDimensions());
    mSSAOTechnique->addMeshToRender(lMeshName);
    mSSAOTechnique->lightToUseDuringRender(0, 1, 2, 5);
}

void Application::_initShadowMapDirectionalLight(void)
{
    // Display the name of the technique that is initialized
    Log::consoleMessage("Shadow Map Directional Light", Log::EDecoration::TITLE);


    // Display the state of some configuration parameters of openGL
    GLUtils::currentState();

    _checkLightsType();

    // Set up directional light
    static_pointer_cast<DirectionalLight>(mLights[0])->color(vec3f(1.0f));
    static_pointer_cast<DirectionalLight>(mLights[0])->ambientIntensity(0.1f);
    static_pointer_cast<DirectionalLight>(mLights[0])->diffuseIntensity(0.9f);
    static_pointer_cast<DirectionalLight>(mLights[0])->direction(vec3f(1.0f, -1.0f, 0.0f));

    // Setup the camera
    mCamera->position(vec3f(13.4f, 18.39f, -24.4f));
    mCamera->lookAt(vec3f(-0.45f, -0.25f, 0.86f));
    mCamera->up(vec3f(0.0f, 1.0f, 0.0f));

    const array<string, 3> lMeshNames = {{string("Dragon"), string("Buddha"), string("Bunny")}};

    for (unsigned int i = 0; i < lMeshNames.size(); ++i)
    {
        auto lMeshIterator = mMeshes.find(lMeshNames[i]);

        if (lMeshIterator != mMeshes.end())
        {
            lMeshIterator->second.transform.emplace_back();
            lMeshIterator->second.transform.back().translation(0.0f, 0.0f, 3.0f + static_cast<float>(i) * 30.0f);
        }
    }

    // Configure mesh to illustrate the shadow with shadow map
    const auto lFloorMeshName = string("quad");
    auto lFloorMeshIterator = mMeshes.find(lFloorMeshName);

    if (lFloorMeshIterator != mMeshes.end())
    {
        lFloorMeshIterator->second.transform.emplace_back();
        lFloorMeshIterator->second.transform.back().scaling(50.0f, 100.0f, 1.0f);
        lFloorMeshIterator->second.transform.back().translation(0.0f, -1.0f, 1.0f);
        lFloorMeshIterator->second.transform.back().rotation(90.0f, 0.0f, 0.0f);
    }

    mCamera->orthogonalProjectionParameterCount(1);
    mCamera->orthogonalProjectionParameter(0, Camera::EOrthogonalProjection::LEFT, -100.0f);
    mCamera->orthogonalProjectionParameter(0, Camera::EOrthogonalProjection::RIGHT, 100.0f);
    mCamera->orthogonalProjectionParameter(0, Camera::EOrthogonalProjection::TOP, 100.0f);
    mCamera->orthogonalProjectionParameter(0, Camera::EOrthogonalProjection::BOTTOM, -100.0f);
    mCamera->orthogonalProjectionParameter(0, Camera::EOrthogonalProjection::NEAR, -10.0f);
    mCamera->orthogonalProjectionParameter(0, Camera::EOrthogonalProjection::FAR, 100.0f);

    mShadowMapDirectionalLightTechnique = make_unique<ShadowMapDirectionalLightTechnique>();
    mShadowMapDirectionalLightTechnique->init(mWindow->frameBufferDimensions());
    mShadowMapDirectionalLightTechnique->camera(mCamera);
    mShadowMapDirectionalLightTechnique->addMeshToRender(lMeshNames[0]);
    mShadowMapDirectionalLightTechnique->addMeshToRender(lMeshNames[1]);
    mShadowMapDirectionalLightTechnique->addMeshToRender(lMeshNames[2]);
    mShadowMapDirectionalLightTechnique->floor(lFloorMeshName);
}

void Application::_initCascadedShadowMapping(void)
{
    // Display the name of the technique that is initialized
    Log::consoleMessage("Cascaded Shadow Mapping", Log::EDecoration::TITLE);


    // Display the state of some configuration parameters of openGL
    GLUtils::currentState();

    _checkLightsType();

    // Set up directional light
    static_pointer_cast<DirectionalLight>(mLights[0])->color(vec3f(1.0f));
    static_pointer_cast<DirectionalLight>(mLights[0])->ambientIntensity(0.1f);
    static_pointer_cast<DirectionalLight>(mLights[0])->diffuseIntensity(0.9f);
    static_pointer_cast<DirectionalLight>(mLights[0])->direction(vec3f(1.0f, -1.0f, 0.0f));

    // Setup the camera
    mCamera->position(vec3f(16.88f, 11.69f, -15.70f));
    mCamera->lookAt(vec3f(-0.59f, -0.25f, 0.77f));
    mCamera->up(vec3f(0.0f, 1.0f, 0.0f));

    const array<string, 3> lMeshNames = {{string("Dragon"), string("Buddha"), string("Bunny")}};

    for (unsigned int i = 0; i < lMeshNames.size(); ++i)
    {
        auto lMeshIterator = mMeshes.find(lMeshNames[i]);

        if (lMeshIterator != mMeshes.end())
        {
            lMeshIterator->second.transform.emplace_back();
            lMeshIterator->second.transform.back().translation(0.0f, 0.0f, 3.0f + static_cast<float>(i) * 30.0f);
        }
    }

    // Configure mesh to illustrate the shadow with shadow map
    const auto lFloorMeshName = string("quad");
    auto lFloorMeshIterator = mMeshes.find(lFloorMeshName);

    if (lFloorMeshIterator != mMeshes.end())
    {
        lFloorMeshIterator->second.transform.emplace_back();
        lFloorMeshIterator->second.transform.back().scaling(50.0f, 100.0f, 1.0f);
        lFloorMeshIterator->second.transform.back().translation(0.0f, -1.0f, 1.0f);
        lFloorMeshIterator->second.transform.back().rotation(90.0f, 0.0f, 0.0f);
    }

    mCamera->orthogonalProjectionParameterCount(3);

    mCascadedShadowMapDirectionalLightTechnique = make_unique<CascadedShadowMapDirectionalLightTechnique>();
    mCascadedShadowMapDirectionalLightTechnique->camera(mCamera);
    mCascadedShadowMapDirectionalLightTechnique->init(mWindow->frameBufferDimensions());
    mCascadedShadowMapDirectionalLightTechnique->addMeshToRender(lMeshNames[0]);
    mCascadedShadowMapDirectionalLightTechnique->addMeshToRender(lMeshNames[1]);
    mCascadedShadowMapDirectionalLightTechnique->addMeshToRender(lMeshNames[2]);
    mCascadedShadowMapDirectionalLightTechnique->floor(lFloorMeshName);
}

void Application::_initMultipleExamples(void)
{
    // Display the name of the technique that is initialized
    Log::consoleMessage("Multiple examples", Log::EDecoration::TITLE);

    // Display the state of some configuration parameters of openGL
    GLUtils::currentState();

    _checkLightsType();

    // Directional light
    static_pointer_cast<DirectionalLight>(mLights[0])->color(vec3f(1.0f));
    static_pointer_cast<DirectionalLight>(mLights[0])->ambientIntensity(0.0f);
    static_pointer_cast<DirectionalLight>(mLights[0])->diffuseIntensity(0.5f);
    static_pointer_cast<DirectionalLight>(mLights[0])->direction(vec3f({1.0f, -1.0f, 0.0f}));

    // Point lights
    for (unsigned int i = 1; i < 5; ++i)
    {
        static_pointer_cast<PointLight>(mLights[i])->ambientIntensity(0.0f);
        static_pointer_cast<PointLight>(mLights[i])->diffuseIntensity(1.0f);
        static_pointer_cast<PointLight>(mLights[i])->attenuation(SpotLight::ATTENUATION_TYPE::CONSTANT, 0.0f);
    }

    static_pointer_cast<PointLight>(mLights[1])->color(vec3f(0.0f, 1.0f, 0.0f));
    static_pointer_cast<PointLight>(mLights[1])->position(vec3f(-30.0f, 10.0f, -10.0f));
    static_pointer_cast<PointLight>(mLights[1])->attenuation(SpotLight::ATTENUATION_TYPE::LINEAR, 0.1f);

    static_pointer_cast<PointLight>(mLights[2])->color(vec3f(0.6f, 0.6f, 1.0f));
    static_pointer_cast<PointLight>(mLights[2])->position(vec3f(28.0f, 5.f, -14.0f));
    static_pointer_cast<PointLight>(mLights[2])->attenuation(SpotLight::ATTENUATION_TYPE::LINEAR, 0.04f);

    static_pointer_cast<PointLight>(mLights[3])->color(vec3f(1.0f, 0.5f, 1.0f));
    static_pointer_cast<PointLight>(mLights[3])->position(vec3f(28.0f, 5.5f, 12.0f));
    static_pointer_cast<PointLight>(mLights[3])->attenuation(SpotLight::ATTENUATION_TYPE::LINEAR, 0.04f);

    static_pointer_cast<PointLight>(mLights[4])->color(vec3f(1.0f, 0.8f, 0.23f));
    static_pointer_cast<PointLight>(mLights[4])->position(vec3f(0.0f, 20.0f, 8.0f));
    static_pointer_cast<PointLight>(mLights[4])->attenuation(SpotLight::ATTENUATION_TYPE::LINEAR, 0.08f);

    // Spot lights
    static_pointer_cast<SpotLight>(mLights[5])->ambientIntensity(0.1f);
    static_pointer_cast<SpotLight>(mLights[5])->diffuseIntensity(0.9f);
    static_pointer_cast<SpotLight>(mLights[5])->color(vec3f(1.0f, 1.0f, 1.0f));
    static_pointer_cast<SpotLight>(mLights[5])->position(vec3f(-30.0f, 35.0f, 17.0f));
    static_pointer_cast<SpotLight>(mLights[5])->direction(vec3f(0.73f, -0.64f, -0.24f));
    static_pointer_cast<SpotLight>(mLights[5])->attenuation(SpotLight::ATTENUATION_TYPE::LINEAR, 0.02f);
    static_pointer_cast<SpotLight>(mLights[5])->cutoff(40.0f);


    const auto lFloorMeshName = string("quad");
    auto lFloorMeshIterator = mMeshes.find(lFloorMeshName);

    if (lFloorMeshIterator != mMeshes.end())
    {
        lFloorMeshIterator->second.transform.emplace_back();
        lFloorMeshIterator->second.transform.back().scaling(40.0f, 20.0f, 50.0f);
        lFloorMeshIterator->second.transform.back().translation(-20.0f, -1.0f, 0.0f);
        lFloorMeshIterator->second.transform.back().rotation(90.0f, 0.0f, 0.0f);
    }

    // Container to store all the names of the meshes that will be rendered with the simple lighting technique
    vector<string> lSimpleLightingMeshNames;

    lSimpleLightingMeshNames.emplace_back("spaceShip");
    auto lMeshIterator = mMeshes.find(lSimpleLightingMeshNames.back());

    if (lMeshIterator != mMeshes.end())
    {
        lMeshIterator->second.transform.emplace_back();
        lMeshIterator->second.transform.back().scaling(0.1f, 0.1f, 0.1f);
        lMeshIterator->second.transform.back().translation(5.0f, 0.0f, 3.0f);
    }

    lSimpleLightingMeshNames.emplace_back("cube");
    auto lCubeMeshIterator = mMeshes.find(lSimpleLightingMeshNames.back());

    if (lCubeMeshIterator != mMeshes.end())
    {
        lCubeMeshIterator->second.transform.emplace_back();
        lCubeMeshIterator->second.transform.back().translation(7.0f, 3.0f, -6.0f);
    }

    lSimpleLightingMeshNames.emplace_back("jeep");
    auto lJeepMeshIterator = mMeshes.find(lSimpleLightingMeshNames.back());

    if (lJeepMeshIterator != mMeshes.end())
    {
        lJeepMeshIterator->second.transform.emplace_back();
        lJeepMeshIterator->second.transform.back().translation(-9.0f, 0.5f, 6.0f);
        lJeepMeshIterator->second.transform.back().scaling(0.01f, 0.01f, 0.01f);
    }

    lSimpleLightingMeshNames.emplace_back("helicopter");
    auto lHelicopterMeshIterator = mMeshes.find(lSimpleLightingMeshNames.back());

    if (lHelicopterMeshIterator != mMeshes.end())
    {
        lHelicopterMeshIterator->second.transform.emplace_back();
        lHelicopterMeshIterator->second.transform.back().translation(-6.0f, 4.0f, -9.0f);
        lHelicopterMeshIterator->second.transform.back().scaling(0.04f, 0.04f, 0.04f);
    }

    // Create and initialize the simple lighting technique
    mSimpleLightingWithShadow = make_unique<SimpleLightingWithShadow>();
    mSimpleLightingWithShadow->init(4u, mWindow->frameBufferDimensions());
    mSimpleLightingWithShadow->camera(mCamera);
    mSimpleLightingWithShadow->floor(lFloorMeshName);
    mSimpleLightingWithShadow->lightToUseDuringRender(1, 2, 3, 4, 5);

    lSimpleLightingMeshNames.emplace_back("spider");
    auto lSpiderPickingIterator = mMeshes.find(lSimpleLightingMeshNames.back());

    if (lSpiderPickingIterator != mMeshes.end())
    {
        Transform lTransform;
        lTransform.scaling(0.02f, 0.02f, 0.02f);

        const degreef lRotationY = 90.0f;
        lTransform.rotation(0.0f, lRotationY, 0.0f);
        lTransform.translation(-6.0f, 0.8f, -5.0f);

        lSpiderPickingIterator->second.transform.push_back(lTransform);

        lTransform.translation(-2.0f, 0.8f, -5.0f);
        lSpiderPickingIterator->second.transform.push_back(lTransform);
    }

    // Particle system example
    mParticleSystem = make_unique<ParticleSystem>();
    mParticleSystem->init(500, vec3f(15.0f, 0.0f, -15.0f));

    mCurrentTime = high_resolution_clock::now();

    // 3D picking example
    mPicking3D = make_unique<Picking3D>();
    mPicking3D->init(mWindow->frameBufferDimensions(), mWindow->windowDimensions());
    mPicking3D->camera(mCamera);
    mPicking3D->meshToRender(lSimpleLightingMeshNames.back());

    mPickingOn = true;

    for (auto name : lSimpleLightingMeshNames)
        mSimpleLightingWithShadow->addMeshToRender(name);

    // GLFX example
    array<vec3f, 4> lPositions = {{vec3f(-5.0f, 10.0f, 0.0f), vec3f( 0.0f, 10.0f, 0.0f), vec3f( 5.0f, 10.0f, 0.0f), vec3f( 10.0f, 10.0f, 0.0f)}};
    array<vec3f, 4> lColors = {{vec3f(1.0f, 0.0f, 0.0f), vec3f(0.0f, 1.0f, 0.0f), vec3f(0.0f, 0.0f, 1.0f), vec3f(0.7f, 0.7f, 0.0f)}};
    array<string, 4> lMeshNames = {{string("letter g"), string("letter l"), string("letter f"), string("letter x")}};

    mGLFXTechnique = make_unique<GLFXTechnique>();
    mGLFXTechnique->init(3u, 0u);
    mGLFXTechnique->camera(mCamera);
    mGLFXTechnique->lightToUseDuringRender(0, 2, 3, 4);

    for (size_t i = 0; i < lPositions.size(); ++i)
    {
        auto lMeshIterator = mMeshes.find(lMeshNames[i]);

        if (lMeshIterator != mMeshes.end())
        {
            Transform lTransform;
            lTransform.scaling(0.2f, 0.2f, 0.2f);
            lTransform.translation(lPositions[i].x(), lPositions[i].y(), lPositions[i].z());

            lMeshIterator->second.transform.push_back(lTransform);

            mGLFXTechnique->addMeshToRender(lMeshNames[i]);
            mGLFXTechnique->addUniformColor(lColors[i].x(), lColors[i].y(), lColors[i].z());
        }
    }

    // Instanced rendering example
    const auto lSpiderName = string("spider - instanced rendering");
    auto lSpiderIterator = mMeshes.find(lSpiderName);

    if (lSpiderIterator != mMeshes.end())
    {
        Transform lTransform;
        lTransform.scaling(0.005f, 0.005f, 0.005f);
        lTransform.rotation(0.0f, 90.0f, 0.0f);

        lSpiderIterator->second.transform.push_back(lTransform);
    }

    //    _validateShaderWithMesh(mInstancedLighting, lName);

    // Compute the position of the different instances
    const unsigned int lRows = 50;
    const unsigned int lColumns = 20;
    vector<vec3f> lInstancePositions(lRows * lColumns, vec3f(0.0f));
    vector<vec3f> lInstanceVelocities(lRows * lColumns, vec3f(0.0f));

    default_random_engine lGenerator;
    lGenerator.seed(static_cast<unsigned int>(system_clock::now().time_since_epoch().count()));

    uniform_real_distribution<float> lDistribution(0, 1.0f);

    const vec3f lOrigin(20.0f, 0.0f, -20.0f);

    for (unsigned int i = 0; i < lRows; ++i)
    {
        for (unsigned int j = 0; j < lColumns; ++j)
        {
            const unsigned int lIndex = i * lColumns + j;

            lInstancePositions[lIndex].x() = lOrigin.x() + static_cast<float>(j);
            lInstancePositions[lIndex].y() = lOrigin.y() + lDistribution(lGenerator) * 5.0f;
            lInstancePositions[lIndex].z() = lOrigin.z() + static_cast<float>(i);

            // Set the velocity only on the y axis
            lInstanceVelocities[lIndex].y() = lDistribution(lGenerator);

            // Velocities with odd indices are a negative
            if ((i & 1) == 1)
                lInstanceVelocities[lIndex].y() *= -1.0f;
        }
    }

    mInstancedLighting = make_unique<InstancedLightingTechnique>();
    mInstancedLighting->init(2u, mWindow->frameBufferDimensions());
    mInstancedLighting->camera(mCamera);
    mInstancedLighting->addMeshToRender(lSpiderName);
    mInstancedLighting->instancePositions(lInstancePositions);
    mInstancedLighting->instanceVelocities(lInstanceVelocities);
    mInstancedLighting->lightToUseDuringRender(2, 3);

    // Billboarding example
    mBillboardList = make_unique<BillboardList>();
    mBillboardList->init(string(R"(./monster_hellknight.jpg)"), vec3f({-40.0f, -1.0f, -18.0f}), 4, 4, 10.0f);
    mBillboardList->camera(mCamera);

    // Skybox example
    mSkyBox = make_unique<SkyBox>(mCamera);
    mSkyBox->init({"./sp3right.jpg", "./sp3left.jpg", "./sp3top.jpg", "./sp3bot.jpg", "./sp3front.jpg", "./sp3back.jpg"});

    // Tessellationb example
    const auto lMeshName = string("terrain");
    auto lTerranIterator = mMeshes.find(lMeshName);

    if (lTerranIterator != mMeshes.end())
    {
        Transform lTransform;
        lTransform.scaling(10.0f, 1.0f, 20.0f);
        lTransform.translation(30.0f, -2.0f, 0.0f);

        lTerranIterator->second.transform.push_back(lTransform);
    }

    mTessellation = make_unique<Tessellation>();
    mTessellation->init(1u, string(R"(./heightmap.jpg)"));
    mTessellation->camera(mCamera);
    mTessellation->addMeshToRender(lMeshName);
    mTessellation->lightToUseDuringRender(2);
    mTessellation->displacementFactor(4.1f);

    // Tessellation Point Normal example
    const auto lMonkeyName = string("monkey");
    auto lMonkeyIterator = mMeshes.find(lMonkeyName);

    if (lMonkeyIterator != mMeshes.end())
    {
        Transform lTransform;
        lTransform.scaling(3.0f, 3.0f, 3.0f);
        lTransform.translation(-5.0f, 20.0f, 0.0f);
        lTransform.rotation(-90.0f, 0.0f, 0.0f);

        lMonkeyIterator->second.transform.push_back(lTransform);

        lTransform.translation(5.0f, 20.0f, 0.0f);

        lMonkeyIterator->second.transform.push_back(lTransform);
    }

    mTessellationPN = make_unique<TessellationPN>();
    mTessellationPN->init(2u);
    mTessellationPN->camera(mCamera);
    mTessellationPN->addMeshToRender(lMonkeyName);
    mTessellationPN->addTessellationLevel(1.1f);
    mTessellationPN->addTessellationLevel(1.1f);
    mTessellationPN->lightToUseDuringRender(0, 1, 4);

    // Setup the camera
    mCamera->position(vec3f(0.0f, 14.0f, -93.0f));
    mCamera->lookAt(vec3f(0.03f, -0.1f, 1.0f));
    mCamera->up(vec3f(0.0f, 1.0f, 0.0f));
}

void Application::_initATB(void)
{
    const auto lWindowDims = mWindow->frameBufferDimensions();

    vec2f lDimensionsAsFraction(0.3, 0.8);
    vec3f lColor(200.0f, 180.0f, 255.0f);
    mATB.init(lDimensionsAsFraction, get<0>(lWindowDims), get<1>(lWindowDims), lColor);

    /*! \remark The names of the following rendering techniques MUST match the ORDER used in _initCurrentRenderingTechnique
                and _deactivateRenderingTechniques */
    vector<string> lRenderingTechniqueNames =
    {
        "3D Picking",
        "Billboarding",
        "Particle System",
        "Simple Lighting With Shadow",
        "Tessellation",
        "Tessellation PN",
        "Instanced Lighting",
        "GLFX",
        "Deferred Shading",
        "Skinning", //skeletal animation
        "Skinning With Motion Blur", //skeletal animation
        "Silhouette",
        "Shadow Volume",
        "Multipass ShadowMap",
        "SSAO",
        "ShadowMap Directional Light",
        "Cascaded ShadowMap Directional Light",
        "Skybox",
        "MultipleTechniques"
    };

    mATB.initRenderingTechnique(lRenderingTechniqueNames);

    mATB.initCamera(*mCamera);
    mATB.initRotation(mMeshOrientation);

    mATB.initLights(mLights);
}

void Application::_renderSimpleLighting(void)
{
    // Name of meshes to rotate
    const auto & lNames = mSimpleLightingWithShadow->meshToRenderNames();

    for (const auto it : lNames)
    {
        for (auto & transform : mMeshes.at(it).transform)
        {
            // Make the mesh rotate
            transform.rotation(0.0f, mRotationAngle, 0.0f);

            // Update the orientation
            auto lTmpRotation = transform.rotation();
            lTmpRotation = lTmpRotation * mMeshOrientation;
            transform.rotation(lTmpRotation);
        }
    }

    mSimpleLightingWithShadow->render(mMeshes, mLights);
}

void Application::_renderBillboarding(void)
{
    mBillboardList->render();
}

void Application::_renderParticleSystem(void)
{
    auto lNow = high_resolution_clock::now();
    auto lDeltaTime = static_cast<unsigned int>(duration_cast<milliseconds>(lNow - mCurrentTime).count());

    mCurrentTime = lNow;

    auto lVP = mCamera->projection() * mCamera->view();

    mParticleSystem->render(lDeltaTime, lVP, mCamera->position());
}

void Application::_render3DPicking(void)
{
    // Name of meshes to rotate
    const auto & lNames = mSimpleLightingWithShadow->meshToRenderNames();

    for (const auto it : lNames)
    {
        for (auto & transform : mMeshes.at(it).transform)
        {
            // Make the mesh rotate
            transform.rotation(0.0f, mRotationAngle - 90.0f, 0.0f);

            // Update the orientation
            auto lTmpRotation = transform.rotation();
            lTmpRotation = lTmpRotation * mMeshOrientation;
            transform.rotation(lTmpRotation);
        }
    }

    // Do not use the directional light
    mLights[0]->ambientIntensity(0.0f);
    mLights[0]->diffuseIntensity(0.0f);

    // Update the position of the point lights
    auto lAngle = mRotationAngle.toRadian();
    const auto lRadius = 5.0f;
    static_pointer_cast<PointLight>(mLights[1])->position(vec3f(7.0f + lRadius * cosf(lAngle),                      2.0f, -7.0f + lRadius * sinf(lAngle)));
    static_pointer_cast<PointLight>(mLights[2])->position(vec3f(7.0f + lRadius * cosf(lAngle + M_PI / 2.0f),        2.0f, -7.0f + lRadius * sinf(lAngle + M_PI / 2.0f)));
    static_pointer_cast<PointLight>(mLights[3])->position(vec3f(7.0f + lRadius * cosf(lAngle + M_PI),               2.0f, -7.0f + lRadius * sinf(lAngle + M_PI)));
    static_pointer_cast<PointLight>(mLights[4])->position(vec3f(7.0f + lRadius * cosf(lAngle + 3.0f * M_PI / 2.0f), 2.0f, -7.0f + lRadius * sinf(lAngle + 3.0f * M_PI / 2.0f)));

    /** \bug Some triangles are not highlighted */
    // We MUST render the triangle illustrating the 3D picking BEFORE rendering the corresponding mesh, otherwise, we won't see it. Not sure why...
    if (mCtrlKeyPressed)
        mPicking3D->render(mMeshes, mMousePos);

    mSimpleLightingWithShadow->render(mMeshes, mLights);
}

void Application::_renderTessellation(void)
{
    mTessellation->render(mMeshes, mLights);
}

void Application::_renderTessellationPN(void)
{
    mTessellationPN->render(mMeshes, mLights);
}

void Application::_renderInstancedRendering(void)
{
    auto & lMesh = mMeshes.at("spider - instanced rendering");
    lMesh.transform[0].rotation(0.0f, 90.0f + mRotationAngle, 0.0f);

    // Update the orientation
    auto lTmpRotation = lMesh.transform[0].rotation();
    lTmpRotation = lTmpRotation * mMeshOrientation;
    lMesh.transform[0].rotation(lTmpRotation);

    mInstancedLighting->instanceVelocitiesMultiplier(sinf(mRotationAngle.toRadian()));
    mInstancedLighting->render(mMeshes, mLights);
}

void Application::_renderGLFXExample(void)
{
    const array<string, 4> lMeshNames = {{string("letter g"), string("letter l"), string("letter f"), string("letter x")}};

    for (const auto name : lMeshNames)
    {
        auto & lMesh = mMeshes.at(name);
        const degreef lInitialAngle = 180.0f;
        lMesh.transform[0].rotation(0.0f, lInitialAngle + mRotationAngle, 0.0f);

        // Update the orientation
        auto lTmpRotation = lMesh.transform[0].rotation();
        lTmpRotation = lTmpRotation * mMeshOrientation;
        lMesh.transform[0].rotation(lTmpRotation);
    }

    mGLFXTechnique->render(mMeshes, mLights);
}

void Application::_renderDeferredShading(void)
{
    const auto & lNames = mDeferredShading->meshToRenderNames();
    auto & lMesh = mMeshes.at(lNames[0]);

    for (auto & transform : lMesh.transform)
    {
        // Make the mesh rotate
        transform.rotation(0.0f, mRotationAngle + 90.0f, 0.0f);

        // Update the orientation
        auto lTmpRotation = transform.rotation();
        lTmpRotation = lTmpRotation * mMeshOrientation;
        transform.rotation(lTmpRotation);
    }

    mDeferredShading->render(mMeshes, mLights);
}

void Application::_renderSkeletalAnimation(void)
{
    const auto & lNames = mSkinningTechnique->meshToRenderNames();
    auto & lMesh = mMeshes.at(lNames[0]);
    lMesh.transform[0].rotation(90.0f, 0.0f + mRotationAngle, 0.0f);

    // Update the orientation
    auto lTmpRotation = lMesh.transform[0].rotation();
    lTmpRotation = lTmpRotation * mMeshOrientation;
    lMesh.transform[0].rotation(lTmpRotation);

    mSkinningTechnique->runningTime(mWindow->runningTime());
    mSkinningTechnique->render(mMeshes, mLights);
}

void Application::_renderSilhouetteDetection(void)
{
    // Update the orientation of the mesh from silhouette technique
    const auto & lSilhouetteNames = mSilhouetteTechnique->meshToRenderNames();
    auto & lSilhouetteMesh = mMeshes.at(lSilhouetteNames[0]);
    lSilhouetteMesh.transform[0].rotation(0.0f, 0.0f + mRotationAngle, 0.0f);

    auto lTmpRotation = lSilhouetteMesh.transform[0].rotation();
    lTmpRotation = lTmpRotation * mMeshOrientation;
    lSilhouetteMesh.transform[0].rotation(lTmpRotation);

    // Update the orientation of the mesh from lighting technique
    const auto & lLightingNames = mSimpleLightingWithShadow->meshToRenderNames();
    auto & lLightingMesh = mMeshes.at(lLightingNames[0]);
    lLightingMesh.transform[0].rotation(0.0f, 0.0f + mRotationAngle, 0.0f);

    lTmpRotation = lLightingMesh.transform[0].rotation();
    lTmpRotation = lTmpRotation * mMeshOrientation;
    lLightingMesh.transform[0].rotation(lTmpRotation);

    // YOU MUST CALL _renderSimpleLighting to show the cube graphically to be able to use the silhouette
    // rendering on top of it.
    mSimpleLightingWithShadow->render(mMeshes, mLights);

    mSilhouetteTechnique->render(mMeshes, mLights);
}

void Application::_renderShadowVolume(void)
{
    // Udpate the mesh used for graphics
    const auto & lNames = mShadowVolumeTechnique->meshToRenderNames();
    auto & lMesh = mMeshes.at(lNames[0]);
    lMesh.transform[0].rotation(0.0f, 0.0f + mRotationAngle, 0.0f);

    // Update the orientation
    auto lTmpRotation = lMesh.transform[0].rotation();
    lTmpRotation = lTmpRotation * mMeshOrientation;
    lMesh.transform[0].rotation(lTmpRotation);

    // Update the mesh with adjacencies used for shadow
    const auto & lNamesWithAdjacencies = mShadowVolumeTechnique->meshWithAdjacenciesNames();
    auto & lMeshWithAdjacencies = mMeshes.at(lNamesWithAdjacencies[0]);
    lMeshWithAdjacencies.transform[0].rotation(0.0f, 0.0f + mRotationAngle, 0.0f);

    auto lTmpRotation2 = lMeshWithAdjacencies.transform[0].rotation();
    lTmpRotation2 = lTmpRotation2 * mMeshOrientation;
    lMeshWithAdjacencies.transform[0].rotation(lTmpRotation2);

    mShadowVolumeTechnique->render(mMeshes, mLights);
}

void Application::_renderMultipassShadowMapping(void)
{
    // Udpate the mesh used for graphics
    const auto & lNames = mMultipassShadowMapTechnique->meshToRenderNames();
    auto & lMesh = mMeshes.at(lNames[0]);

    for (auto & transform : lMesh.transform)
    {
        // Make the mesh rotate
        transform.rotation(0.0f, mRotationAngle, 0.0f);

        // Update the orientation
        auto lTmpRotation = transform.rotation();
        lTmpRotation = lTmpRotation * mMeshOrientation;
        transform.rotation(lTmpRotation);
    }

    mMultipassShadowMapTechnique->render(mMeshes, mLights);
}

void Application::_renderSSAO(void)
{
    // Udpate the mesh used for graphics
    const auto & lNames = mSSAOTechnique->meshToRenderNames();
    auto & lMesh = mMeshes.at(lNames[0]);

    for (auto & transform : lMesh.transform)
    {
        // Make the mesh rotate
        transform.rotation(0.0f, mRotationAngle, 0.0f);

        // Update the orientation
        auto lTmpRotation = transform.rotation();
        lTmpRotation = lTmpRotation * mMeshOrientation;
        transform.rotation(lTmpRotation);
    }

    mSSAOTechnique->render(mMeshes, mLights);
}

void Application::_renderShadowMapDirectionalLight(void)
{
    for (const auto name : mShadowMapDirectionalLightTechnique->meshToRenderNames())
    {
        auto & lMesh = mMeshes.at(name);
        lMesh.transform[0].rotation(0.0f, mRotationAngle, 0.0f);

        // Update the orientation
        auto lTmpRotation = lMesh.transform[0].rotation();
        lTmpRotation = lTmpRotation * mMeshOrientation;
        lMesh.transform[0].rotation(lTmpRotation);
    }

    mShadowMapDirectionalLightTechnique->render(mMeshes, mLights);
}

void Application::_renderCascadedShadowMapping(void)
{
    for (const auto name : mCascadedShadowMapDirectionalLightTechnique->meshToRenderNames())
    {
        auto & lMesh = mMeshes.at(name);
        lMesh.transform[0].rotation(0.0f, mRotationAngle, 0.0f);

        // Update the orientation
        auto lTmpRotation = lMesh.transform[0].rotation();
        lTmpRotation = lTmpRotation * mMeshOrientation;
        lMesh.transform[0].rotation(lTmpRotation);
    }

    mCascadedShadowMapDirectionalLightTechnique->render(mMeshes, mLights);
}

void Application::_renderMultipleExamples(void)
{
    _renderGLFXExample();

    // 3D picking
    // We MUST render the triangle illustrating the 3D picking BEFORE rendering the corresponding mesh, otherwise, we won't see it. Not sure why...
    if (mCtrlKeyPressed)
        mPicking3D->render(mMeshes, mMousePos);

    _renderSimpleLighting();

    _renderInstancedRendering();

    _renderBillboarding();

    _renderParticleSystem();

    mTessellation->render(mMeshes, mLights);

    mTessellationPN->render(mMeshes, mLights);

    mSkyBox->render();
}

void Application::_saveGLParametersInLogFile(void) const
{
    GLenum lParams[] = { GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,
                         GL_MAX_CUBE_MAP_TEXTURE_SIZE,
                         GL_MAX_DRAW_BUFFERS,
                         GL_MAX_FRAGMENT_UNIFORM_COMPONENTS,
                         GL_MAX_TEXTURE_IMAGE_UNITS,
                         GL_MAX_TEXTURE_SIZE,
                         GL_MAX_VARYING_FLOATS,
                         GL_MAX_VERTEX_ATTRIBS,
                         GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,
                         GL_MAX_VERTEX_UNIFORM_COMPONENTS,
                         GL_MAX_PATCH_VERTICES,
                         GL_MAX_VIEWPORT_DIMS,
                         GL_STEREO};

    string lNames[] = { "GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS ",
                        "GL_MAX_CUBE_MAP_TEXTURE_SIZE ",
                        "GL_MAX_DRAW_BUFFERS ",
                        "GL_MAX_FRAGMENT_UNIFORM_COMPONENTS ",
                        "GL_MAX_TEXTURE_IMAGE_UNITS ",
                        "GL_MAX_TEXTURE_SIZE ",
                        "GL_MAX_VARYING_FLOATS ",
                        "GL_MAX_VERTEX_ATTRIBS ",
                        "GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS ",
                        "GL_MAX_VERTEX_UNIFORM_COMPONENTS ",
                        "GL_MAX_PATCH_VERTICES ",
                        "GL_MAX_VIEWPORT_DIMS ",
                        "GL_STEREO "};

    Log::write(Log::EType::COMMENT, "- GL Context Params:", true);

    // integers - only works if the order is 0-10 integer return types
    for(int i = 0; i < 11; i++)
    {
        int lValue = 0;
        glGetIntegerv (lParams[i], &lValue);
        lNames[i].append(to_string(lValue));
        Log::consoleMessage(lNames[i]);
    }

    // others
    int lOtherValues[2] = {0, 0};
    glGetIntegerv (lParams[11], lOtherValues);
    lNames[11].append(to_string(lOtherValues[0]));
    lNames[11].append(" ");
    lNames[11].append(to_string(lOtherValues[1]));
    Log::consoleMessage(lNames[11]);

    unsigned char s = 0;
    glGetBooleanv (lParams[12], &s);
    lNames[12].append(to_string(s));
    Log::consoleMessage(lNames[12]);
}
