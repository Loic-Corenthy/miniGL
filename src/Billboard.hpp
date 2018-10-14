//===============================================================================================//
/*!
 *  \file      Billboard.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

#include <GL/glew.h>

#include "Program.hpp"
#include "Algebra.hpp"
#include "Constants.hpp"

namespace miniGL
{
    /*!
     *  \brief This class implements the billboarding rendering technique
     *  \details This class allows to display an image on a texture while creating the quad using
     *         the geometry shader
     */
    class Billboard : public Program
    {
    public:
        enum class ECutoff {MIN, MAX};

    public:
        Billboard(void);

        /*!
         *  \brief Load and compile the shaders associated to this billboarding technique.
         *         Initialize the id of the uniform variables used in the shaders.
         *  @param pPathVS is the path to the vertex shader
         *  @param pPathGS is the path to the geometry shader
         *  @param pPathFS is the path to the fragment shader
         */
        void init(const std::string & pPathVS, const std::string & pPathGS, const std::string & pPathFS);

        /*!
         *  \brief Set the view projection matrix
         *  @param pVP is a 4x4 matrix
         */
        void VP(const mat4f & pVP);

        /*!
         *  \brief Set the point of view in the scene
         *  @param pPos is the position of the camera
         */
        void eyeWorldPosition(const vec3f & pPos);

        /*!
         *  \brief Set the texture unit for the color texture
         *  @param pTexUnit is the texture unit index
         */
        void textureUnit(unsigned int pTextureUnit);

        /*!
         *  \brief Filter pixels with color values outide of the cutoff range
         *  @param pLimit refers to minimum or maximum value
         *  @param pActivate is true to filter values below min or above max, false to not filter pixel colors
         */
        void activateCutoff(ECutoff pLimit, bool pActivate);

        /*!
         *  \brief Set the cutoff value for the pixel filter
         *  @param pLimit refers to minimum or maximum value
         *  @param pValue is the corresponding cutoff value
         */
        void cutoff(ECutoff pLimit, float pValue);

        /*!
         *  \brief Set the size of the billboard
         *  @param pSize is the size
         */
        void size(float pValue);

    private:
        /*!
         *  \brief Implementation of a virtual method from Program
         */
        virtual bool checkUniformLocations(void) const final;

    private:
        GLuint mVPLocation = Constants::invalidUniformLocation<GLuint>();
        GLuint mCameraPositionLocation = Constants::invalidUniformLocation<GLuint>();
        GLuint mColorMapLocation = Constants::invalidUniformLocation<GLuint>();
        GLuint mSizeLocation = Constants::invalidUniformLocation<GLuint>();
        GLuint mCutoffMax = Constants::invalidUniformLocation<GLuint>();
        GLuint mActivateCutoffMax = Constants::invalidUniformLocation<GLuint>();
        GLuint mCutoffMin = Constants::invalidUniformLocation<GLuint>();
        GLuint mActivateCutoffMin = Constants::invalidUniformLocation<GLuint>();

    }; // class Billboard

} // namespace miniGL
