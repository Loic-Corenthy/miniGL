//===============================================================================================//
/*!
 *  \file      EngineCommon.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

/*!
 *  \brief     Defines the use of different texture buffer
 *  \details   Texture buffers, e.g. GL_TEXTURE0, GL_TEXTURE1, ... are associated to a type of
 *             texture, e.g. COLOR_TEXTURE_UNIT, NORMAL_TEXTURE_UNIT, ...
 */
#define COLOR_TEXTURE_UNIT                      GL_TEXTURE0
#define COLOR_TEXTURE_UNIT_INDEX                0

#define SHADOW_TEXTURE_UNIT                     GL_TEXTURE1
#define SHADOW_TEXTURE_UNIT_INDEX               1

#define DEPTH_TEXTURE_UNIT                      GL_TEXTURE1
#define DEPTH_TEXTURE_UNIT_INDEX                1

#define NORMAL_TEXTURE_UNIT                     GL_TEXTURE2
#define NORMAL_TEXTURE_UNIT_INDEX               2

#define RANDOM_TEXTURE_UNIT                     GL_TEXTURE3
#define RANDOM_TEXTURE_UNIT_INDEX               3

/*! \remark Is it safe to use GL_TEXTURE3 again if we were to use rendering techniques that use both random textures and ambient occulusion? */
#define AMBIENT_OCCULUSION_TEXTURE_UNIT_INDEX   GL_TEXTURE3
#define AMBIENT_OCCULUSION_TEXTURE_UNIT         3

#define DISPLACEMENT_TEXTURE_UNIT               GL_TEXTURE4
#define DISPLACEMENT_TEXTURE_UNIT_INDEX         4

#define MOTION_BLUR_TEXTURE_UNIT                GL_TEXTURE5
#define MOTION_BLUR_TEXTURE_UNIT_INDEX          5

#define CASCADE_SHADOW_TEXTURE_UNIT0            SHADOW_TEXTURE_UNIT
#define CASCADE_SHADOW_TEXTURE_UNIT0_INDEX      SHADOW_TEXTURE_UNIT_INDEX

#define CASCADE_SHADOW_TEXTURE_UNIT1            GL_TEXTURE6
#define CASCADE_SHADOW_TEXTURE_UNIT1_INDEX      6

#define CASCADE_SHADOW_TEXTURE_UNIT2            GL_TEXTURE7
#define CASCADE_SHADOW_TEXTURE_UNIT2_INDEX      7

#define INDEX_LOCATION  0
#define VERTEX_LOCATION 1
