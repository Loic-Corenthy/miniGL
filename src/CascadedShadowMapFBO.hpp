//===============================================================================================//
/*!
 *  \file      CascadedShadowMapFBO.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

#include <array>
#include <GL/glew.h>

namespace miniGL
{
    /*!
     *  \brief Mainly maintain the texture used to render the shadow
     *  \details This class eases the manipulation of a frame buffer object for the shadow map technique
     */
    class CascadedShadowMapFBO
    {
    public:
        /*!
         *  \brief Default constructor
         */
        CascadedShadowMapFBO(void);

        /*!
         *  \brief Destructor
         */
        ~CascadedShadowMapFBO(void);

        /*!
         *  \brief Create a 2d texture and bind to the FBO
         *  @param pWindowWidth is used for the width of the texture
         *  @param pWindowHeight is used for the height of the texture
         */
        void init(unsigned int pWindowWidth, unsigned int pWindowHeight);

        /*!
         *  \brief Make the FBO active for writting with the shader
         */
        void bindForWriting(unsigned int pCascadeIndex);

        /*!
         *  \brief Make the texture linked to the FBO active for reading
         *  @param pTextureUnit is the ID of the texture to which the values
         *         of the FBO will be copied to
         */
        void bindForReading(void);

        /*!
         *  \brief Get the number of cascade in this FBO
         *  @return the number of shadow map that we can write into
         */
        size_t size(void) const noexcept;

    private:
        GLuint mFBO;
        std::array<GLuint, 3> mShadowMap;

    }; // class CascadedShadowMapFBO

} // namespace miniGL
