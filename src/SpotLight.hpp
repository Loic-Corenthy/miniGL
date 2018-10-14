//===============================================================================================//
/*!
 *  \file      SpotLight.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

#include "Algebra.hpp"
#include "PointLight.hpp"
#include "Angle.hpp"

namespace miniGL
{
    /*!
     *  \brief   This class simulates a spot light
     *  \details A spot light is a point light that also has a direction and a cutoff angle around the main
     *           direcition of the light
     */
    class SpotLight : public PointLight
    {
    public:
        /*!
         *  \brief Default constructor
         */
        SpotLight(void);

        /*!
         *  \brief Constructor with attributes parameters
         *  @param pRGB is the color
         *  @param pPos is the position
         *  @param pDir is the main direction, centre of the cone of light
         *  @param pAmbInt is the ambient intensity
         *  @param pDiffInt is the diffuse intensity
         *  @param pConstAtt is the constant attenuation coefficient
         *  @param pLinAtt is the linear attenuation coefficient
         *  @param pExpAtt is the quadratique attenuation coefficient
         *  @param pCutoff is the cutoff angle in degrees
         */
        SpotLight(const vec3f & pRGB, const vec3f & pPos, const vec3f & pDir, float pAmbInt, float pDiffInt, float pConstAtt, float pLinAtt, float pExpAtt, float pCutoff);

        /*!
         *  \brief Destructor
         */
        ~SpotLight(void);

        /*!
         *  \brief Set the direction of the light
         *  @param pDir is the main direction, centre of the cone of light
         */
        void direction(const vec3f & pDir);

        /*!
         *  \brief Set a coefficient defining the cutoff angle
         *  @param pAngle is the angle
         */
        void cutoff(degreef pAngle);

        /*!
         *  \brief Get the direction of the light (read only)
         *  @return the main direction, centre of the cone of light
         */
        const vec3f & direction(void) const;

        /*!
         *  \brief Get a coefficient defining the cutoff (read only)
         *  @return the cutoff angle in degrees
         */
        degreef cutoff(void) const;

        /*!
         *  \brief Get the direction of the light (read/write)
         *  @return the main direction, centre of the cone of light
         */
        vec3f & direction(void);

        /*!
         *  \brief Get a coefficient defining the cutoff (read/write)
         *  @return the cutoff angle in degrees
         */
        degreef & cutoff(void);

    private:
        /*!
         *  \brief Implementation of a virtual method from BaseLight
         */
        virtual void resetImplementation(void);


    private:
        vec3f mDirection;
        degreef mCutoff;

    }; // class SpotLight

} // namespace miniGL

