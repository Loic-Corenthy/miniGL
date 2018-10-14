//===============================================================================================//
/*!
 *  \file      MeshEdge.cpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#include "MeshEdge.hpp"

#include <cassert>

using miniGL::MeshEdge;

MeshEdge::MeshEdge(unsigned int pA, unsigned int pB)
{
    assert(pA != pB && "Cannot create an edge with twice the same vertex index");

    if (pA < pB)
    {
        mA = pA;
        mB = pB;
    }
    else
    {
        mA = pB;
        mB = pA;
    }
}

void MeshEdge::display() const
{
    std::cout << "Edge " << mA << " " << mB << std::endl;
}

void MeshEdge::a(unsigned int pIndex) noexcept
{
    mA = pIndex;
}

void MeshEdge::b(unsigned int pIndex) noexcept
{
    mB = pIndex;
}

unsigned int MeshEdge::a(void) const noexcept
{
    return mA;
}

unsigned int MeshEdge::b(void) const noexcept
{
    return mB;
}
