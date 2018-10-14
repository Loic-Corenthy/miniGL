//===============================================================================================//
/*!
 *  \file      MultipassShadowMapFBO.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

#include <GL/glew.h>

namespace miniGL
{
    /*!
     *  \brief Mainly maintain the textures (cube map) used to render the shadow
     *  \details This class eases the manipulation of a frame buffer object for the shadow map technique
     */
    class MultipassShadowMapFBO
    {
    public:
        /*!
         *  \brief Destructor
         */
        ~MultipassShadowMapFBO(void);

        /*!
         *  \brief Create a 2d texture and bind to the FBO
         *  @param pWindowWidth is used for the width of the texture
         *  @param pWindowHeight is used for the height of the texture
         */
        void init(unsigned int pWindowWidth, unsigned int pWindowHeight);

        /*!
         *  \brief Make the FBO active for writting with the shader
         *  @param pCubeFace is the face of the cube
         */
        void bindForWriting(GLenum pCubeFace);

        /*!
         *  \brief Make the texture linked to the FBO active for reading
         *  @param pTextureUnit is the ID of the texture to which the values
         *         of the FBO will be copied to
         */
        void bindForReading(GLenum pTextureUnit);

    private:
        GLuint mFBO = 0u;
        GLuint mShadowMap = 0u;
        GLuint mDepth = 0u;

    }; // class MultipassShadowMapFBO

} // namespace miniGL
