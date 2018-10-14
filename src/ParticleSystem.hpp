//===============================================================================================//
/*!
 *  \file      ParticleSystem.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

#include <GL/glew.h>

#include "Algebra.hpp"
#include "Billboard.hpp"
#include "Texture.hpp"
#include "RandomTexture.hpp"
#include "ParticleSystemRender.hpp"
#include "MeshBase.hpp"

namespace miniGL
{
    /*!
     *  \brief   This class handles the particle system technique
     *  \details This class encapsulates all the mechanics involved in managing the transform feedback buffer
     */
    class ParticleSystem
    {
    public:
        /*!
         *  \brief Destructor
         */
        ~ParticleSystem(void);

        /*!
         *  \brief Initialize the rendering technique
         *  \param pParticle count is the maximum number of particles emitted
         *  @param pLauncherPos is the position of the launcher (from where the particles are emitted)
         */
        void init(unsigned int pParticleCount, const vec3f & pLauncherPos);

        /*!
         *  \brief
         *  \param
         *  \param
         *  \param
         */
        void render(int pDeltaTime, const mat4f & pVP, const vec3f & pCameraPos);

    private:
        /*!
         *  \brief
         *  \param
         */
        void _update(int pDeltaTime);

        /*!
         *  \brief
         *  \param
         */
        void _render(const mat4f & pVP, const vec3f & pCameraPos);

    private:
        Billboard mBillbording;
        ParticleSystemRender mRenderer;
        RandomTexture mRandomTexture;
        std::unique_ptr<Texture> mTexture;
        unsigned int mParticleCount = 0u;
        unsigned int mCurrentVB = 0u;
        unsigned int mCurrentTFB = 1u;
        GLuint mParticleBuffers[8] = {0u};
        GLuint mTransformFeedback[2] = {0u, 0u};
        GLuint mVAO[2] = {0u, 0u};

        int mTime = 0;
        bool mIsFirstRender = true;

        GLuint mQuery = 0;
        GLuint mQuerySavedToTranformFeedback = 0;
        GLuint mVerticesToRender;
        GLuint mVerticesSaved;
        GLuint mVerticesToDraw = 0;

    }; // class ParticleSystem

} // namespace miniGL
