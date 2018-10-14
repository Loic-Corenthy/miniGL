//===============================================================================================//
/*!
 *  \file      Mesh.cpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#include "MeshAOS.hpp"

#include <cassert>
#include <iostream>

#include "Constants.hpp"
#include "Exceptions.hpp"
#include "EngineCommon.hpp"
#include "Log.hpp"

using std::vector;
using std::string;
using Assimp::Importer;
using miniGL::Vertex;
using miniGL::MeshBase;
using miniGL::MeshAOS;
using miniGL::Constants;
using miniGL::Exceptions;
using miniGL::CallbacksRender;
using miniGL::VertexBoneData;
using miniGL::Log;

MeshAOS::MeshAOS(const std::string & pName)
:MeshBase(pName),
 MeshBoneData()
{

}

MeshAOS::~MeshAOS(void)
{
    clear();
}

bool MeshAOS::load(const char* pFile, MeshBase::EOptions pOptions)
{
    // Release the previously loaded mesh (if it exists)
    clear();

    // Save the options used to load the mesh
    mLoadOptions = pOptions;

    string lFilename(pFile);

    switch (pOptions)
    {
        case EOptions::UNSET:
            mScene = mImporter.ReadFile(lFilename.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);
            break;

        case EOptions::COMPUTE_TANGENT_SPACE:
            mScene = mImporter.ReadFile(lFilename.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
            break;

        case EOptions::ADJACENCIES:
            mScene = mImporter.ReadFile(lFilename.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);
            mWithAdjacencies = true;
            break;

        default:
            assert(false && "Wrong enum options in loading mesh");
            break;
    }

    if(!mScene)
        throw Exceptions(mImporter.GetErrorString(), __FILE__, __LINE__);

    // Copy root node transformation as inverse transformation
    MeshBoneData::globalInverseTransform(mScene->mRootNode->mTransformation);

    bool lResult = _initFromScene(mScene, lFilename);

    unbindVAO();

    return lResult;
}

void MeshAOS::render(EPrimitiveType pPrimitive, CallbacksRender* pRenderCallbacks)
{
    glFrontFace(mOrientation);

    for (unsigned int i = 0; i < mEntries.size(); i++)
    {
        bindVAO(i);

        if (mEntries[i].materialIndex < mTextures.size() && mTextures[mEntries[i].materialIndex] != nullptr)
            mTextures[mEntries[i].materialIndex]->bind(COLOR_TEXTURE_UNIT);

        if (pRenderCallbacks != nullptr)
            pRenderCallbacks->drawStartCallback(i);

        switch (pPrimitive)
        {
            case EPrimitiveType::TRIANGLE:
            {
                const auto lTopology = mWithAdjacencies ? GL_TRIANGLES_ADJACENCY : GL_TRIANGLES;
                glDrawElements(lTopology, mEntries[i].numIndices, GL_UNSIGNED_INT, 0);
            }   break;

            case EPrimitiveType::PATCH:
                glDrawElements(GL_PATCHES, mEntries[i].numIndices, GL_UNSIGNED_INT, 0);
                break;

            default:
                assert(false && "primitive type not supported");
                break;
        }

        unbindVAO();
    }
}

void MeshAOS::render(unsigned int pDrawIndex, unsigned int pPrimitiveIndex)
{
    assert(pDrawIndex < mEntries.size() && "Wrong index ");

    glFrontFace(mOrientation);

    bindVAO(pDrawIndex);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, reinterpret_cast<const GLvoid*>(pPrimitiveIndex * 3 * sizeof(GLuint)));
    unbindVAO();
}

void MeshAOS::render(unsigned int pCount, const mat4f* pWVPs, const mat4f* pWorlds)
{
    assert(false && "Instanced rendering not implemented yet!");

    glFrontFace(mOrientation);
}

bool MeshAOS::_initFromScene(const aiScene* pScene, const string & pFile)
{
    // Initalize the vectors storing the entries and textures with default (empty) values
    MeshEntry lDefault = { 0, 0, 0, Constants::invalidMaterial<GLuint>() };

    mEntries.resize(pScene->mNumMeshes, lDefault);
    mTextures.resize(pScene->mNumMaterials,nullptr);

    // Get the number of vertices per entry in the mesh
    vector<unsigned int> lPartialVertexCount;
    lPartialVertexCount.reserve(mEntries.size());

    // We must add 0 here so that the first partial count is zero
    // (It does not affect the total count)
    lPartialVertexCount.push_back(0);

    // Accumulate the number of vertices
    for (unsigned int i = 0; i < mEntries.size(); ++i)
        lPartialVertexCount.push_back(lPartialVertexCount.back() + pScene->mMeshes[i]->mNumVertices);

    // Get the total number of vertices in the mesh
    unsigned int lVertexCount = lPartialVertexCount.back();

    vector<VertexBoneData<4>> lBoneData;
    lBoneData.resize(lVertexCount);

    // Initialize the meshes in the scene one by one
    for (unsigned int i = 0 ; i < mEntries.size() ; i++)
    {
        const aiMesh* paiMesh = pScene->mMeshes[i];

        // Create a VAO for this mesh
        createVAO();
        bindVAO(i);
        MeshBoneData::loadBones(lPartialVertexCount[i], paiMesh, lBoneData);
        _initMesh(i, paiMesh, lPartialVertexCount[i], lBoneData);
        unbindVAO();
    }

    return initMaterials(pScene, pFile);
}

void MeshAOS::_initMesh(unsigned int pIndex, const aiMesh* pMesh, unsigned int pPartialVertexCount, vector<VertexBoneData<4>> & pBoneData)
{
    mEntries[pIndex].materialIndex = pMesh->mMaterialIndex;

    vector<Vertex> lVertices;
    vector<unsigned int> lIndices;
    const unsigned int lVerticesPerPrimitive = mWithAdjacencies? 6 : 3;

    lVertices.reserve(pMesh->mNumVertices);
    lIndices.reserve(pMesh->mNumFaces * lVerticesPerPrimitive);

    const aiVector3D lZero3D(0.0f, 0.0f, 0.0f);

    // Saves all the vertices in a vector
    for (unsigned int i = 0; i < pMesh->mNumVertices; i++)
    {
        const aiVector3D* rPos = & pMesh->mVertices[i];
        const aiVector3D* rTexCoord = pMesh->HasTextureCoords(0) ? &(pMesh->mTextureCoords[0][i]) : & lZero3D;
        const aiVector3D* rNormal = & pMesh->mNormals[i];

        Vertex lVertex(vec3f({rPos->x, rPos->y, rPos->z}),
                       vec2f({rTexCoord->x, rTexCoord->y}),
                       vec3f({rNormal->x, rNormal->y, rNormal->z}));

        if(pMesh->mTangents != nullptr)
        {
            const aiVector3D* rTangent = & pMesh->mTangents[i];
            lVertex.tangent(vec3f({rTangent->x, rTangent->y, rTangent->z}));
        }

        lVertices.push_back(lVertex);
    }

    for (unsigned int i = 0; i < pMesh->mNumVertices; ++i)
        lVertices[i].boneData() = pBoneData[pPartialVertexCount + i];

    // When loading indices with adjacencies, there are 6 indices per triangle
    if (mWithAdjacencies)
    {
        mAdjacencyTool.findAdjacencies(pMesh, lIndices);
    }
    else
    {
        // Saves all the indices
        for (unsigned int i = 0; i < pMesh->mNumFaces; i++)
        {
            const aiFace & rFace = pMesh->mFaces[i];

            assert(rFace.mNumIndices == 3);

            lIndices.push_back(rFace.mIndices[0]);
            lIndices.push_back(rFace.mIndices[1]);
            lIndices.push_back(rFace.mIndices[2]);
        }
    }

    // Send those vbo and ibo to openGL
    _initMeshEntry(mEntries[pIndex], lVertices, lIndices);
}

void MeshAOS::clear(void)
{
    for(unsigned int i = 0; i < mVAOs.size(); ++i)
    {
        if (mVAOs[i] == 0)
            continue;

        bindVAO(i);

        switch (mLoadOptions)
        {
            case EOptions::UNSET:
                glDisableVertexAttribArray(0);
                glDisableVertexAttribArray(1);
                glDisableVertexAttribArray(2);
                break;

            case EOptions::COMPUTE_TANGENT_SPACE:
                glDisableVertexAttribArray(0);
                glDisableVertexAttribArray(1);
                glDisableVertexAttribArray(2);
                glDisableVertexAttribArray(3);
                break;

            case EOptions::ADJACENCIES:
            case EOptions::INSTANCE_RENDERING:
                Log::consoleMessage("Those options are not supported yet");
                break;
        }

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        unbindVAO();
    }

    clearTextures();
    clearVAOs();

    mEntries.clear();
}

void MeshAOS::_initMeshEntry(MeshEntry & pMeshEntry, const vector<Vertex> & pVertices, const vector<unsigned int>& pIndices)
{
    pMeshEntry.numIndices = static_cast<unsigned int>(pIndices.size());

    glGenBuffers(1, &pMeshEntry.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, pMeshEntry.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)* pVertices.size(), pVertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &pMeshEntry.ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pMeshEntry.ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * pMeshEntry.numIndices, pIndices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const GLvoid*>(12));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const GLvoid*>(20));

    if (mLoadOptions == MeshBase::EOptions::COMPUTE_TANGENT_SPACE)
    {
        glEnableVertexAttribArray(3);
         glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const GLvoid*>(32)); // tangent
    }

    if (MeshBoneData::boneCount() > 0)
    {
        glEnableVertexAttribArray(4);
        glVertexAttribIPointer(4, 4, GL_INT, sizeof(Vertex), reinterpret_cast<const GLvoid*>(44));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const GLvoid*>(60));
    }

}
