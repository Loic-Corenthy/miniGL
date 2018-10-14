//===============================================================================================//
/*!
 *  \file      MeshAndTransform.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

#include <vector>

#include "Transform.hpp"
#include "MeshBase.hpp"

namespace miniGL
{
    /*!
     *  \brief   Simple container for a mesh and its associated transform
     *  \details The mesh is stored as a shared pointer on a MeshBase but should be created using one of the derived classes
     */
    struct MeshAndTransform
    {
        std::vector<Transform> transform;
        std::shared_ptr<MeshBase> mesh;

    }; // struct MeshAndTransform

} // namespace miniGL
