
//===============================================================================================//
/*!
 *  \file      MeshNeighbors.cpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#include "MeshNeighbors.hpp"

#include <limits>

using std::numeric_limits;
using miniGL::MeshNeighbors;

MeshNeighbors::MeshNeighbors(void)
:mIndices{{numeric_limits<unsigned int>::max(), numeric_limits<unsigned int>::max()}}
{
}

void MeshNeighbors::add(unsigned int pIndex)
{
    if (mIndices[0] == numeric_limits<unsigned int>::max())
        mIndices[0] = pIndex;
    else if(mIndices[1] == numeric_limits<unsigned int>::max())
        mIndices[1] = pIndex;
    else
        assert(false && "Adding to many indices to MeshNeighbors");
}

unsigned int MeshNeighbors::other(unsigned int pIndex) const
{
    unsigned int lRes = numeric_limits<unsigned int>::max();

    if (mIndices[0] == pIndex)
        lRes = mIndices[1];
    else if(mIndices[1] == pIndex)
        lRes = mIndices[0];
    else
        assert(false && "Getting unassigned index in MeshNeighbors");

    return lRes;
}

unsigned int MeshNeighbors::index(unsigned int pFaceIndex) const
{
    assert((pFaceIndex == 0 || pFaceIndex == 1) && "Index out of boundaries in MeshNeighbors::index");
    return mIndices[pFaceIndex];
}
