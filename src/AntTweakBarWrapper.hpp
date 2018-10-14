//===============================================================================================//
/*!
 *  \file      AntTweakBarWrapper.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

#include <vector>
#include <map>

#include <AntTweakBar.h>

#include "Algebra.hpp"
#include "MeshAndTransform.hpp"
#include "Camera.hpp"
#include "BaseLight.hpp"

namespace miniGL
{
    /*!
     *  \brief  This class is a wrapper on top of the Ant Tweak Bar library to be able to integrate it to
     *          this engine.
     *  \details AntTweakBar is a small and easy-to-use C/C++ library that allows programmers to quickly add a light and intuitive
     *           graphical user interface into graphic applications based on OpenGL (compatibility and core profiles), DirectX 9,
     *           DirectX 10 or DirectX 11 to interactively tweak parameters on-screen. See http://anttweakbar.sourceforge.net/doc/
     */
    class AntTweakBarWrapper
    {
    public:
        /*!
         *  \brief Initialize the main UI.
         *  @param pDimensionsAsFraction contain the dimensions of the bar as a fraction (in [0,1]) of the window size (i.e. framebuffer width and height)
         *  @param pFramebufferWidth is the width of the framebuffer in pixels
         *  @param pFramebufferHeight is the height of the framebuffer in pixels
         *  @param pColor contains the RGB coefficients defining the color of the UI (in [0,255] range)
         */
        void init(vec2f pDimensionsAsFraction, unsigned int pFramebufferWidth, unsigned int pFramebufferHeight, vec3f pColor);

        /*!
         *  \brief Initialize the camera section in the UI
         *  @param pCamera is a reference on the main camera
         */
        void initCamera(Camera & pCamera);

        /*!
         *  \brief Initialize the mesh rotation section in the UI
         *  @param pMeshOrientation is a reference on the quaternion that will control mesh rotations
         */
        void initRotation(quatf & pMeshOrientation);

        /*!
         *  \brief Initialize the lights in the UI
         *  @param pLights is a vector containing all the lights (directional, point or spot)
         */
        void initLights(std::vector<std::shared_ptr<BaseLight>> & pLights);

        /*!
         *  \brief Initialize the rendering techniques in the UI
         *  @param pNames is a vector containing all the names of the rendering techniques (each name MUST be unique in the container)
         */
        void initRenderingTechnique(const std::vector<std::string> & pNames);

        /*!
         *  \brief Get the increment for the current angle controlling mesh rotation
         *  @return the increment value as displayed in the UI
         */
        float meshRotationIncrement(void) const noexcept;

        /*!
         *  \brief Check if the auto rotate check box is ticked
         *  @return true if the auto rotate check box is ticked, false otherwise
         */
        bool autoRotateActive(void) const noexcept;

        /*!
         *  \brief Check if the reset mesh orientation check box was ticked
         *  @return true if it's the first time the method is called since the reset mesh orientation check box was ticked, false otherwise
         */
        bool shouldResetMeshOrientation(void) const noexcept;

        /*!
         *  \brief
         */
        void keyboardCallback(int pKey);

        /*!
         *  \brief Send mouse movement events to ATB
         *  @param pX is the horizontal position of the mouse in windows coordinates
         *  @param pY is the vertical position of the mouse in windows coordinates
         *  @return true if movement was done over by the GUI
         */
        bool passiveMouseCallback(int pX, int pY);

        /*!
         *  \brief Render the AntTweakBar window
         */
        void render(void);

        /*!
         *  \brief Send mouse button events to ATB
         *  @param pButton is 0 for left click, 1 for right click
         *  @param pIsPressed is true of the button is pressed, false otherwise
         *  @return true if the button click was used by the GUI
         */
        bool mouseCallback(int pButton, bool pIsPressed);

        /*!
         *  \brief Call this method when the window gets resized in order to resize the UI accordingly
         *  @param pFramebufferWidth is the width of the framebuffer in pixel
         *  @param pFramebufferHeight is the height of the framebuffer in pixel
         */
        void resizeWindow(unsigned int pFramebufferWidth, unsigned int pFramebufferHeight);

        /*!
         *  \brief Get the rendering technique currently active
         *  @return the name of the currently selected rendering technique.
         */
        std::string currentRenderingTechnique(void) const;

        /*!
         *  \brief Get the rendering technique currently active
         *  @return the index of the currently selected rendering technique.
         */
        int currentRenderingTechniqueIndex(void) const noexcept;

    private:
        std::string _createSizeMessage(size_t pWidth, size_t pHeight);
        std::string _createColorMessage(size_t pRed, size_t pGreen, size_t pBlue);

        /*!
         *  \brief Callback used by Ant Tweak Bar to activate/deactivate the rotation of the mesh
         */
        static void TW_CALL _autoRotateCallback(void* pParameters);

        /*!
         *  \brief Callback used by Ant Tweak Bar to reset the orientation of the mesh
         */
        static void TW_CALL _resetOrientationCallback(void* pParameters);

    private:
        TwBar* mMainBar = nullptr;
        std::vector<std::string> mMeshNames;
        std::vector<std::string> mRenderingTechniqueNames;
        TwStructMember mVec3fCoordinates[3];
        TwType mVec3fTwType;
        vec2f mMainBarDimensions;
        float mIncrement = 1.0f;

        int mRenderingTechniqueID = 0;

        static bool mAutoRotate;
        static bool mResetOrientation;
    }; // class AntTweakBarWrapper

} // namespace miniGL
