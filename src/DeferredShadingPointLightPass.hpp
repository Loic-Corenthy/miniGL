//===============================================================================================//
/*!
 *  \file      DeferredShadingPointLightPass.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

#include <GL/glew.h>

#include "DeferredShadingLightPass.hpp"
#include "PointLight.hpp"
#include "Constants.hpp"

namespace miniGL
{
    /*!
     *  \brief Renders the effects of the point lights on the scene
     *  \details This class derives from DeferredShadingLightPass. It contains one or more point lights
     *           and handles the shaders using them to render their effects on the scene.
     */
    class DeferredShadingPointLightPass : public DeferredShadingLightPass
    {
    private:
        enum class EParam : size_t
        {
            COLOR,
            AMBIANT_INTENSITY,
            DIFFUSE_INTENSITY,
            POSITION,
            ATTENUATION_CONSTANT_COEFF,
            ATTENUATION_LINEAR_COEFF,
            ATTENUATION_EXPONENTIAL_COEFF
        };

    public:
        /*!
         * \brief Default constuctor
         */
        DeferredShadingPointLightPass(void);

        /*!
         * \brief Destructor
         */
        virtual ~DeferredShadingPointLightPass(void);

        /*!
         * \brief Load, build and link shaders to render the point lights
         */
        void init(void);

        /*!
         *  \brief Update the current parameters on the GPU
         *  \param pLight is the light whose parameters will be used by the shader
         */
        void updateLightState(const PointLight & pLight);

    private:
        /*!
         *  \brief Implementation of a virtual method from Program
         */
        virtual bool checkUniformLocations(void) const final;

    private:
        std::array<GLuint, 7> mParameterLocations;
    }; // class DeferredShadingPointLightPass

} // namespace miniGL
