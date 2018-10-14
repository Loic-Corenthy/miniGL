//===============================================================================================//
/*!
 *  \file      MeshSOA.cpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#include "MeshSOA.hpp"

#include <cassert>
#include <iostream>

#include "Constants.hpp"
#include "Exceptions.hpp"
#include "EngineCommon.hpp"
#include "EnumClassCast.hpp"
#include "GLUtils.hpp"
#include "Transform.hpp"

using std::vector;
using std::string;
using std::cout;
using std::endl;
using Assimp::Importer;
using miniGL::MeshBase;
using miniGL::MeshSOA;
using miniGL::Constants;
using miniGL::Exceptions;
using miniGL::Transform;
using miniGL::CallbacksRender;
using miniGL::VertexBoneData;

MeshSOA::MeshSOA(void)
:MeshBase()
{
}

MeshSOA::MeshSOA(const std::string & pName)
:MeshBase(pName),
 MeshBoneData()
{

}

MeshSOA::~MeshSOA(void)
{
    clear();
}

bool MeshSOA::load(const char* pFile, MeshBase::EOptions pOptions)
{
    // Release the previously loaded mesh (if it exists)
    clear();

    /*! \warning "Should we do this after generating and binding the VAOS?" */

    // Create buffers with ids to access the vertices attributes
    glGenBuffers(mBuffers.size(), mBuffers.data());

    // Save the options used to load the mesh
    mLoadOptions = pOptions;

    string lFilename(pFile);

    switch (pOptions)
    {
        case EOptions::UNSET:
        case EOptions::INSTANCE_RENDERING:
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

void MeshSOA::render(EPrimitiveType pPrimitive, CallbacksRender* pRenderCallbacks)
{
    glFrontFace(mOrientation);

    for (unsigned int i = 0; i < mEntries.size(); ++i)
    {
        bindVAO(i);

        const unsigned int lMaterialIndex = mEntries[i].materialIndex;

        assert(lMaterialIndex < mTextures.size() && "Material index out of boundaries in MeshSOA::render");

        if(lMaterialIndex < mTextures.size() && mTextures[lMaterialIndex] != nullptr)
            mTextures[lMaterialIndex]->bind(COLOR_TEXTURE_UNIT);

        if(pRenderCallbacks != nullptr)
            pRenderCallbacks->drawStartCallback(i);

        switch(pPrimitive)
        {
            case EPrimitiveType::TRIANGLE:
            {
                const auto lTopology = mWithAdjacencies ? GL_TRIANGLES_ADJACENCY : GL_TRIANGLES;
                glDrawElementsBaseVertex(lTopology, mEntries[i].numIndices, GL_UNSIGNED_INT, reinterpret_cast<void*>(sizeof(unsigned int) * mEntries[i].baseIndex), mEntries[i].baseVertex);
            }    break;

            case EPrimitiveType::PATCH:
                glDrawElementsBaseVertex(GL_PATCHES, mEntries[i].numIndices, GL_UNSIGNED_INT, reinterpret_cast<void*>(sizeof(unsigned int) * mEntries[i].baseIndex), mEntries[i].baseVertex);
                glDrawElements(GL_PATCHES, mEntries[i].numIndices, GL_UNSIGNED_INT, 0);
                break;

            default:
                assert(false && "primitive type not supported");
                break;
        }

        unbindVAO();
    }
}

void MeshSOA::render(unsigned int pDrawIndex, unsigned int pPrimitiveIndex)
{
    /** \todo Method not tested yet */
    assert(pDrawIndex < mEntries.size() && "Wrong index ");

    glFrontFace(mOrientation);

    bindVAO(pDrawIndex);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, reinterpret_cast<const GLvoid*>(pPrimitiveIndex * 3 * sizeof(GLuint)));
    unbindVAO();
}

void MeshSOA::render(unsigned int pCount, const mat4f* pWVPs, const mat4f* pWorlds)
{
    glBindBuffer(GL_ARRAY_BUFFER, mBuffers[toUT(EAttributes::WVP_MATRIX_INSTANCED_VERTEX_BUFFER)]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(mat4f) * pCount, pWVPs, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, mBuffers[toUT(EAttributes::WORLD_MATRIX_INSTANCED_VERTEX_BUFFER)]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(mat4f) * pCount, pWorlds, GL_DYNAMIC_DRAW);

    glFrontFace(mOrientation);

    for (unsigned int i = 0; i < mEntries.size(); ++i)
    {
        bindVAO(i);

        const unsigned int lMaterialIndex = mEntries[i].materialIndex;

        assert(lMaterialIndex < mTextures.size() && "Material index out of boundaries in MeshSOA::render");

        if(lMaterialIndex < mTextures.size() && mTextures[lMaterialIndex] != nullptr)
            mTextures[lMaterialIndex]->bind(COLOR_TEXTURE_UNIT);

        glDrawElementsInstancedBaseVertex(GL_TRIANGLES, mEntries[i].numIndices, GL_UNSIGNED_INT, reinterpret_cast<void*>(sizeof(unsigned int) * mEntries[i].baseIndex), pCount, mEntries[i].baseVertex);

        unbindVAO();
    }
}

void MeshSOA::clear(void)
{
    for(unsigned int i = 0; i < mVAOs.size(); ++i)
    {
        if (mVAOs[i] == 0)
            continue;

        bindVAO(i);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);

        if (mLoadOptions == EOptions::COMPUTE_TANGENT_SPACE)
            glDisableVertexAttribArray(3);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        unbindVAO();
    }

    clearTextures();
    clearVAOs();

    mEntries.clear();

    for (unsigned int i = 0; i < mBuffers.size(); ++i)
    {
        if(mBuffers[i] != 0)
            glDeleteBuffers(1, & mBuffers[i]);
    }
}

bool MeshSOA::_initFromScene(const aiScene* pScene, const string & pFile)
{
    // Initalize the vectors storing the entries and textures with default (empty) values
    MeshEntry lDefault = { 0, 0, 0, Constants::invalidMaterial<unsigned int>() };

    mEntries.resize(pScene->mNumMeshes, lDefault);
    mTextures.resize(pScene->mNumMaterials, nullptr);

    vector<vec3f> lPositions;
    vector<vec3f> lNormals;
    vector<vec2f> lTexCoords;
    vector<vec3f> lTangents;
    vector<VertexBoneData<4>> lBones;
    vector<unsigned int> lIndices;

    unsigned int lVertexCount = 0;
    unsigned int lIndexCount = 0;

    const unsigned int lVerticesPerPrimitive = mWithAdjacencies ? 6 : 3;

    for (unsigned int i = 0; i < mEntries.size(); ++i)
    {
        mEntries[i].materialIndex = pScene->mMeshes[i]->mMaterialIndex;
        mEntries[i].numIndices = pScene->mMeshes[i]->mNumFaces * lVerticesPerPrimitive;
        mEntries[i].baseVertex = lVertexCount;
        mEntries[i].baseIndex = lIndexCount;

        lVertexCount += pScene->mMeshes[i]->mNumVertices;
        lIndexCount += mEntries[i].numIndices;
    }

    // Reserve memory in container for the vertex attributes and indices
    lPositions.reserve(lVertexCount);
    lNormals.reserve(lVertexCount);
    lTexCoords.reserve(lVertexCount);

    if (mLoadOptions == MeshBase::EOptions::COMPUTE_TANGENT_SPACE)
        lTangents.reserve(lVertexCount);

    lBones.resize(lVertexCount);

    lIndices.reserve(lIndexCount);

    for (unsigned int i = 0; i < mEntries.size(); ++i)
    {
        const aiMesh* rAiMesh = pScene->mMeshes[i];

        switch (mLoadOptions)
        {
            case EOptions::UNSET :
            case EOptions::INSTANCE_RENDERING:
            case EOptions::ADJACENCIES:
                _initMesh(i, rAiMesh, lPositions, lNormals, lTexCoords, lBones, lIndices);
                break;

            case EOptions::COMPUTE_TANGENT_SPACE :
                _initMesh(rAiMesh, lPositions, lNormals, lTexCoords, lTangents, lIndices);
                break;

            default:
                assert(false && "Wrong enum options in loading mesh");
                break;
        }

        createVAO();
        bindVAO(i);

        // Generate and populate the buffers with the vertex attributes and indices
        glBindBuffer(GL_ARRAY_BUFFER, mBuffers[toUT(EAttributes::POSITION_VERTEX_BUFFER)]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vec3f) * lPositions.size(), lPositions.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        checkOpenGLState;

        glBindBuffer(GL_ARRAY_BUFFER, mBuffers[toUT(EAttributes::TEXTURE_COORDINATE_VERTEX_BUFFER)]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vec2f) * lTexCoords.size(), lTexCoords.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
        checkOpenGLState;

        glBindBuffer(GL_ARRAY_BUFFER, mBuffers[toUT(EAttributes::NORMAL_VERTEX_BUFFER)]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vec3f) * lNormals.size(), lNormals.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
        checkOpenGLState;

        if (mLoadOptions == MeshBase::EOptions::COMPUTE_TANGENT_SPACE)
        {
            glBindBuffer(GL_ARRAY_BUFFER, mBuffers[toUT(EAttributes::TANGENT_VERTEX_BUFFER)]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vec3f) * lTangents.size(), lTangents.data(), GL_STATIC_DRAW);
            glEnableVertexAttribArray(3);
            glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
            checkOpenGLState;
        }

        // Add attributes for skinning if the model has bones
        if (MeshBoneData::boneCount() > 0)
        {
            glBindBuffer(GL_ARRAY_BUFFER, mBuffers[toUT(EAttributes::BONE_VERTEX_BUFFER)]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(VertexBoneData<4>) * lBones.size(), lBones.data(), GL_STATIC_DRAW);
            glEnableVertexAttribArray(12);
            glVertexAttribIPointer(12, 4, GL_INT, sizeof(VertexBoneData<4>), reinterpret_cast<const GLvoid*>(0));
            glEnableVertexAttribArray(13);
            glVertexAttribPointer(13, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData<4>), reinterpret_cast<const GLvoid*>(16));
        }

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBuffers[toUT(EAttributes::INDEX_BUFFER)]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * lIndices.size(), lIndices.data(), GL_STATIC_DRAW);
        checkOpenGLState;

        if (mLoadOptions == MeshBase::EOptions::INSTANCE_RENDERING)
        {
            const GLuint lWVPLocation = 4;
            const GLuint lWorldLocation = 8;

            glBindBuffer(GL_ARRAY_BUFFER, mBuffers[toUT(EAttributes::WVP_MATRIX_INSTANCED_VERTEX_BUFFER)]);

            for (GLuint i = 0; i < 4; ++i)
            {
                glEnableVertexAttribArray(lWVPLocation + i);
                glVertexAttribPointer(lWVPLocation + i, 4, GL_FLOAT, GL_FALSE, sizeof(mat4f), reinterpret_cast<GLvoid*>(sizeof(GLfloat) * i * 4));
                // The function glVertexAttribDivisor() is what makes this an instance data rather than vertex data. It takes two parameters - the first one is the vertex array attribute and the second tells OpenGL the rate by which the attribute advances during instanced rendering.
                // It basically means the number of times the entire set of vertices is rendered before the attribute is updated from the buffer.
                // By default, the divisor is zero. This causes regular vertex attributes to be updated from vertex to vertex. If the divisor is 10 it means that the first 10 instances will use the first piece of data from the buffer, the next 10 instances will use the second, etc.
                // We want to have a dedicated WVP matrix for each instance so we use a divisor of 1. (http://ogldev.atspace.co.uk/www/tutorial33/tutorial33.html)
                glVertexAttribDivisor(lWVPLocation + i, 1);
                checkOpenGLState;
            }

            glBindBuffer(GL_ARRAY_BUFFER, mBuffers[toUT(EAttributes::WORLD_MATRIX_INSTANCED_VERTEX_BUFFER)]);

            for (GLuint i = 0; i < 4; ++i)
            {
                // Note that unlike the other vertex attributes such as the position and the normal we don't upload any data into the buffers.
                // The reason is that the WVP and world matrices are dynamic and will be updated every frame. (http://ogldev.atspace.co.uk/www/tutorial33/tutorial33.html)
                glEnableVertexAttribArray(lWorldLocation + i);
                glVertexAttribPointer(lWorldLocation + i, 4, GL_FLOAT, GL_FALSE, sizeof(mat4f), reinterpret_cast<GLvoid*>(sizeof(GLfloat) * i * 4));
                glVertexAttribDivisor(lWorldLocation + i, 1);
                checkOpenGLState;
            }

        }

        unbindVAO();
    }

    bool lResult = initMaterials(pScene, pFile);

    return lResult;
}

void MeshSOA::_initMesh(unsigned int pMeshIndex, const aiMesh* pMesh, vector<vec3f> & pPosition, vector<vec3f> & pNormals, vector<vec2f> & pTexCoords, vector<VertexBoneData<4>> & pBones, vector<unsigned int> & pIndices)
{
    const aiVector3D lZero3D(0.0f, 0.0f, 0.0f);

    // Save the positions, normals and texture coordinates
    for (unsigned int i = 0; i < pMesh->mNumVertices; i++)
    {
        const aiVector3D* rPos = & pMesh->mVertices[i];
        const aiVector3D* rTexCoord = pMesh->HasTextureCoords(0) ? &(pMesh->mTextureCoords[0][i]) : & lZero3D;
        const aiVector3D* rNormal = & pMesh->mNormals[i];

        pPosition.push_back(vec3f({rPos->x, rPos->y, rPos->z}));
        pTexCoords.push_back(vec2f({rTexCoord->x, rTexCoord->y}));
        pNormals.push_back(vec3f({rNormal->x, rNormal->y, rNormal->z}));
    }

    MeshBoneData::loadBones(mEntries[pMeshIndex].baseVertex, pMesh, pBones);

    // When loading indices with adjacencies, there are 6 indices per triangle
    if (mWithAdjacencies)
    {
        mAdjacencyTool.findAdjacencies(pMesh, pIndices);
    }
    else
    {
        // Saves all the indices
        for (unsigned int i = 0; i < pMesh->mNumFaces; i++)
        {
            const aiFace & rFace = pMesh->mFaces[i];

            assert(rFace.mNumIndices == 3);

            pIndices.push_back(rFace.mIndices[0]);
            pIndices.push_back(rFace.mIndices[1]);
            pIndices.push_back(rFace.mIndices[2]);
        }
    }
}

void MeshSOA::_initMesh(const aiMesh* pMesh, std::vector<vec3f> & pPosition, std::vector<vec3f> & pNormals, std::vector<vec2f> & pTexCoords, std::vector<vec3f> & pTangents, std::vector<unsigned int> & pIndices)
{
    const aiVector3D lZero3D(0.0f, 0.0f, 0.0f);

    // Save the positions, normals, texture coordinates, and tangents
    for (unsigned int i = 0; i < pMesh->mNumVertices; i++)
    {
        const aiVector3D* rPos = & pMesh->mVertices[i];
        const aiVector3D* rTexCoord = pMesh->HasTextureCoords(0) ? &(pMesh->mTextureCoords[0][i]) : & lZero3D;
        const aiVector3D* rNormal = & pMesh->mNormals[i];
        const aiVector3D* rTangent = & pMesh->mTangents[i];

        pPosition.push_back(vec3f({rPos->x, rPos->y, rPos->z}));
        pTexCoords.push_back(vec2f({rTexCoord->x, rTexCoord->y}));
        pNormals.push_back(vec3f({rNormal->x, rNormal->y, rNormal->z}));
        pTangents.push_back(vec3f({rTangent->x, rTangent->y, rTangent->z}));
    }

//    MeshBoneData::loadBones(mEntries[pMeshIndex].baseVertex, pMesh, pBones);

    // Saves all the indices
    for (unsigned int i = 0; i < pMesh->mNumFaces; i++)
    {
        const aiFace & rFace = pMesh->mFaces[i];

        assert(rFace.mNumIndices == 3);

        pIndices.push_back(rFace.mIndices[0]);
        pIndices.push_back(rFace.mIndices[1]);
        pIndices.push_back(rFace.mIndices[2]);
    }
}
