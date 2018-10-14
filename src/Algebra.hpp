//===============================================================================================//
/*!
 *  \file      Algebra.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

#include "Vector.hpp"
#include "Matrix.hpp"
#include "Quaternion.hpp"

/*!
 *  \brief   Helper include file to include all the headers useful for linear algebra operations
 *  \details Include generic matrix and vector header files and create user friendly matrix 4x4
 *           and vector types
 */
using vec2i = miniGL::Vector<int, miniGL::TWO, 2>;
using vec2f = miniGL::Vector<float, miniGL::TWO, 2>;
using vec2d = miniGL::Vector<double, miniGL::TWO, 2>;
using vec3i = miniGL::Vector<int, miniGL::THREE, 3>;
using vec3f = miniGL::Vector<float, miniGL::THREE, 3>;
using vec3d = miniGL::Vector<double, miniGL::THREE, 3>;
using vec4f = miniGL::Vector<float, miniGL::FOUR, 4>;
using vec4i = miniGL::Vector<int, miniGL::FOUR, 4>;
using mat4f = miniGL::Matrix<float, miniGL::FOUR, miniGL::FOUR, 4, 4>;
using mat4d = miniGL::Matrix<double, miniGL::FOUR, miniGL::FOUR, 4, 4>;
using quatf = miniGL::Quaternion<float>;
