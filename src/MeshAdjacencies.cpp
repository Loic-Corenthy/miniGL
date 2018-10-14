//===============================================================================================//
/*!
 *  \file      MeshAdjacencies.cpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#include "MeshAdjacencies.hpp"

#include <cassert>
#include <limits>

using std::numeric_limits;
using miniGL::MeshAdjacencies;
using miniGL::MeshFace;

void MeshAdjacencies::findAdjacencies(const aiMesh * pMesh, std::vector<unsigned int> & pIndices)
{
    // Step 1: find the two triangles that share every edge
    for (unsigned int i = 0; i < pMesh->mNumFaces; ++i)
    {
        const aiFace & rFace = pMesh->mFaces[i];

        MeshFace lUnique;

        // If a position vector is duplicated in the VB we fetch the index of the first occurrence.
        for (unsigned int j = 0; j < 3; ++j )
        {
            unsigned int lIndex = rFace.mIndices[j];
            const aiVector3D & lVertex = pMesh->mVertices[lIndex];

            if (mPositions.find(lVertex) == mPositions.end())
                mPositions[lVertex] = lIndex;
            else
                lIndex = mPositions[lVertex];

            lUnique.index(j, lIndex);
        }

        mUniqueFaces.push_back(lUnique);

        const MeshEdge lEdge1(lUnique.index(0), lUnique.index(1));
        const MeshEdge lEdge2(lUnique.index(1), lUnique.index(2));
        const MeshEdge lEdge3(lUnique.index(2), lUnique.index(0));

        mIndices[lEdge1].add(i);
        mIndices[lEdge2].add(i);
        mIndices[lEdge3].add(i);
    }

    // Step 2: build the index buffer with the adjacency info
    for (unsigned int i = 0; i < pMesh->mNumFaces; ++i)
    {
        const MeshFace & rFace = mUniqueFaces[i];

        for (unsigned int j = 0; j < 3; ++j)
        {
            MeshEdge lEdge(rFace.index(j), rFace.index((j + 1) % 3));
            assert(mIndices.find(lEdge) != mIndices.end());
            const MeshNeighbors lNeighbor = mIndices[lEdge];
            unsigned int lOtherTriangle = lNeighbor.other(i);

            assert(lOtherTriangle != numeric_limits<unsigned int>::max());

            const MeshFace & rOtherFace = mUniqueFaces[lOtherTriangle];
            unsigned int lOppositeIndex = rOtherFace.oppositeIndex(lEdge);

            pIndices.push_back(rFace.index(j));
            pIndices.push_back(lOppositeIndex);
        }
    }
}
