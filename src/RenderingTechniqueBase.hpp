//===============================================================================================//
/*!
 *  \file      RenderingTechniqueBase.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

#include <vector>
#include <string>
#include <map>

#include "Camera.hpp"
#include "MeshAndTransform.hpp"

namespace miniGL
{
    /*!
     *  \brief   This class contains common methods to all rendering techniques
     *  \details It handles the camera, the names of meshes to render.
     */
    class RenderingTechniqueBase
    {
    public:
        /*!
         *  \brief Prevent deriving classes to use the default contructor to force them use the one
         *         with the name as parameter
         */
        RenderingTechniqueBase(void) = delete;

        /*!
         *  \brief Constructor with name
         *  @param pName is the name of a class deriving from this one
         */
        RenderingTechniqueBase(const std::string & pName);

        /*!
         *  \brief Set a copy of the camera
         *  @param pCamera is a reference to the main camera in the scene
         */
        void camera(const std::shared_ptr<Camera> pCamera);

        /*!
         *  \brief Add the name of a mesh to be rendered
         *  @param pName is a name that will be stored to be rendered later on by a class deriving from RenderingTechniqueBase
         */
        void addMeshToRender(const std::string & pName);

        /*!
         *  \brief Get the name of the meshes to be rendered
         *  @return a reference on a vector with all the names of the meshes to be rendered later on by
         *          a class deriving from RenderingTechniqueBase
         */
        const std::vector<std::string> & meshToRenderNames(void) const;

        /*!
         *  \brief Get the name of the rendering technique
         *  @return the name of the class that will derive from this one
         */
        std::string name(void) const noexcept;

    protected:
        /*!
         *  \brief Set the name of the rendering technique
         *  @param pName is the name of the class that will derive from this one
         */
        void name(const std::string & pName);

        /*!
         *  \brief Helper method to match the names of the meshes to render by this technique with
         *         all the meshes in the input container
         *  @param pMeshes is the container where this method will look for the meshes to render
         */
        std::vector<std::map<std::string, MeshAndTransform>::const_iterator> findMeshesToRender(const std::map<std::string, MeshAndTransform> & pMeshes) const;

    protected:
        std::vector<std::string> mMeshToRenderNames;
        std::shared_ptr<Camera> mCamera;
        std::string mName;

    }; // class RenderingTechniqueBase

} // namespace miniGL
