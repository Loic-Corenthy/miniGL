//===============================================================================================//
/*!
 *  \file      Application.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

#include <iostream>
#include <array>
#include <map>
#include <vector>
#include <string>
#include <chrono>
#include <type_traits>

#include "CallbacksInterface.hpp"
#include "BaseBackend.hpp"
#include "Constants.hpp"
#include "Camera.hpp"
#include "Transform.hpp"
#include "Texture.hpp"
#include "SimpleLightingWithShadow.hpp"
#include "MeshBase.hpp"
#include "MeshAndTransform.hpp"
#include "Skybox.hpp"
#include "BillboardList.hpp"
#include "ParticleSystem.hpp"
#include "Picking3D.hpp"
#include "Tessellation.hpp"
#include "TessellationPN.hpp"
#include "InstancedLightingTechnique.hpp"
#include "GLFXTechnique.hpp"
#include "DeferredShadingTechnique.hpp"
#include "SkinningTechnique.hpp"
#include "SilhouetteTechnique.hpp"
#include "ShadowVolumeTechnique.hpp"
#include "MultipassShadowMapTechnique.hpp"
#include "SSAOTechnique.hpp"
#include "ShadowMapDirectionalLightTechnique.hpp"
#include "CascadedShadowMapDirectionalLightTechnique.hpp"
#include "AntTweakBarWrapper.hpp"

namespace miniGL
{
    /*!
     *  \brief This class stores, initialize and run the different rendering techniques available in miniGL
     *  \details This class encapsulates a window and all the different effects that the application offers
     */
    class Application : public CallbacksInterface
    {
    private:
        enum class ELogMessage
        {
            COMMENT, /*! The message to save in the log file is a simple commment */
            ERROR    /*! The message to save in the log file is an error message */
        };

    public:
        /*!
         *  \brief Default constructor
         */
        Application(void);

        /*!
         *  \brief Destructor
         */
        ~Application(void);

        /*!
         *  \brief Do all the initialization for the application
         *  @param pWindowWidth is the width of the window in pixels
         *  @param pWindowHeight is the height of the window in pixels
         *  @param pTitle is the title of the window of the application
         */
        void init(unsigned int pWindowWidth, unsigned int pWindowHeight, bool pFullScreen, const std::string & pTitle = std::string("mini GL engine"));

        /*!
         *  \brief Run the application
         */
        void run(void);

        /*!
         *  \brief Virtual method from CallbacksInterface
         */
        virtual void keyboardCallBack(int pKey, int pScancode, int pAction, int pMods) final;

        /*!
         *  \brief Virtual method from CallbacksInterface
         */
        virtual void passiveMouseCallBack(int pX, int pY) final;

        /*!
         *  \brief Virtual method from CallbacksInterface
         */
        virtual void mouseCallBack(int pButton, bool pIsPressed) final;

        /*!
         *  \brief Virtual method from CallbacksInterface
         */
        virtual void pickingPhaseCallback(void) final;

        /*!
         *  \brief Virtual method from CallbacksInterface
         */
        virtual void renderPhaseCallBack(void) final;

        /*!
         *  \brief Virtual method from CallbacksInterface
         */
        virtual void errorCallback(int pError, const char* pDescription) final;

        /*!
         *  \brief Virtual method from CallbacksInterface
         */
        virtual void messageCallback(const std::string & pMessage) const final;

        /*!
         *  \brief Virtual method from CallbacksInterface
         */
        virtual void frameBufferSizeCallback(int pWidth, int pHeight) final;

    private:
        /*!
         *  \brief Helper method to validate a program with a specific mesh
         *  @param pProgram is a pointer on the program to validate
         *  @param pName is the name of the mesh
         */
        void _validateShaderWithMesh(Program* pProgram, const std::string & pName);

        /*!
         *  \brief Create a new mesh and stores the pointer in the map containing all the meshes. Calls load on the newly allocated mesh.
         *         If a mesh with the same same was already in the map, delete the newly created pointer
         *  @param pName is the name of the mesh
         *  @param pFile is the filename to load the mesh
         *  @param pFrontFace is either GL_CW or GL_CCW
         *  @param pOption can be use to compute the tangent space for the mesh or create a mesh for instanced rendering
         */
        template <typename T>
        void _createMesh(const std::string & pName, const std::string & pFile, GLenum pFrontFace, MeshBase::EOptions pOption = MeshBase::EOptions::UNSET);

        /*!
         *  \brief Helper method to load the different meshes that will be available at run time
         */
        void _loadMeshes(void);

        /*!
         *  \brief Helper method to create lights for the different scenes
         */
        void _createLights(unsigned int pPointLightCount, unsigned int pSpotLightCount);

        /*!
         *  \brief Helper method check if the light really have the type that is used in _createLights before setting parameters.
         *         This method ASSERTS if the type is correct, use it while compiling in debug
         */
        void _checkLightsType(void);

        /*!
         *  \brief Helper method to initialise the appropriate rendering technique, according to the
         *         current rendering technique index
         */
        void _initCurrentRenderingTechnique(void);

        /*!
         *  \brief Helper method to deactivate the appropriate rendering technique, according to the
         *         previous rendering technique index.
         */
        void _deactivateRenderingTechniques(void);

        /*!
         *  \brief Helper method to initialize a simple lighting with shadows for 1 spot light (using shadow mapping)
         */
        void _initSimpleLighting(void);

        /*!
         *  \brief Helper method to initialize the skybox
         */
        void _initSkybox(void);

        /*!
         *  \brief Helper method to initialize the billboarding
         */
        void _initBillboarding(void);

        /*!
         *  \brief Helper method to initialize the particle system
         */
        void _initParticleSystem(void);

        /*!
         *  \brief Helper method to initialize the 3D picking
         */
        void _init3DPicking(void);

        /*!
         *  \brief Helper method to initialize the tessellation
         */
        void _initTessellation(void);

        /*!
         *  \brief Helper method to initialize the tessellation point normal
         */
        void _initTessellationPN(void);

        /*!
         *  \brief Helper method to initialize the example with instance rendering
         */
        void _initInstancedRendering(void);

        /*!
         *  \brief Helper method to initialize the example using GLFX
         */
        void _initGLFXExample(void);

        /*!
         *  \brief Helper method to initialize the example using deferred shading
         */
        void _initDeferredShading(void);

        /*!
         *  \brief Helper method to initialize the example using skeletal animation
         */
        void _initSkeletalAnimation(void);

        /*!
         *  \brief Helper method to initialize the example using silhouette detection
         */
        void _initSilhouetteDetection(void);

        /*!
         *  \brief Helper method to initialize the example using shadow volume rendering
         */
        void _initShadowVolume(void);

        /*!
         *  \brief Helper method to initialize the example using skeletal animation with motion blur
         */
        void _initSkeletalAnimationWithMotionBlur(void);

        /*!
         *  \brief Helper method to initialize the example using skeletal animation with motion blur
         */
        void _initMultipassShadowMapping(void);

        /*!
         *  \brief Helper method to initialize the example using Screen Space Ambient Occlusion
         */
        void _initSSAO(void);

        /*!
         *  \brief Helper method to initialize the example using Shadow Mapping for a directional light
         */
        void _initShadowMapDirectionalLight(void);

        /*!
         *  \brief Helper method to initialize the example using cascade shadow mapping
         */
        void _initCascadedShadowMapping(void);

        /*!
         *  \brief Helper method to illustrate multiple examples at once
         */
        void _initMultipleExamples(void);

        /*!
         *  \brief Helper method to initialize the example using the Ant Tweak Bar library
         */
        void _initATB(void);

        /*!
         *  \brief Helper method to render with the Simple Lighting with Shadow technique
         */
        void _renderSimpleLighting(void);

        /*!
         *  \brief Helper method to render the billboards
         */
        void _renderBillboarding(void);

        /*!
         *  \brief Helper method to render the particle system
         */
        void _renderParticleSystem(void);

        /*!
         *  \brief Helper method to render the spider illustrating the 3D picking technique
         */
        void _render3DPicking(void);

        /*!
         *  \brief Helper method to render terrain with tessellation
         */
        void _renderTessellation(void);

        /*!
         *  \brief Helper method to render terrain with tessellation point normal
         */
        void _renderTessellationPN(void);

        /*!
         *  \brief Helper method to render using instanced rendering
         */
        void _renderInstancedRendering(void);

        /*!
         *  \brief Helper method to render using the GLFX library
         */
        void _renderGLFXExample(void);

        /*!
         *  \brief Helper method to render using deferred shading
         */
        void _renderDeferredShading(void);

        /*!
         *  \brief Helper method to render using skinning technique (associated to skeletal animation)
         */
        void _renderSkeletalAnimation(void);

        /*!
         *  \brief Helper method to render silhouettes
         */
        void _renderSilhouetteDetection(void);

        /*!
         *  \brief Helper method to render using shadow volume technique (associated to skeletal animation)
         */
        void _renderShadowVolume(void);

        /*!
         *  \brief Helper method to render the example using skeletal animation with motion blur
         */
        void _renderMultipassShadowMapping(void);

        /*!
         *  \brief Helper method to render the example using Screen Space Ambient Occlusion
         */
        void _renderSSAO(void);

        /*!
         *  \brief Helper method to render the example using Shadow Mapping for a directional light
         */
        void _renderShadowMapDirectionalLight(void);

        /*!
         *  \brief Helper method to render the example using cascade shadow mapping
         */
        void _renderCascadedShadowMapping(void);

        /*!
         *  \brief Helper method to render multiple examples at once
         */
        void _renderMultipleExamples(void);

        /*!
         *  \brief Save several parameters concerning the graphic card and openGL properties in the log file
         */
        void _saveGLParametersInLogFile(void) const;

    private:
        std::unique_ptr<BaseBackend> mWindow;
        std::shared_ptr<Camera> mCamera;

        std::unique_ptr<ParticleSystem> mParticleSystem;
        std::unique_ptr<SimpleLightingWithShadow> mSimpleLightingWithShadow;
        std::unique_ptr<SkyBox> mSkyBox;
        std::unique_ptr<BillboardList> mBillboardList;
        std::unique_ptr<Picking3D> mPicking3D;
        std::unique_ptr<Tessellation> mTessellation;
        std::unique_ptr<TessellationPN> mTessellationPN;
        std::unique_ptr<InstancedLightingTechnique> mInstancedLighting;
        std::unique_ptr<GLFXTechnique> mGLFXTechnique;
        std::unique_ptr<DeferredShadingTechnique> mDeferredShading;
        std::unique_ptr<SkinningTechnique> mSkinningTechnique;
        std::unique_ptr<SilhouetteTechnique> mSilhouetteTechnique;
        std::unique_ptr<ShadowVolumeTechnique> mShadowVolumeTechnique;
        std::unique_ptr<MultipassShadowMapTechnique> mMultipassShadowMapTechnique;
        std::unique_ptr<SSAOTechnique> mSSAOTechnique;
        std::unique_ptr<ShadowMapDirectionalLightTechnique> mShadowMapDirectionalLightTechnique;
        std::unique_ptr<CascadedShadowMapDirectionalLightTechnique> mCascadedShadowMapDirectionalLightTechnique;

        AntTweakBarWrapper mATB;

        std::map<std::string, MeshAndTransform> mMeshes;
        std::vector<std::shared_ptr<BaseLight>> mLights;

        std::chrono::high_resolution_clock::time_point mCurrentTime;
        std::string mLogFile = std::string("./littlegl.log");
        quatf mMeshOrientation;
        vec2i mMousePos = vec2i(0);
        degreef mRotationAngle = 0.0f;
        float mCameraStep = 1.0f;
        int mCurrentRenderingTechniqueIndex = 0;
        int mPreviousRenderingTechniqueIndex = 0;
        bool mCtrlKeyPressed = false;
        bool mPickingOn = false;
        bool mIsWireframe = false;
        bool mDisplayCurrentPixelColor = false;
        bool mMouseButtonIsPressed = false;
        bool mWindowWasResized = false;

    }; // class Application

    template <typename T>
    void Application::_createMesh(const std::string & pName, const std::string & pFile, GLenum pFrontFace, MeshBase::EOptions pOption)
    {
        static_assert(std::is_base_of<MeshBase, T>::value, "The mesh class must derive from MeshBase");

        MeshAndTransform lMeshContainer;

        lMeshContainer.mesh = std::make_shared<T>(pName);
        auto lResult = mMeshes.emplace(pName, lMeshContainer);

        if (lResult.second)
        {
            mMeshes[pName].mesh->load(pFile.c_str(), pOption);
            mMeshes[pName].mesh->frontFace(pFrontFace);
        }
        else
        {
            std::cerr << "Mesh " << pName << " not created because it is already in the mesh container" << std::endl;
        }
    }

} // namespace miniGL
