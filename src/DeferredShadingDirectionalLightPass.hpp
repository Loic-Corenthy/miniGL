//===============================================================================================//
/*!
 *  \file      DeferredShadingDirectionalLightPass.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

#include <GL/glew.h>

#include "DeferredShadingLightPass.hpp"
#include "DirectionalLight.hpp"

namespace miniGL
{
    /*!
     *  \brief Renders the effects of the directional light on the scene
     *  \details This class derives from DeferredShadingLightPass. It contains a directional light and
     *           handles the shaders using it to render its effects on the scene.
     */
    class DeferredShadingDirectionalLightPass : public DeferredShadingLightPass
    {
    public:
        /*!
         * \brief Default constuctor
         */
        DeferredShadingDirectionalLightPass(void);

        /*!
         * \brief Destructor
         */
        virtual ~DeferredShadingDirectionalLightPass(void);

        /*!
         * \brief Load, build and link shaders to render the directional light
         */
        void init(void);

        /*!
         *  \brief Update the lighting state in the GPU (overload)
         *  \param pLight is a pointer on a directional light
         */
        void updateLightsState(const std::shared_ptr<DirectionalLight> pLight);

    private:
        /*!
         *  \brief Implementation of a virtual method from Program
         */
        virtual bool checkUniformLocations(void) const final;

    private:
        GLuint mBaseColorLocation;
        GLuint mBaseAmbiantIntensityLocation;
        GLuint mBaseDiffuseIntensityLocation;
        GLuint mDirectionLocation;

    }; // class DeferredShadingDirectionalLightPass

} // namespace miniGL
