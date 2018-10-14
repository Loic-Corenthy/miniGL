//===============================================================================================//
/*!
 *  \file      MeshAOS.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

#include <vector>
#include <string>

#include <GL/glew.h>

#include <Importer.hpp>
#include <scene.h>
#include <postprocess.h>

#include "MeshBase.hpp"
#include "MeshBoneData.hpp"
#include "Vertex.hpp"
#include "CallbacksRender.hpp"
#include "VertexBoneData.hpp"

namespace miniGL
{
    /*!
     *  \brief This class handles a mesh as an "array of structures"
     *  \details This class uses the assimp library to facilitate the mesh processing
     */
    class MeshAOS : public MeshBase, public MeshBoneData
    {
    public:
        /*!
         *  \brief Default constructor
         */
        MeshAOS(void) = default;

        /*!
         *  \brief Constructor with name parameter
         *  @param pName is the name of the mesh
         */
        MeshAOS(const std::string & pName);

        /*!
         * \brief Destructor
         */
        virtual ~MeshAOS(void);

        /*!
         *  \brief Implementation of a virtual method from MeshBase
         */
        virtual bool load(const char* pFile, MeshBase::EOptions pOptions = MeshBase::EOptions::UNSET) final;

        /*!
         *  \brief Implementation of a virtual method from MeshBase
         */
        virtual void render(EPrimitiveType pPrimitive = EPrimitiveType::TRIANGLE, CallbacksRender* pRenderCallbacks = nullptr) final;

        /*!
         *  \brief Implementation of a virtual method from MeshBase
         */
        virtual void render(unsigned int pDrawIndex, unsigned int pPrimitiveIndex) final;

        /*!
         *  \brief Implementation of a virtual method from MeshBase
         */
        virtual void render(unsigned int pCount, const mat4f* pWVPs, const mat4f* pWorlds) final;

        /*!
         *  \brief Implementation of a virtual method from MeshBase
         */
        virtual void clear(void) final;

        /*!
         *  \brief Implementation of a virtual method from MeshBase
         */
        virtual void boneTransform(float pTime, std::vector<mat4f> & pTransforms) final;

        /*!
         *  \brief Implementation of a virtual method from MeshBase
         */
        virtual unsigned int boneCount(void) const noexcept final;

    private:
        struct MeshEntry
        {
            GLuint          vbo;
            GLuint          ibo;
            unsigned int numIndices;
            unsigned int materialIndex;

        }; // struct MeshEntry

    private:
        /*!
         *  \brief Helper method to get the number of entries and textures in the scene
         *  @param pScene is the scene created using assimp
         *  @param pFile is the entire path of the file
         */
        bool _initFromScene(const aiScene* pScene, const std::string & File);

        /*!
         *  \brief Helper method to load the vertices, indices and normals to openGL
         *  @param pIndex is the index of an object in the scene
         *  @param pAiMesh is the mesh of the corresponding object
         *  @param pPartialVertexCount is used to read from the correct start position in pBoneData
         *  @param pBoneData is an array containing the bone data for each vertex
         */
        void _initMesh(unsigned int pIndex, const aiMesh* pAiMesh, unsigned int pPartialVertexCount, std::vector<VertexBoneData<4>> & pBoneData);

        /*!
         *  \brief Clear the loaded textures
         */
        void _clear(void);

        /*!
         *  \brief Load vbo and ibo in openGL
         *  @param pMeshEntry is the mesh created using assimp
         *  @param pVertices contains all the vertices (vertex, normal, texture coordinates) of the mesh
         *  @param pIndices contains all the indices corresponding to the vertices of the mesh
         */
        void _initMeshEntry(MeshEntry & pMeshEntry, const std::vector<Vertex> & pVertices, const std::vector<unsigned int> & pIndices);

    private:
        std::vector<MeshEntry> mEntries;

    }; // class MeshAOS

    inline void MeshAOS::boneTransform(float pTime, std::vector<mat4f> & pTransforms)
    {
        MeshBoneData::boneTransform(mScene, pTime, pTransforms);
    }

    inline unsigned int MeshAOS::boneCount(void) const noexcept
    {
        return MeshBoneData::boneCount();
    }

} // namespace miniGL
