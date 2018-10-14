//===============================================================================================//
/*!
 *  \file      MeshAdjacencies.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

#include <vector>
#include <map>

#include <scene.h>

#include "MeshEdge.hpp"
#include "MeshNeighbors.hpp"
#include "MeshFace.hpp"

namespace miniGL
{
    /*!
     *  \brief This helper class can be used to find adjacent triangles to a face
     *  \details The goal is to obtain, for each face, the vertex indices as well as the adjacent vertex indices.
     */
    class MeshAdjacencies
    {
    public:
        /*!
         *  \brief Find all the adjacencies in the faces of the mesh and store the corresponding indices
         *  @param pMesh is the mesh to process
         *  @param pIndices are the indices of the vertices of each face with their adjacencies (6 vertices per face)
         */
        void findAdjacencies(const aiMesh * pMesh, std::vector<unsigned int> & pIndices);

    private:
        /*!
         *  \brief Functor to compare two edges
         */
        struct CompareEdges
        {
            bool operator()(const MeshEdge & pEdge1, const MeshEdge & pEdge2) const
            {
                if (pEdge1.a() < pEdge2.a())
                    return true;
                else if (pEdge1.a() == pEdge2.a())
                    return (pEdge1.b() < pEdge2.b());
                else
                    return false;
            }
        };

        /*!
         *  \brief Functor to compare two 3d vectors
         */
        struct CompareVectors
        {
            bool operator()(const aiVector3D & pV1, const aiVector3D & pV2) const
            {
                if (pV1.x < pV2.x)
                {
                    return true;
                }
                else if (pV1.x == pV2.x)
                {
                    if (pV1.y < pV2.y)
                    {
                        return true;
                    }
                    else if (pV1.y == pV2.y)
                    {
                        if (pV1.z < pV2.z)
                            return true;
                    }
                }

                return false;
            }
        };

        std::map<MeshEdge, MeshNeighbors, CompareEdges> mIndices;

#ifdef WIN32
		std::map<aiVector3D, unsigned int, CompareVectors> mPositions;
#else
        std::map<aiVector3D, uint, CompareVectors> mPositions;
#endif
        std::vector<MeshFace> mUniqueFaces;

    }; // class MeshAdjacencies

} // namespace miniGL
