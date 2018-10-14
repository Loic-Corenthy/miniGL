//===============================================================================================//
/*!
 *  \file      ShadowMapFBO.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

#include <GL/glew.h>

namespace miniGL
{
    /*!
     *  \brief Mainly maintain the texture used to render the shadow
     *  \details This class eases the manipulation of a frame buffer object for the shadow map technique
     */
    class ShadowMapFBO
    {
    public:
        /*!
         *  \brief Default constructor
         */
        ShadowMapFBO(void);

        /*!
         *  \brief Destructor
         */
        ~ShadowMapFBO(void);

        /*!
         *  \brief Create a 2d texture and bind to the FBO
         *  @param pWindowWidth is used for the width of the texture
         *  @param pWindowHeight is used for the height of the texture
         */
        void init(unsigned int pWindowWidth, unsigned int pWindowHeight);

        /*!
         *  \brief Make the FBO active for writting with the shader
         */
        void bindForWriting(void);

        /*!
         *  \brief Make the texture linked to the FBO active for reading
         *  @param pTextureUnit is the ID of the texture to which the values
         *         of the FBO will be copied to
         */
        void bindForReading(GLenum pTextureUnit);

    private:
        GLuint mFBO;
        GLuint mShadowMap;

    }; // class ShadowMapFBO

} // namespace miniGL


