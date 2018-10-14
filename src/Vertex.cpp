//===============================================================================================//
/*!
 *  \file      Vertex.cpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#include "Vertex.hpp"

using miniGL::Vertex;
using miniGL::VertexBoneData;

Vertex::Vertex(const vec3f & pPosition,
               const vec2f & pTextureCoords,
               const vec3f & pNormal,
               const vec3f & pTangent)
:mPosition(pPosition),
 mTextureCoords(pTextureCoords),
 mNormal(pNormal),
 mTangent(pTangent)
{

}

Vertex::~Vertex(void)
{
}

void Vertex::position(const vec3f & pPosition) noexcept
{
    mPosition = pPosition;
}

void Vertex::textureCoords(const vec2f & pTextureCoords) noexcept
{
    mTextureCoords = pTextureCoords;
}

void Vertex::normal(const vec3f & pNormal) noexcept
{
    mNormal = pNormal;
}

void Vertex::tangent(const vec3f & pTangent) noexcept
{
    mTangent = pTangent;
}

void Vertex::tangent(vec3f && pTangent) noexcept
{
    mTangent = std::move(pTangent);
}

const vec3f & Vertex::position(void) const noexcept
{
    return mPosition;
}

const vec2f & Vertex::textureCoords(void) const noexcept
{
    return mTextureCoords;
}

const vec3f & Vertex::normal(void) const noexcept
{
    return mNormal;
}

vec3f & Vertex::position(void) noexcept
{
    return mPosition;
}

vec2f & Vertex::textureCoords(void) noexcept
{
    return mTextureCoords;
}

vec3f & Vertex::normal(void) noexcept
{
    return mNormal;
}

VertexBoneData<4> & Vertex::boneData(void)
{
    return mBoneData;
    }
