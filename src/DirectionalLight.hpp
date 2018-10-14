//===============================================================================================//
/*!
 *  \file      DirectionalLight.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

#include "Algebra.hpp"
#include "BaseLight.hpp"

namespace miniGL
{
    /*!
     *  \brief   This class simulates a directional light
     *  \details Directional lights are mainly characterized by their direction
     */
    class DirectionalLight : public BaseLight
    {
    public:
        /*!
         * \brief Default constructor
         */
        DirectionalLight(void);

        /*!
         * \brief Constructor with attributes parameters
         *  @param pRGB is the color of the light
         *  @param pDirection indicates the direction of the light
         *  @param pAmbientIntensity is a coefficient in the range [0,1]
         *  @param pDiffuseIntensity is a coefficient in the range [0,1]
         */
        DirectionalLight(const vec3f & pRGB, const vec3f & pDirection, float pAmbientIntensity, float pDiffuseIntensity);

        /*!
         *  \brief Destructor
         */
        ~DirectionalLight(void);

        /*!
         *  \brief Set the direction vector
         *  @param pDirection is a normalized vector indicating the direction of the light
         */
        void direction(const vec3f & pDirection);

        /*!
         *  \brief Get the direction (read only)
         *  @return a vector indicating the direction of the light
         */
        const vec3f & direction(void) const;

        /*!
         *  \brief Get the direction (read/write)
         *  @return a vector indicating the direction of the light
         */
        vec3f & direction(void);

    private:
        /*!
         *  \brief Implementation of a virtual method from BaseLight
         */
        virtual void resetImplementation(void);

    private:
        vec3f mDirection;

    }; // class DirectionalLight

} // namespace miniGL

