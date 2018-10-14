//===============================================================================================//
/*!
 *  \file      AntTweakBarWrapper.cpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#include "AntTweakBarWrapper.hpp"

#include <iostream>
#include <string>

#include "DirectionalLight.hpp"
#include "PointLight.hpp"
#include "SpotLight.hpp"
#include "Exceptions.hpp"

using std::string;
using std::to_string;
using std::shared_ptr;
using std::static_pointer_cast;
using std::map;
using std::vector;
using miniGL::AntTweakBarWrapper;
using miniGL::MeshBase;
using miniGL::MeshAndTransform;
using miniGL::Camera;
using miniGL::BaseLight;

bool AntTweakBarWrapper::mAutoRotate = false;
bool AntTweakBarWrapper::mResetOrientation = false;

void AntTweakBarWrapper::init(vec2f pDimensionsAsFraction, unsigned int pFramebufferWidth, unsigned int pFramebufferHeight, vec3f pColor)
{
    if (TwInit(ETwGraphAPI::TW_OPENGL_CORE, NULL) != 1)
        throw Exceptions(string("Failed initializing Ant Tweak Bar"), __FILE__, __LINE__);

    // Define a "vec3f type" for internal use
    mVec3fCoordinates[0].Name = "x";
    mVec3fCoordinates[0].Type = TW_TYPE_FLOAT;
    mVec3fCoordinates[0].Offset = 0;
    mVec3fCoordinates[0].DefString = "";

    mVec3fCoordinates[1].Name = "y";
    mVec3fCoordinates[1].Type = TW_TYPE_FLOAT;
    mVec3fCoordinates[1].Offset = sizeof(float);
    mVec3fCoordinates[1].DefString = "";

    mVec3fCoordinates[2].Name = "z";
    mVec3fCoordinates[2].Type = TW_TYPE_FLOAT;
    mVec3fCoordinates[2].Offset = 2 * sizeof(float);
    mVec3fCoordinates[2].DefString = "";

    mVec3fTwType = TwDefineStruct("vec3f", mVec3fCoordinates, 3, sizeof(vec3f), nullptr, nullptr);

    // Define properties of the main bar
    mMainBar = TwNewBar("miniGL");
    TwDefine(" GLOBAL help='This example shows how to integrate AntTweakBar with OpenGL.' "); // Message added to the help bar.
    TwDefine(" GLOBAL fontresizable=true fontscaling=8 "); // use large font
    TwDefine(" GLOBAL contained=true "); // bars cannot move outside of the window

    // Set the size of the window
    TwWindowSize(pFramebufferWidth, pFramebufferHeight);

    // Set the size of the UI
    mMainBarDimensions = pDimensionsAsFraction;
    auto lSizeMessage = _createSizeMessage(mMainBarDimensions.x() * static_cast<float>(pFramebufferWidth), mMainBarDimensions.y() * static_cast<float>(pFramebufferHeight));
    TwDefine(lSizeMessage.c_str());

    // Set the color of the UI
    auto lColorMessage = _createColorMessage(pColor.x(), pColor.y(), pColor.z());
    TwDefine(lColorMessage.c_str());
    TwDefine(" miniGL valueswidth=400 "); // column width fits content
    TwDefine(" GLOBAL iconalign=horizontal "); // icons will be aligned horizontally

    // Set refresh rate of the main bar
    const auto lRefreshRate = 0.1f;
    TwSetParam(mMainBar, nullptr, "refresh", TW_PARAM_FLOAT, 1, &lRefreshRate);
}

void AntTweakBarWrapper::initCamera(Camera & pCamera)
{
    TwAddButton(mMainBar, "Camera", nullptr, nullptr, "help='Main camera of this application'");

    TwAddVarRW(mMainBar, "Position", mVec3fTwType, (void*)&pCamera.position(), "opened=true");
    TwAddVarRO(mMainBar, "Direction", TW_TYPE_DIR3F, &pCamera.lookAt(), " axisz=-z opened=true");

    // Separator
    TwAddSeparator(mMainBar, "", nullptr);
}

void AntTweakBarWrapper::initRotation(quatf & pMeshOrientation)
{
    // Rotation of objects in the scene
    TwAddButton(mMainBar, "Rotation", nullptr, nullptr, "help='Rotation control'");
    TwAddVarRW(mMainBar, "ObjectsRotation", TW_TYPE_QUAT4F, & pMeshOrientation, " opened=true");
    TwAddButton(mMainBar, "ResetOrientation", _resetOrientationCallback, nullptr, " label='Reset the orientation of the mesh'");
    TwAddButton(mMainBar, "AutoRotate", _autoRotateCallback, nullptr, " label='Auto rotate'");

    TwAddVarRW(mMainBar, "Rotation speed", TW_TYPE_FLOAT, & mIncrement, "min=0 max=180 step=0.5 keyIncr=s keyDecr=d help='Rotation speed'");

    // Separator
    TwAddSeparator(mMainBar, "", nullptr);
}

void AntTweakBarWrapper::initLights(vector<shared_ptr<BaseLight>> & pLights)
{
    TwAddButton(mMainBar, "All lights", nullptr, nullptr, "help='Lights control'");

    for (auto it : pLights)
    {
        switch (it->type())
        {
            case BaseLight::EType::DIRECTIONAL:
                // Lights (direcitional light in this case)
                TwAddButton(mMainBar, "Directional Light", nullptr, nullptr, "help='Directional light controls'");
                TwAddVarRW(mMainBar, it->name().append(" color").c_str(), TW_TYPE_COLOR3F, & it->color(), "help='light color'");
                TwAddVarRW(mMainBar, it->name().append(" ambient intensity").c_str(), TW_TYPE_FLOAT, & it->ambientIntensity(), "min=0.0 max=1.0 step=0.005");
                TwAddVarRW(mMainBar, it->name().append(" diffuse intensity").c_str(), TW_TYPE_FLOAT, & it->diffuseIntensity(), "min=0.0 max=1.0 step=0.005");
                TwAddVarRW(mMainBar, it->name().append(" direction").c_str(), TW_TYPE_DIR3F, & static_pointer_cast<DirectionalLight>(it)->direction(), "  opened=true");
                break;

            case BaseLight::EType::POINT:
                TwAddButton(mMainBar, it->name().c_str(), nullptr, nullptr, "help='Point light controls'");
                TwAddVarRW(mMainBar, it->name().append(" color").c_str(), TW_TYPE_COLOR3F, & it->color(), "help='light color'");
                TwAddVarRW(mMainBar, it->name().append(" ambient intensity").c_str(), TW_TYPE_FLOAT, & it->ambientIntensity(), "min=0.0 max=1.0 step=0.005");
                TwAddVarRW(mMainBar, it->name().append(" diffuse intensity").c_str(), TW_TYPE_FLOAT, & it->diffuseIntensity(), "min=0.0 max=1.0 step=0.005");

                TwAddVarRW(mMainBar, it->name().append(" constant attenuation").c_str(), TW_TYPE_FLOAT, & static_pointer_cast<PointLight>(it)->attenuation(miniGL::PointLight::ATTENUATION_TYPE::CONSTANT), "min=0.0 max=1.0 step=0.005");
                TwAddVarRW(mMainBar, it->name().append(" linear attenuation").c_str(), TW_TYPE_FLOAT, & static_pointer_cast<PointLight>(it)->attenuation(miniGL::PointLight::ATTENUATION_TYPE::LINEAR), "min=0.0 max=1.0 step=0.005");
                TwAddVarRW(mMainBar, it->name().append(" exponential attenuation").c_str(), TW_TYPE_FLOAT, & static_pointer_cast<PointLight>(it)->attenuation(miniGL::PointLight::ATTENUATION_TYPE::EXPONENTIAL), "min=0.0 max=1.0 step=0.005");
                TwAddVarRW(mMainBar, it->name().append(" position").c_str(), mVec3fTwType, & static_pointer_cast<PointLight>(it)->position(), " opened=true");
                break;

            case BaseLight::EType::SPOT:
                TwAddButton(mMainBar, it->name().c_str(), nullptr, nullptr, "help='Spot light controls'");
                TwAddVarRW(mMainBar, it->name().append(" color").c_str(), TW_TYPE_COLOR3F, & it->color(), "help='light color'");
                TwAddVarRW(mMainBar, it->name().append(" ambient intensity").c_str(), TW_TYPE_FLOAT, & it->ambientIntensity(), "min=0.0 max=1.0 step=0.005");
                TwAddVarRW(mMainBar, it->name().append(" diffuse intensity").c_str(), TW_TYPE_FLOAT, & it->diffuseIntensity(), "min=0.0 max=1.0 step=0.005");

                TwAddVarRW(mMainBar, it->name().append(" constant attenuation").c_str(), TW_TYPE_FLOAT, & static_pointer_cast<SpotLight>(it)->attenuation(miniGL::PointLight::ATTENUATION_TYPE::CONSTANT), "min=0.0 max=1.0 step=0.005");
                TwAddVarRW(mMainBar, it->name().append(" linear attenuation").c_str(), TW_TYPE_FLOAT, & static_pointer_cast<SpotLight>(it)->attenuation(miniGL::PointLight::ATTENUATION_TYPE::LINEAR), "min=0.0 max=1.0 step=0.005");
                TwAddVarRW(mMainBar, it->name().append(" exponential attenuation").c_str(), TW_TYPE_FLOAT, & static_pointer_cast<SpotLight>(it)->attenuation(miniGL::PointLight::ATTENUATION_TYPE::EXPONENTIAL), "min=0.0 max=1.0 step=0.005");
                TwAddVarRW(mMainBar, it->name().append(" position").c_str(), mVec3fTwType, & static_pointer_cast<SpotLight>(it)->position(), " opened=true");
                TwAddVarRW(mMainBar, it->name().append(" direction").c_str(), TW_TYPE_DIR3F, & static_pointer_cast<SpotLight>(it)->direction(), " opened=true");
                TwAddVarRW(mMainBar, it->name().append(" cut off angle").c_str(), TW_TYPE_FLOAT, & static_pointer_cast<SpotLight>(it)->cutoff(), "");
                break;

            default:
                assert(false && "Undefined light type");
                break;
        }
    }

    // Separator
    TwAddSeparator(mMainBar, "", nullptr);
}

void AntTweakBarWrapper::initRenderingTechnique(const vector<string> & pNames)
{
    TwAddButton(mMainBar, "Rendering techniques", nullptr, nullptr, "help='Rendering technique methods'");

    mRenderingTechniqueNames = pNames;

    string lAllNames;
    for (const auto it : mRenderingTechniqueNames)
    {
        lAllNames.append(it);
        lAllNames.append(",");
    }

    // Remove the last comma
    lAllNames.pop_back();

    // Defining new enum type
    TwType lRenderingTechniqueType = TwDefineEnumFromString("RenderingTechnique", lAllNames.c_str());

    // Adding density to bar
    TwAddVarRW(mMainBar, "List", lRenderingTechniqueType, & mRenderingTechniqueID, NULL);

    // Separator
    TwAddSeparator(mMainBar, "", nullptr);
}

float AntTweakBarWrapper::meshRotationIncrement(void) const noexcept
{
    return mIncrement;
}

bool AntTweakBarWrapper::autoRotateActive(void) const noexcept
{
    return mAutoRotate;
}

bool AntTweakBarWrapper::shouldResetMeshOrientation(void) const noexcept
{
    if (mResetOrientation)
    {
        mResetOrientation = false;
        return true;
    }
    else
    {
        return false;
    }
}

void AntTweakBarWrapper::keyboardCallback(int pKey)
{
    TwKeyPressed(pKey, TW_KMOD_NONE);
}

bool AntTweakBarWrapper::passiveMouseCallback(int pX, int pY)
{
    return TwMouseMotion(pX, pY);
}

void AntTweakBarWrapper::render(void)
{
    TwDraw();
}

bool AntTweakBarWrapper::mouseCallback(int pButton, bool pIsPressed)
{
    TwMouseButtonID lButton = (pButton == 0) ? TW_MOUSE_LEFT : TW_MOUSE_RIGHT;
    TwMouseAction lAction = (pIsPressed) ? TW_MOUSE_PRESSED : TW_MOUSE_RELEASED;

    return TwMouseButton(lAction, lButton);
}

void AntTweakBarWrapper::resizeWindow(unsigned int pFramebufferWidth, unsigned int pFramebufferHeight)
{
//    int lBarDimensions[2] = {0, 0};
//    TwGetParam(mMainBar, nullptr, "size", TW_PARAM_INT32, 2, lBarDimensions);
//
//    mMainBarDimensions.x() = static_cast<float>(lBarDimensions[0]) / static_cast<float>(pFramebufferWidth);
//    mMainBarDimensions.y() = static_cast<float>(lBarDimensions[1]) / static_cast<float>(pFramebufferHeight);
//
//    mMainBarDimensions.display();

    TwWindowSize(pFramebufferWidth, pFramebufferHeight);
    auto lSizeMessage = _createSizeMessage(mMainBarDimensions.x() * static_cast<float>(pFramebufferWidth), mMainBarDimensions.y() * mMainBarDimensions.y() * static_cast<float>(pFramebufferHeight));
    TwDefine(lSizeMessage.c_str());
}

string AntTweakBarWrapper::currentRenderingTechnique(void) const
{
    return mRenderingTechniqueNames.at(mRenderingTechniqueID);
}

int AntTweakBarWrapper::currentRenderingTechniqueIndex(void) const noexcept
{
    return mRenderingTechniqueID;
}

string AntTweakBarWrapper::_createSizeMessage(size_t pWidth, size_t pHeight)
{
    string lMessage(" miniGL size='");
    lMessage.append(to_string(pWidth));
    lMessage.append(" ");
    lMessage.append(to_string(pHeight));
    lMessage.append("'");

    return lMessage;
}

string AntTweakBarWrapper::_createColorMessage(size_t pRed, size_t pGreen, size_t pBlue)
{
    string lMessage(" miniGL color='");
    lMessage.append(to_string(pRed));
    lMessage.append(" ");
    lMessage.append(to_string(pGreen));
    lMessage.append(" ");
    lMessage.append(to_string(pBlue));
    lMessage.append("'");

    return lMessage;
}

void TW_CALL AntTweakBarWrapper::_autoRotateCallback(void* pParameters)
{
    mAutoRotate = !mAutoRotate;
}

void TW_CALL AntTweakBarWrapper::_resetOrientationCallback(void* pParameters)
{
    mResetOrientation = true;
}
