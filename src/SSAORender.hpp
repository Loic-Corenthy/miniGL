//===============================================================================================//
/*!
 *  \file      SSAORender.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

#include "Program.hpp"
#include "Algebra.hpp"
#include "Constants.hpp"
#include "IOBuffer.hpp"
#include "Angle.hpp"

namespace miniGL
{
    /*!
     *  \brief   This class handles the shaders
     *  \details Compute the light taking into account how flat/cornered objects are
     */
    class SSAORender : public Program
    {
    public:
        /*!
         *  \brief Destructor
         */
        virtual ~SSAORender(void);

        /*!
         *  \brief Load shaders and get uniform IDS.
         */
        void init(void);

        /*!
         * \brief Configure the input/output buffer to access the depth buffer
         * @param pBuffer is the buffer to configure
         */
        void bindDepthBuffer(IOBuffer & pBuffer);

        /*!
         * \brief Set the the sample radius
         * @param pRadius is the radius in pixel
         */
        void sampleRadius(float pRadius);

        /*!
         *  \brief Set the projection matrix
         *  @param pProjection is a 4x4 matrix
         */
        void projection(const mat4f & pProjection);

        /*!
         *  \brief Set the aspect ratio
         *  @param pValue should be computed as framebuffer width / framebuffer height
         */
        void aspectRatio(float pValue);

        /*!
         *  \brief Set the Field of View
         *  @param pValue is the Field Of View in radian
         */
        void FOV(radianf pValue);

    protected:
        /*!
         *  \brief Implementation of a virtual method from Program
         */
        virtual bool checkUniformLocations(void) const final;

    private:
        /*!
         * \brief Generate the coefficients that will be used to scale the translation vectors used to
         *        compute random points around a vertex
         */
        void _generateKernel(void);

    private:
        GLuint mDepthTextureUnitLocation = Constants::invalidUniformLocation<GLuint>();
        GLuint mSampleRadiusLocation = Constants::invalidUniformLocation<GLuint>();
        GLuint mKernelLocation = Constants::invalidUniformLocation<GLuint>();
        GLuint mProjectionLocation = Constants::invalidUniformLocation<GLuint>();
        GLuint mAspectRatioLocation = Constants::invalidUniformLocation<GLuint>();
        GLuint mTanHalfFOVLocation = Constants::invalidUniformLocation<GLuint>();

    }; // class SSAORender

} // namespace miniGL
