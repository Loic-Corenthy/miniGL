//===============================================================================================//
/*!
 *  \file      MeshFace.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#include "MeshFace.hpp"

#include <limits>

using std::numeric_limits;
using miniGL::MeshFace;
using miniGL::MeshEdge;

MeshFace::MeshFace(void)
:mIndices{{numeric_limits<unsigned int>::max(), numeric_limits<unsigned int>::max(), numeric_limits<unsigned int>::max()}}
{
}

unsigned int MeshFace::oppositeIndex(const MeshEdge & pEdge) const
{
    for (unsigned int i = 0 ; i < mIndices.size() ; ++i)
    {
        if (mIndices[i] != pEdge.a() && mIndices[i] != pEdge.b())
            return mIndices[i];
    }

    assert(false && "Unable to find index in mesh face");

    return 0;
}

void MeshFace::index(unsigned int pFaceIndex, unsigned int pVertexIndex)
{
    assert((pFaceIndex == 0 || pFaceIndex == 1 || pFaceIndex == 2) && "Index out of boundaries in MeshFace::index");
    mIndices[pFaceIndex] = pVertexIndex;
}

unsigned int MeshFace::index(unsigned int pFaceIndex) const
{
    assert((pFaceIndex == 0 || pFaceIndex == 1 || pFaceIndex == 2) && "Index out of boundaries in MeshFace::index");
    return mIndices[pFaceIndex];
    }
