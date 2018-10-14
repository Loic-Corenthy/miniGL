//===============================================================================================//
/*!
 *  \file      SkyBoxRender.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

#include <GL/glew.h>

#include "Program.hpp"
#include "Algebra.hpp"

namespace miniGL
{
    /*!
     *  \brief   This class implements a skybox rendering
     *  \details The skybox is created using a cube map
     */
    class SkyBoxRender : public Program
    {
    public:
        /*!
         *  \brief Default constructor
         */
        SkyBoxRender(void);

        /*!
         *  \brief Destructor
         */
        virtual ~SkyBoxRender(void);

        /*!
         *  \brief Load and compile the shaders associated to this lighting technique.
         *         Initialize the id of the uniform variables used in the shaders.
         *  @param pPathVS is the path to the vertex shader
         *  @param pPathFS is the path to the fragment shader
         */
        void init(const std::string & pPathVS, const std::string & pPathFS);

        /*!
         *  \brief Set the world view projection matrix
         *  @param pWVP is a 4x4 matrix
         */
        void WVP(const mat4f & pWVP);

        /*!
         *  \brief Set the texture unit of the texture associated to the skybox
         *  @param pTexUnit is the texture unit index
         */
        void textureUnit(unsigned int pTexUnit);

    private:
        /*!
         *  \brief Implementation of a virtual method from Program
         */
        virtual bool checkUniformLocations(void) const final;

    private:
        GLuint mWVPLocation;
        GLuint mTextureLocation;

    }; // class SkyBoxRender

} // namespace miniGL
