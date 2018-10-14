//===============================================================================================//
/*!
 *  \file      MeshBoneData.cpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#include "MeshBoneData.hpp"

#include "Exceptions.hpp"
#include "Transform.hpp"

using std::string;
using std::vector;
using miniGL::MeshBoneData;
using miniGL::Exceptions;
using miniGL::Transform;
using miniGL::VertexBoneData;

void MeshBoneData::boneTransform(const aiScene * pScene, float pTime, std::vector<mat4f> & pTransforms)
{
    mat4f lIdentity(1.0f);

    float lTickPerSeconds = static_cast<float>(pScene->mAnimations[0]->mTicksPerSecond != 0.0 ? pScene->mAnimations[0]->mTicksPerSecond : 25.0f);
    float lTimeInTicks = pTime * lTickPerSeconds;
    float lAnimationTime = fmod(lTimeInTicks, static_cast<float>(pScene->mAnimations[0]->mDuration));

    _readNodeHierarchy(pScene, lAnimationTime, pScene->mRootNode, lIdentity);

    pTransforms.resize(mBoneCount);

    for (unsigned int i = 0; i < mBoneCount; ++i)
        pTransforms[i] = mBoneInfo[i].finalTransformation;
}

void MeshBoneData::loadBones(unsigned int pMeshEntryBaseVertex, const aiMesh* pMesh, vector<VertexBoneData<4>> & pBones)
{
    for (unsigned int i = 0; i < pMesh->mNumBones; ++i)
    {
        unsigned int lBoneIndex = 0;
        string lBoneName(pMesh->mBones[i]->mName.data);

        if (mBoneMapping.find(lBoneName) == mBoneMapping.end())
        {
            // Allocate an index for a new bone
            lBoneIndex = mBoneCount;
            mBoneCount++;
            BoneInfo lBoneInfo;
            mBoneInfo.push_back(lBoneInfo);

            // Copy bone matrix to bone info
            mBoneInfo[lBoneIndex].boneOffset = _convertMatrix(pMesh->mBones[i]->mOffsetMatrix);

            mBoneMapping[lBoneName] = lBoneIndex;
        }
        else
            lBoneIndex = mBoneMapping[lBoneName];

        for (unsigned int j = 0; j < pMesh->mBones[i]->mNumWeights; ++j)
        {
            const unsigned int lVertexID = pMeshEntryBaseVertex + pMesh->mBones[i]->mWeights[j].mVertexId;
            const float lWeight = pMesh->mBones[i]->mWeights[j].mWeight;

            pBones[lVertexID].addBoneData(lBoneIndex, lWeight);
        }
    }
}

unsigned int MeshBoneData::boneCount(void) const noexcept
{
    return mBoneCount;
}

void MeshBoneData::globalInverseTransform(const aiMatrix4x4 & pTransform)
{
    mGlobalInverseTransform = _convertMatrix(pTransform);
    mGlobalInverseTransform.inverse();
}

aiVector3D MeshBoneData::_interpolatedScaling(float pAnimationTime, const aiNodeAnim * pNodeAnim)
{
    // At least 2 values are necessary to interpolate
    if (pNodeAnim->mNumScalingKeys == 1)
    {
        return pNodeAnim->mScalingKeys[0].mValue;
    }
    else
    {
        unsigned int lScalingIndex = _findScaling(pAnimationTime, pNodeAnim);
        const unsigned int lNextScalingIndex = lScalingIndex + 1;

        assert(lNextScalingIndex < pNodeAnim->mNumScalingKeys);

        auto lDeltaTime = static_cast<float>(pNodeAnim->mScalingKeys[lNextScalingIndex].mTime - pNodeAnim->mScalingKeys[lScalingIndex].mTime);

        float lFactor = (pAnimationTime - static_cast<float>(pNodeAnim->mScalingKeys[lScalingIndex].mTime)) / lDeltaTime;

        assert(0.0f <= lFactor && lFactor <= 1.0f);

        const aiVector3D & lStart = pNodeAnim->mScalingKeys[lScalingIndex].mValue;
        const aiVector3D & lEnd = pNodeAnim->mScalingKeys[lNextScalingIndex].mValue;

        return lStart + lFactor * (lEnd - lStart);
    }
}

aiQuaternion MeshBoneData::_interpolatedRotation(float pAnimationTime, const aiNodeAnim * pNodeAnim)
{
    // At least 2 values are necessary to interpolate
    if (pNodeAnim->mNumRotationKeys == 1)
    {
        return pNodeAnim->mRotationKeys[0].mValue;
    }
    else
    {
        unsigned int lRotationIndex = _findRotation(pAnimationTime, pNodeAnim);
        const unsigned int lNextRotationIndex = lRotationIndex + 1;

        assert(lNextRotationIndex < pNodeAnim->mNumRotationKeys);

        auto lDeltaTime = static_cast<float>(pNodeAnim->mRotationKeys[lNextRotationIndex].mTime - pNodeAnim->mRotationKeys[lRotationIndex].mTime);

        float lFactor = (pAnimationTime - static_cast<float>(pNodeAnim->mRotationKeys[lRotationIndex].mTime)) / lDeltaTime;

        assert(0.0f <= lFactor && lFactor <= 1.0f);

        const aiQuaternion & lStart = pNodeAnim->mRotationKeys[lRotationIndex].mValue;
        const aiQuaternion & lEnd = pNodeAnim->mRotationKeys[lNextRotationIndex].mValue;

        aiQuaternion lRes;
        aiQuaternion::Interpolate(lRes, lStart, lEnd, lFactor);
        return lRes.Normalize();
    }
}

aiVector3D MeshBoneData::_interpolatedPosition(float pAnimationTime, const aiNodeAnim * pNodeAnim)
{
    // At least 2 values are necessary to interpolate
    if (pNodeAnim->mNumPositionKeys == 1)
    {
        return pNodeAnim->mPositionKeys[0].mValue;
    }
    else
    {
        unsigned int lPositionIndex = _findPosition(pAnimationTime, pNodeAnim);
        const unsigned int lNextPositionIndex = lPositionIndex + 1;

        assert(lNextPositionIndex < pNodeAnim->mNumPositionKeys);

        auto lDeltaTime = static_cast<float>(pNodeAnim->mPositionKeys[lNextPositionIndex].mTime - pNodeAnim->mPositionKeys[lPositionIndex].mTime);

        float lFactor = (pAnimationTime - static_cast<float>(pNodeAnim->mPositionKeys[lPositionIndex].mTime)) / lDeltaTime;

        assert(0.0f <= lFactor && lFactor <= 1.0f);

        const aiVector3D & lStart = pNodeAnim->mPositionKeys[lPositionIndex].mValue;
        const aiVector3D & lEnd = pNodeAnim->mPositionKeys[lNextPositionIndex].mValue;

        return lStart + lFactor * (lEnd - lStart);
    }
}

unsigned int MeshBoneData::_findScaling(float pAnimationTime, const aiNodeAnim * pNodeAnim)
{
    if (pNodeAnim->mNumScalingKeys <= 0)
        throw Exceptions("No scaling keys found for animation", __FILE__, __LINE__);

    for (unsigned int i = 0; i < pNodeAnim->mNumScalingKeys - 1; ++i)
    {
        if (pAnimationTime < static_cast<float>(pNodeAnim->mScalingKeys[i + 1].mTime))
            return i;
    }

    throw Exceptions("Unable to find node scaling for current animation", __FILE__, __LINE__);

    return 0;
}

unsigned int MeshBoneData::_findRotation(float pAnimationTime, const aiNodeAnim * pNodeAnim)
{
    if (pNodeAnim->mNumRotationKeys <= 0)
        throw Exceptions("No rotation keys found for animation", __FILE__, __LINE__);

    for (unsigned int i = 0; i < pNodeAnim->mNumRotationKeys - 1; ++i)
    {
        if (pAnimationTime < static_cast<float>(pNodeAnim->mRotationKeys[i + 1].mTime))
            return i;
    }

    throw Exceptions("Unable to find node rotation for current animation", __FILE__, __LINE__);

    return 0;
}

unsigned int MeshBoneData::_findPosition(float pAnimationTime, const aiNodeAnim * pNodeAnim)
{
    for (unsigned int i = 0; i < pNodeAnim->mNumPositionKeys - 1; ++i)
    {
        if (pAnimationTime < static_cast<float>(pNodeAnim->mPositionKeys[i + 1].mTime))
            return i;
    }

    throw Exceptions("Unable to find node position for current animation", __FILE__, __LINE__);

    return 0;
}

const aiNodeAnim* MeshBoneData::_findNodeAnim(const aiAnimation* pAnimation, const std::string & pNodeName)
{
    for (unsigned int i = 0; i < pAnimation->mNumChannels; ++i)
    {
        const aiNodeAnim* rNodeAnim = pAnimation->mChannels[i];

        if (string(rNodeAnim->mNodeName.data) == pNodeName)
            return rNodeAnim;
    }

    return nullptr;
}

void MeshBoneData::_readNodeHierarchy(const aiScene * pScene, float pAnimationTime, const aiNode* pNode, const mat4f & pParentTransform)
{
    string lNodeName(pNode->mName.data);

    const aiAnimation * rAnimation = pScene->mAnimations[0];

    mat4f lNodeTransformation = _convertMatrix(pNode->mTransformation);

    const aiNodeAnim* rNodeAnim = _findNodeAnim(rAnimation, lNodeName);

    if (rNodeAnim)
    {
        Transform lTransform;

        // Interpolate scaling and generate scaling transformation matrix
        aiVector3D lScaling = _interpolatedScaling(pAnimationTime, rNodeAnim);
        lTransform.scaling(lScaling.x, lScaling.y, lScaling.z);

        // Interpolate rotation and generate rotation transformation matrix
        aiQuaternion lRotationQ = _interpolatedRotation(pAnimationTime, rNodeAnim);

        auto lTmpRotation = lRotationQ.GetMatrix();
        mat4f lRotationMatrix(1.0f);

        lRotationMatrix(0,0) = lTmpRotation.a1; lRotationMatrix(0,1) = lTmpRotation.a2; lRotationMatrix(0,2) = lTmpRotation.a3;
        lRotationMatrix(1,0) = lTmpRotation.b1; lRotationMatrix(1,1) = lTmpRotation.b2; lRotationMatrix(1,2) = lTmpRotation.b3;
        lRotationMatrix(2,0) = lTmpRotation.c1; lRotationMatrix(2,1) = lTmpRotation.c2; lRotationMatrix(2,2) = lTmpRotation.c3;

        lTransform.rotation(lRotationMatrix);

        // Interpolate translation and generate translation transformation matrix
        aiVector3D lTranslation = _interpolatedPosition(pAnimationTime, rNodeAnim);
        lTransform.translation(lTranslation.x, lTranslation.y, lTranslation.z);

        // Combine all the transformations
        lNodeTransformation = lTransform.final();
    }

    mat4f lGlobalTransformation = pParentTransform * lNodeTransformation;

    if (mBoneMapping.find(lNodeName) != mBoneMapping.end())
    {
        unsigned int lBoneIndex = mBoneMapping[lNodeName];
        mBoneInfo[lBoneIndex].finalTransformation = mGlobalInverseTransform * lGlobalTransformation * mBoneInfo[lBoneIndex].boneOffset;
    }

    for (unsigned int i = 0; i < pNode->mNumChildren; ++i)
        _readNodeHierarchy(pScene, pAnimationTime, pNode->mChildren[i], lGlobalTransformation);
}

mat4f MeshBoneData::_convertMatrix(const aiMatrix4x4 & pMat) const
{
    mat4f lRes;

    lRes(0,0) = pMat.a1; lRes(0,1) = pMat.a2; lRes(0,2) = pMat.a3; lRes(0,3) = pMat.a4;
    lRes(1,0) = pMat.b1; lRes(1,1) = pMat.b2; lRes(1,2) = pMat.b3; lRes(1,3) = pMat.b4;
    lRes(2,0) = pMat.c1; lRes(2,1) = pMat.c2; lRes(2,2) = pMat.c3; lRes(2,3) = pMat.c4;
    lRes(3,0) = pMat.d1; lRes(3,1) = pMat.d2; lRes(3,2) = pMat.d3; lRes(3,3) = pMat.d4;

    return lRes;
}
