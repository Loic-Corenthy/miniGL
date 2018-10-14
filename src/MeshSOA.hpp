//===============================================================================================//
/*!
 *  \file      MeshSOA.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

#include <vector>
#include <map>
#include <string>
#include <array>

#include <GL/glew.h>

#include <Importer.hpp>
#include <scene.h>
#include <postprocess.h>

#include "MeshBase.hpp"
#include "MeshBoneData.hpp"
#include "Texture.hpp"
#include "CallbacksRender.hpp"
#include "Algebra.hpp"
#include "VertexBoneData.hpp"

namespace miniGL
{
    /*!
     *  \brief This class encapsulates the process of creating a mesh reading from several file types (.obj, ...)
     *  \details This class derives from MeshBase and MeshBoneData. It uses the assimp library to facilitate the mesh processing
     */
    class MeshSOA : public MeshBase, public MeshBoneData
    {
    public:
        /*!
         *  \brief Default constructor
         */
        MeshSOA(void);

        /*!
         *  \brief Constructor with name parameter
         *  @param pName is the name of the mesh
         */
        MeshSOA(const std::string & pName);

        /*!
         * \brief Destructor
         */
        virtual ~MeshSOA(void);

        /*!
         *  \brief Implementation of a virtual method from MeshBase
         */
        virtual bool load(const char* pFile, MeshBase::EOptions pOptions = MeshBase::EOptions::UNSET) final;

        /*!
         *  \brief Implementation of a virtual method from MeshBase
         */
        virtual void render(MeshBase::EPrimitiveType pPrimitive = MeshBase::EPrimitiveType::TRIANGLE, CallbacksRender* pRenderCallbacks = nullptr) final;

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
            unsigned int numIndices;
            unsigned int baseVertex;
            unsigned int baseIndex;
            unsigned int materialIndex;
        }; // struct MeshEntry

        enum class EAttributes
        {
            INDEX_BUFFER                            = 0,
            POSITION_VERTEX_BUFFER                  = 1,
            TEXTURE_COORDINATE_VERTEX_BUFFER        = 2,
            NORMAL_VERTEX_BUFFER                    = 3,
            TANGENT_VERTEX_BUFFER                   = 4,
            WVP_MATRIX_INSTANCED_VERTEX_BUFFER      = 5,
            WORLD_MATRIX_INSTANCED_VERTEX_BUFFER    = 6,
            BONE_VERTEX_BUFFER                      = 7
        };

    private:
        /*!
         *  \brief Helper method to get the number of entries and textures in the scene
         *  @param pScene is the scene created using assimp
         *  @param pFile is the entire path of the file
         *  @return true if the mesh was loaded correctly, false otherwise
         */
        bool _initFromScene(const aiScene* pScene, const std::string & File);

        /*!
         *  \brief Helper method to load the vertices, indices and normals to openGL
         *  @param pMeshIndex is the index of the current part of the mesh
         *  @param pMesh is the mesh of the corresponding object
         *  @param pPosition will contain the position of each vertex
         *  @param pNormal will contain the normal of each vertex
         *  @param pTexCoords will contain the texture coordinates of each vertex
         *  @param pBones will contain the transformation matrices and the indices of the bones associated
         *         to each vertex
         *  @param pIndices contain the list of indices to draw the vertices using draw elements
         */
        void _initMesh(unsigned int pMeshIndex, const aiMesh* pMesh, std::vector<vec3f> & pPosition, std::vector<vec3f> & pNormals, std::vector<vec2f> & pTexCoords, std::vector<VertexBoneData<4>> & pBones, std::vector<unsigned int> & pIndices);

        /*!
         *  \brief Helper method to load the vertices, indices and normals to openGL
         *  @param pMesh is the mesh of the corresponding object
         *  @param pPosition will contain the position of each vertex
         *  @param pNormal will contain the normal of each vertex
         *  @param pTexCoords will contain the texture coordinates of each vertex
         *  @param pIndices contain the list of indices to draw the vertices using draw elements
         */
        void _initMesh(const aiMesh* pMesh, std::vector<vec3f> & pPosition, std::vector<vec3f> & pNormals, std::vector<vec2f> & pTexCoords, std::vector<vec3f> & pTangents, std::vector<unsigned int> & pIndices);

    private:
        std::vector<MeshEntry> mEntries;
        std::array<GLuint, 8> mBuffers = {{0, 0, 0, 0, 0, 0, 0, 0}};

    }; // class MeshSOA

    inline void MeshSOA::boneTransform(float pTime, std::vector<mat4f> & pTransforms)
    {
        MeshBoneData::boneTransform(mScene, pTime, pTransforms);
    }

    inline unsigned int MeshSOA::boneCount(void) const noexcept
    {
        return MeshBoneData::boneCount();
    }

} // namespace miniGL
