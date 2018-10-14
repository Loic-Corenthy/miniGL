//===============================================================================================//
/*!
 *  \file      MeshBoneData.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

#include <vector>
#include <map>

#include <GL/glew.h>

#include <Importer.hpp>
#include <scene.h>
#include <postprocess.h>

#include "Algebra.hpp"
#include "VertexBoneData.hpp"

namespace miniGL
{
    /*!
     *  \brief This class encapsulate all the bone processing in a mesh for skinning
     *  \details This class is based on the Assimp library
     */
    class MeshBoneData
    {
    public:
        /*!
         *  \brief Get all the transformations associated to each bones for the current time
         *  @param pScene is a pointer of the Assimp scene
         *  @param pTime is in seconds
         *  @param pTransforms contains all the current transformation matrices
         */
        void boneTransform(const aiScene * pScene, float pTime, std::vector<mat4f> & pTransforms);

        /*!
         *  \brief Interpolate the scaling vector according to the current time stamp
         *  @param pOut is the interpolated scaling vector
         *  @param pAnimationTime is the current time stamp used to interpolate the scaling
         */
        void loadBones(unsigned int pMeshEntryBaseVertex, const aiMesh* pMesh, std::vector<VertexBoneData<4>> & pBones);

        /*!
         *  \brief Get the number of bones in the mesh
         *  @return the number of bones in the mesh
         */
        unsigned int boneCount(void) const noexcept;

        /*!
         *  \brief Set the global inverse transform
         *  @return pTransform is the root node transformation (that
         *          will be inversed and stored as the global inverse transform)
         */
        void globalInverseTransform(const aiMatrix4x4 & pTransform);

    private:
        /*!
         *  \brief Helper method to interpolate the scaling vector according to the current time stamp
         *  @param pAnimationTime is the current time stamp used to interpolate the scaling
         *  @param pNodeAnim ????????????????????
         *  @return the interpolated scaling vector
         */
        aiVector3D _interpolatedScaling(float pAnimationTime, const aiNodeAnim * pNodeAnim);

        /*!
         *  \brief Helper method to interpolate the rotation according to the current time stamp
         *  @param pOut is the interpolated rotation quaternion
         *  @param pAnimationTime is the current time stamp used to interpolate the scaling
         *  @param pNodeAnim ????????????????????
         */
        aiQuaternion _interpolatedRotation(float pAnimationTime, const aiNodeAnim * pNodeAnim);

        /*!
         *  \brief Interpolate the position vector according to the current time stamp
         *  @param pOut is the interpolated position vector
         *  @param pAnimationTime is the current time stamp used to interpolate the scaling
         *  @param pNodeAnim ????????????????????
         */
        aiVector3D _interpolatedPosition(float pAnimationTime, const aiNodeAnim * pNodeAnim);

        /*!
         *  \brief
         *  @param pOut is the interpolated scaling vector
         *  @param pAnimationTime is the current time stamp used to interpolate the scaling
         */
        unsigned int _findScaling(float pAnimationTime, const aiNodeAnim * pNodeAnim);

        /*!
         *  \brief
         *  @param pOut is the interpolated scaling vector
         *  @param pAnimationTime is the current time stamp used to interpolate the scaling
         */
        unsigned int _findRotation(float pAnimationTime, const aiNodeAnim * pNodeAnim);

        /*!
         *  \brief
         *  @param pOut is the interpolated scaling vector
         *  @param pAnimationTime is the current time stamp used to interpolate the scaling
         */
        unsigned int _findPosition(float pAnimationTime, const aiNodeAnim * pNodeAnim);

        /*!
         *  \brief Interpolate the scaling vector according to the current time stamp
         *  @param pOut is the interpolated scaling vector
         *  @param pAnimationTime is the current time stamp used to interpolate the scaling
         */
        const aiNodeAnim* _findNodeAnim(const aiAnimation* pAnimation, const std::string & pNodeName);

        /*!
         *  \brief Interpolate the scaling vector according to the current time stamp
         *  @param pOut is the interpolated scaling vector
         *  @param pAnimationTime is the current time stamp used to interpolate the scaling
         */
        void _readNodeHierarchy(const aiScene * pScene, float pAnimationTime, const aiNode* pNode, const mat4f & pParentTransform);

        /*!
         *  \brief Convert from aiMatrix4x4 to mat4f
         *  @param pMat is the matrix to convert
         *  @return a mat4f matrix with the same coefficients as pMat
         */
        mat4f _convertMatrix(const aiMatrix4x4 & pMat) const;

    private:
        struct BoneInfo
        {
            mat4f boneOffset;
            mat4f finalTransformation;
        };

        std::map<std::string, unsigned int> mBoneMapping;
        unsigned int mBoneCount = 0;
        std::vector<BoneInfo> mBoneInfo;
        mat4f mGlobalInverseTransform;

    }; // class MeshBoneData

} // namespace miniGL
