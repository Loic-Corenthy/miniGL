//===============================================================================================//
/*!
 *  \file      MeshFace.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

#include <cassert>
#include <array>

#include "MeshEdge.hpp"

namespace miniGL
{
    /*!
     *  \brief Helper class to store vertex indices per face
     *  \details
     */
    class MeshFace
    {
    public:
        /*!
         *  \brief Default constructor
         */
        MeshFace(void);

        /*!
         *  \brief Default constructor
         */
        unsigned int oppositeIndex(const MeshEdge & pEdge) const;

        /*!
         *  \brief Set a vertex index
         *  @param pFaceIndex is the index on the face (0, 1 or 2)
         *  @param pVertexIndex is the vertex index to store
         */
        void index(unsigned int pFaceIndex, unsigned int pVertexIndex);

        /*!
         *  \brief Get a vertex index
         *  @param pFaceIndex is the index on the face (0, 1 or 2)
         */
        unsigned int index(unsigned int pIndex) const;

    private:
        std::array<unsigned int, 3> mIndices;

    }; // class MeshFace

} // namespace miniGL
