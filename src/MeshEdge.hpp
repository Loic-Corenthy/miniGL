//===============================================================================================//
/*!
 *  \file      MeshEdge.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

#include <iostream>

namespace miniGL
{
    /*!
     *  \brief Helper class to store a simple edge
     *  \details Stores an edge by its vertices and forces a specific order between them
     */
    class MeshEdge
    {
    public:
        /*!
         *  \brief Constructor with vertices
         *  @param pA is the index of the first vertex
         *  @param pB is the index of the second vertex
         */
        MeshEdge(unsigned int pA, unsigned int pB);

        /*!
         *  \brief Display the indices
         */
        void display(void) const;

        /*!
         *  \brief Set the index associated to the first vertex
         *  @param pIndex is the index associated to vertex "a"
         */
        void a(unsigned int pIndex) noexcept;

        /*!
         *  \brief Set the index associated to the second vertex
         *  @param pIndex is the index associated to vertex "b"
         */
        void b(unsigned int pIndex) noexcept;

        /*!
         *  \brief Get the index associated to the first vertex
         *  @return the index associated to vertex "a"
         */
        unsigned int a(void) const noexcept;

        /*!
         *  \brief Get the index associated to the second vertex
         *  @return the index associated to vertex "b"
         */
        unsigned int b(void) const noexcept;

    private:
        unsigned int mA;
        unsigned int mB;

    }; // class MeshEdge

} // namespace miniGL
