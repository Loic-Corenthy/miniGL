//===============================================================================================//
/*!
 *  \file      IntermediateBuffer.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

#include <GL/glew.h>

namespace miniGL
{
    /*!
     *  \brief   This class encapsulates a color buffer, a depth buffer and a motion buffer
     *  \details The "motion buffer" is used to render the "motion blur" effect, see SkinningTechnique
     */
    class IntermediateBuffer
    {
    public:
        /*!
         *  \brief Destructor
         */
        ~IntermediateBuffer(void);

        /*!
         *  \brief Create the FBO, buffers for the textures (color, motion and depth) and initialize them as 2d textures
         *  @param pFrameBufferWidth is the width of the framebuffer in pixel
         *  @param pFrameBufferHeight is the height of the framebuffer in pixel
         */
        void init(unsigned int pFrameBufferWidth, unsigned int pFrameBufferHeight);

        /*!
         *  \brief Bind the texture so that the shader can read from them
         */
        void bindForReading(void);

        /*!
         *  \brief Bind the textures so that we can render into them (as framebuffers)
         */
        void bindForWritting(void);

    private:
        GLuint mFBO = 0;
        GLuint mColorBuffer = 0;
        GLuint mMotionBuffer = 0;
        GLuint mDepthBuffer = 0;

    }; // class IntermediateBuffer

} // namespace miniGL
