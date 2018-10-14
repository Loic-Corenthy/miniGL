//===============================================================================================//
/*!
 *  \file      Picking3D.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

#include <map>
#include <string>
#include <tuple>

#include "PickingRender.hpp"
#include "PickingTexture.hpp"
#include "SimpleColorRender.hpp"
#include "MeshAndTransform.hpp"
#include "Camera.hpp"
#include "Algebra.hpp"

namespace miniGL
{
    /*!
     *  \brief   This class encapsulates all the details to handle 3D picking
     *  \details Contains the picking render and the simple color render
     */
    class Picking3D
    {
    public:
        /*!
         *  \brief Initialize the rendering technique
         *  @param pFramebufferDimensions is the width and height of the framebuffer
         *  @param pWindowDimensions is the width and height of the window
         */
        void init(const std::tuple<int, int> & pFramebufferDimensions, const std::tuple<int, int> & pWindowDimensions);

        /*!
         *  \brief Picking phase of the render, i.e. determine the triangles located "under" the mouse
         *  @param pMeshes is a container with all the meshes
         */
        void pickingPhase(std::map<std::string, MeshAndTransform> & pMeshes);

        /*!
         *  \brief Render the meshes provided as parameter using this rendering technique
         *  @param pMeshes is a container with all the meshes
         *  @param pMousePosition contains the x and y coordinates of the mouse
         */
        void render(std::map<std::string, MeshAndTransform> & pMeshes, const vec2i & pMousePosition);

        /*!
         *  \brief Set a copy of the camera
         *  @param pCamera is a reference to the main camera in the scene
         */
        void camera(const std::shared_ptr<Camera> pCamera);

        /*!
         *  \brief Set the name of a mesh to be rendered by this technique
         *  @param pName is a name that will be matched with the mesh container (pMeshes) when calling the render method
         */
        void meshToRender(const std::string & pName);

    private:
        PickingTexture mPickingTexture;
        std::unique_ptr<PickingRender> mPickingRender;
        std::unique_ptr<SimpleColorRender> mSimpleColor;
        std::shared_ptr<Camera> mCamera;
        std::string mMeshToRenderName;
        vec2d mScaling;
        std::tuple<int, int> mFrameBufferDimensions = {0, 0};

    }; // class Picking3D

} // namespace miniGL
