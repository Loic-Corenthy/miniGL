//===============================================================================================//
/*!
 *  \file      MeshBase.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

#include <vector>
#include <string>
#include <cassert>

#include <GL/glew.h>

#include <Importer.hpp>
#include <scene.h>
#include <postprocess.h>

#include "Texture.hpp"
#include "CallbacksRender.hpp"
#include "Algebra.hpp"
#include "MeshAdjacencies.hpp"

namespace miniGL
{
    /*!
     *  \brief   This class is the base class for 3D meshes
     *  \details This class uses the assimp library to facilitate the mesh processing
     */
    class MeshBase
    {
    public:
        enum class EOptions
        {
            UNSET                   = 0b0001,
            COMPUTE_TANGENT_SPACE   = 0b0010,
            INSTANCE_RENDERING      = 0b0100,
            ADJACENCIES             = 0b1000
        };

        enum class EPrimitiveType
        {
            TRIANGLE = 0b01,
            PATCH    = 0b10
        };

    public:
        /*!
         *  \brief Default constructor
         */
        MeshBase(void) = default;

        /*!
         *  \brief Constructor with name parameter
         *  @param pName is the name of the MeshBase
         */
        MeshBase(const std::string & pName);

        /*!
         * \brief Destructor
         */
        virtual ~MeshBase(void);

        /*!
         * \brief Get a handle for the VAO associated to this mesh
         * @return the handle of the created VAO
         */
        GLuint createVAO(void);

        /*!
         * \brief Bind the VAO associated to this mesh
         * @param pIndex corresponds to the mesh entry
         */
        void bindVAO(unsigned int pIndex);

        /*!
         * \brief Call glBindVertexArray with parameter 0
         */
        void unbindVAO(void);

        /*!
         *  \brief Load the mesh from a file
         *  @param pFile is the entire path of the file
         *  @param pOptions is configured using the EOptions enum to set the various options
         *         offered by assimp library when loading a mesh
         *  @return true is the mesh was loaded correctly, false otherwise
         */
        virtual bool load(const char* pFile, EOptions pOptions = EOptions::UNSET) = 0;

        /*!
         *  \brief Render the loaded mesh
         *  \param pPrimitive is the type of primitive to pass to the draw call, e.g. triangles or patches.
         *  \param pRenderCallbacks is a pointer to an object implementing one of the method that can be executed
         *         before the draw call.
         */
        virtual void render(EPrimitiveType pPrimitive = EPrimitiveType::TRIANGLE, CallbacksRender* pRenderCallbacks = nullptr) = 0;

        /*!
         *  \brief Render only a specified triangle in the loaded mesh
         *  \param pDrawIndex in the index of the current draw call (corresponds to the VAO to use)
         *  \param pPrimitiveIndex is the index of the corresponding triangle in the mesh
         */
        virtual void render(unsigned int pDrawIndex, unsigned int pPrimitiveIndex) = 0;

        /*!
         *  \brief Apply instance rendering to the mesh
         *  \param pCount is the number of instances to draw
         *  \param pWVPs is an array containing the WVP matrices for each instance (as many as pCount)
         *  \param pWorlds is an array containing the world matrices for each instance (as many as pCount)
         */
        virtual void render(unsigned int pCount, const mat4f* pWVPs, const mat4f* pWorlds) = 0;

        /*!
         *  \brief Free all the memory loaded for the current mesh, reset all handles and state variables
         */
        virtual void clear(void) = 0;

        /*!
         *  \brief Get all the transformations associated to each bones for the current time
         *  @param pTime is in seconds
         *  @param pTransforms contains all the current transformation matrices
         */
        virtual void boneTransform(float pTime, std::vector<mat4f> & pTransforms) = 0;

        /*!
         *  \brief Get the number of bones
         *  @param return the number of bones
         */
        virtual unsigned int boneCount(void) const noexcept = 0;

        /*!
         * \brief Get the name of the mesh (read only)
         * @return a copy of mName
         */
        std::string name(void) const;

        /*!
         * \brief Set the name of the mesh
         * @param pValue is the value set to mName
         */
        void name(const std::string & pValue);

        /*!
         * \brief Set the orientation of front-facing polygons. By default, GL_CCW
         * @param pValue is GL_CCW or GL_CW.
         */
        void frontFace(GLenum pValue) noexcept;

        /*!
         * \brief Get the orientation of front-facing polygons. By default, GL_CCW
         * @return GL_CCW or GL_CW.
         */
        GLenum frontFace(void) const noexcept;

        /*!
         * \brief Get the option used when loading the mesh
         * @return an enum corresponding to the loading parameter
         */
        EOptions loadOption(void) const noexcept;

    protected:
        /*!
         *  \brief Helper method to load textures to openGL
         *  @param pScene is the scene created using assimp
         *  @param pFile is the entire path of the file
         */
        bool initMaterials(const aiScene* pScene, const std::string & pFile);

        /*!
         *  \brief Clear the loaded textures
         */
        void clearTextures(void);

        /*!
         *  \brief Free all the VAOs and set the handle to 0
         */
        void clearVAOs(void);

    protected:
        std::vector<Texture*> mTextures;
        std::string mName = std::string("");
        EOptions mLoadOptions = EOptions::UNSET;
        std::vector<GLuint> mVAOs;
        GLenum mOrientation = GL_CCW;

        MeshAdjacencies mAdjacencyTool;
        bool mWithAdjacencies = false;

        const aiScene* mScene = nullptr;
        Assimp::Importer mImporter;

    }; // class MeshBase

} // namespace miniGL
