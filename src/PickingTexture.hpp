//===============================================================================================//
/*!
 *  \file      PickingTexture.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

#include <GL/glew.h>

namespace miniGL
{
    /*!
     *  \brief   This class handles the indices of the primitices associated to a pixel
     *  \details This class represents the FBO which we will render the primitive indices into. It encapsulates the framebuffer object handle, a texture object for the index info and a texture object for the depth buffer.
     */
    class PickingTexture
    {
    public:
        struct PixelInfo
        {
            float objectID = 0.0f;      /*! Corresponds to the number of times the mesh is rendered (at different places), i.e. to one of the transforms associated to the mesh */
            float drawID = 0.0f;        /*! Corresponds to the entry within the mesh, i.e. which "part" of the mesh */
            float primitiveID = 0.0f;   /*! Corresponds to the triangle index within the entry */
        }; // struct PixelInfo

    public:
        /*!
         *  \brief Destructor
         */
        ~PickingTexture(void);

        /*!
         *  \brief
         *  \param pFrameBufferWidth is the width of the current window
         *  \param pFrameBufferWidth is the height of the current window
         */
        void init(unsigned int pFrameBufferWidth, unsigned int pFrameBufferHeight);

        /*!
         *  \brief
         */
        void enableWritting(void);

        /*!
         *  \brief
         */
        void disableWritting(void);

        /*!
         *  \brief Get the indices of the primitive corresponding to the current pixel
         *  \param pX is the x coordinate of the current pixel
         *  \param pY is the y coordinate of the current pixel
         */
        PixelInfo readPixel(unsigned int pX, unsigned int pY);

    private:
        GLuint mFBO = 0;
        GLuint mPickingTexture = 0;
        GLuint mDepthTexture = 0;

    }; // class PickingTexture

} // namespace miniGL
