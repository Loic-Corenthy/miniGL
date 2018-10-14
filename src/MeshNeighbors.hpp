//===============================================================================================//
/*!
 *  \file      MeshNeighbors.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

#include <cassert>
#include <array>

namespace miniGL
{
    /*!
     *  \brief   This class handles the indices to capture vertices neighbors
     *  \details Mainly used when building meshes with the adjacencies option
     */
    class MeshNeighbors
    {
    public:
        /*!
         *  \brief Default constructor
         */
        MeshNeighbors(void);

        /*!
         *  \brief Add a neighbor index to a free slot
         *  @param pIndex is the index of a vertex
         */
        void add(unsigned int pIndex);

        /*!
         *  \brief Get the index corresponding to the other neighbor in this pair
         *  @param pIndex corresponds to one of the indices in this pair
         */
        unsigned int other(unsigned int pIndex) const;

        /*!
         *  \brief Get a vertex index
         *  @param pIndex is the neighbor index on the face (0 or 1)
         */
        unsigned int index(unsigned int pIndex) const;

    private:
        std::array<unsigned int, 2> mIndices;

    }; // class MeshNeighbors

} // namespace miniGL
