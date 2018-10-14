//===============================================================================================//
/*!
 *  \file      ParticleSystemRender.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

#include <string>

#include <GL/glew.h>

#include "Program.hpp"
#include "Constants.hpp"
#include "Algebra.hpp"

namespace miniGL
{
    /*!
     *  \brief   This class manages the shaders for the particle system technique
     *  \details This renderer uses only a vertex and a geometry shader (no fragment shader)
     */
    class ParticleSystemRender : public Program
    {
    public:
        /*!
         *  \brief Load and compile the shaders associated to this billboarding technique.
         *         Initialize the id of the uniform variables used in the shaders.
         *  @param pPathVS is the path to the vertex shader
         *  @param pPathGS is the path to the geometry shader
         */
        void init(const std::string & pPathVS, const std::string & pPathGS);

        /*!
         *  \brief Set the lifetime of the launcher
         *  @param pValue is the lifetime in milliseconds
         */
        void launcherLifetime(float pValue);

        /*!
         *  \brief Set the initial position of the launcher
         *  @param pPosition is in world space
         */
        void launcherInitialPosition(const vec3f & pPosition);

        /*!
         *  \brief Set the lifetime of a shell
         *  @param pValue is the lifetime in milliseconds
         */
        void shellLifetime(float pValue);

        /*!
         *  \brief Set the lifetime of a secondary shell
         *  @param pValue is the lifetime in milliseconds
         */
        void secondaryShellLifetime(float pValue);

        /*!
         *  \brief Set the time difference between updates of the state of the particles
         *  @param pValue is the time in milliseconds
         */
        void deltaTime(unsigned int pValue);

        /*!
         *  \brief Set the current time
         *  @param pValue is the time in milliseconds
         */
        void time(int pValue);

        /*!
         *  \brief Set the texture unit for the random texture
         *  @param pTexUnit is the texture unit index
         */
        void randomTextureUnit(unsigned int pTexUnit);

    private:
        /*!
         *  \brief Implementation of a virtual method from Program
         */
        virtual bool checkUniformLocations(void) const final;

    private:
        GLuint mDeltaTimeLocation = Constants::invalidUniformLocation<GLuint>();
        GLuint mRandomTextureLocation = Constants::invalidUniformLocation<GLuint>();
        GLuint mTimeLocation = Constants::invalidUniformLocation<GLuint>();
        GLuint mLauncherLifetimeLocation = Constants::invalidUniformLocation<GLuint>();
        GLuint mShellLifetimeLocation = Constants::invalidUniformLocation<GLuint>();
        GLuint mSecondaryShellLifetimeLocation = Constants::invalidUniformLocation<GLuint>();
        GLuint mInitialLauncherPosition = Constants::invalidUniformLocation<GLuint>();

    }; // class ParticleSystemRender

} // namespace miniGL
