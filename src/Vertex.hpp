//===============================================================================================//
/*!
 *  \file      Vertex.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

#include "Algebra.hpp"
#include "VertexBoneData.hpp"

namespace miniGL
{
    /*!
     *  \brief   This class encapsulates the parameters definining a vertex of the mesh
     *  \details Each vertex is represented by a position, a pair of texture coordinates and a normal vector.
     *           Optional parameters are a tangent, bone rotation matrices, ...
     */
    class Vertex
    {
    public:
        /*!
         * \brief Default constructor
         */
        Vertex(void) = default;

        /*!
         *  \brief Constructor with member initialization
         */
        Vertex(const vec3f & pPosition,
               const vec2f & pTextureCoords,
               const vec3f & pNormal = vec3f(0.0f),
               const vec3f & pTangent = vec3f(0.0f));

        /*!
         *  \brief Destructor
         */
        ~Vertex(void);

        /*!
         *  \brief Set vertex position
         *  @param pPosition is the 3D vector representing the position of the vertex
         */
        void position(const vec3f & pPosition) noexcept;

        /*!
         *  \brief Set vertex texture coordinates
         *  @param pTextureCoords is the 2D vector representing the texture coordinates associated to the vertex
         */
        void textureCoords(const vec2f & pTextureCoords) noexcept;

        /*!
         *  \brief Set vertex normal vector
         *  @param pNormal is the normalized vector representing the normal
         */
        void normal(const vec3f & pNormal) noexcept;

        /*!
         *  \brief Set vertex tangent vector
         *  @param pTangent is the normalized vector representing the tangent vector (used for bump mapping)
         */
        void tangent(const vec3f & pTangent) noexcept;

        /*!
         *  \brief Set vertex tangent vector
         *  @param pTangent is a rvalue reference on the normalized vector representing the tangent vector (used for bump mapping)
         */
        void tangent(vec3f && pTangent) noexcept;

        /*!
         *  \brief Get vertex position (read only)
         *  @return a 3D vector representing the position of the vertex
         */
        const vec3f & position(void) const noexcept;

        /*!
         *  \brief Get vertex texture coordinates (read only)
         *  @return a 2D vector representing the texture coordinates associated to the vertex
         */
        const vec2f & textureCoords(void) const noexcept;

        /*!
         *  \brief Get vertex normal vector (read only)
         *  @return a normalized vector representing the normal
         */
        const vec3f & normal(void) const noexcept;

        /*!
         *  \brief Get vertex tangent vector (read only)
         *  @return a normalized vector representing the tangent (used for bump mapping)
         */
        const vec3f & tangent(void) const noexcept;

        /*!
         *  \brief Get vertex position (read/write)
         *  @return a 3D vector representing the position of the vertex
         */
        vec3f & position(void) noexcept;

        /*!
         *  \brief Get vertex texture coordinates (read/write)
         *  @return a 2D vector representing the texture coordinates associated to the vertex
         */
        vec2f & textureCoords(void) noexcept;

        /*!
         *  \brief Get vertex normal vector (read/write)
         *  @return a normalized vector representing the normal
         */
        vec3f & normal(void) noexcept;

        /*!
         *  \brief Get vertex tangent vector (read only/write)
         *  @return a normalized vector representing the tangent (used for bump mapping)
         */
        vec3f & tangent(void) noexcept;

        /*!
         *  \brief Get bone data (read only/write)
         *  @return a normalized vector representing the tangent (used for bump mapping)
         */
        VertexBoneData<4> & boneData(void);

    private:
        vec3f mPosition = vec3f(0.0f);
        vec2f mTextureCoords = vec2f(0.0f);
        vec3f mNormal = vec3f(0.0f);
        vec3f mTangent = vec3f(0.0f);
        VertexBoneData<4> mBoneData;

    }; // class Vertex

} // namespace miniGL




