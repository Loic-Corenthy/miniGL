//===============================================================================================//
/*!
 *  \file      DeferredShadingSpotLightPass.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

#include <GL/glew.h>

#include "DeferredShadingLightPass.hpp"
#include "SpotLight.hpp"
#include "Constants.hpp"

namespace miniGL
{
    /*!
     *  \brief Renders the effects of the spot lights on the scene
     *  \details This class derives from DeferredShadingLightPass. It handles the shaders using spot lights
     *           to render their effects on the scene.
     */
    class DeferredShadingSpotLightPass : public DeferredShadingLightPass
    {
    private:
        enum class EParam : size_t
        {
            COLOR,
            AMBIANT_INTENSITY,
            DIFFUSE_INTENSITY,
            POSITION,
            DIRECTION,
            ATTENUATION_CONSTANT_COEFF,
            ATTENUATION_LINEAR_COEFF,
            ATTENUATION_EXPONENTIAL_COEFF,
            CUTOFF
        };

    public:
        /*!
         * \brief Default constuctor
         */
        DeferredShadingSpotLightPass(void);

        /*!
         * \brief Destructor
         */
        virtual ~DeferredShadingSpotLightPass(void);

        /*!
         * \brief Load, build and link shaders to render the point lights
         */
        void init(void);

        /*!
         *  \brief Update the current parameters on the GPU
         *  \param pLight is the light whose parameters will be used by the shader
         */
        void updateLightState(const SpotLight & pLight);

    private:
        /*!
         *  \brief Implementation of a virtual method from Program
         */
        virtual bool checkUniformLocations(void) const final;

    private:
        std::array<GLuint, 9> mParameterLocations;
    }; // class DeferredShadingSpotLightPass

} // namespace miniGL
