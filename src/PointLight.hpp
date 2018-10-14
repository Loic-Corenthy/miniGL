//===============================================================================================//
/*!
 *  \file      PointLight.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

#include "Algebra.hpp"
#include "BaseLight.hpp"
#include "EnumClassCast.hpp"

namespace miniGL
{
    /*!
     *  \brief   This class simulates a point light
     *  \details A point light is mainly described by its position and how the light is attenuated
     *           according to the distance between the enlighted object and the point light.
     */
    class PointLight : public BaseLight
    {
    public:
        enum class ATTENUATION_TYPE : unsigned int
        {
            CONSTANT    = 0,
            LINEAR      = 1,
            EXPONENTIAL = 2
        };

    public:
        /*!
         * \brief Default constructor
         */
        PointLight(void);

        /*!
         * \brief Constructor with attributes parameters
         *  @param pRGB is the color
         *  @param pPos is the position
         *  @param pAmbInt is the ambient intensity
         *  @param pDiffInt is the diffuse intensity
         *  @param pConst is the constant attenuation coefficient
         *  @param pLin is the linear attenuation coefficient
         *  @param pExp is the quadratique attenuation coefficient
         */
        PointLight(const vec3f & pRGB, const vec3f & pPos, float pAmbInt, float pDiffInt, float pConstAtt, float pLinAtt, float pExpAtt);

        /*!
         *  \brief Destructor
         */
        ~PointLight(void);

        /*!
         *  \brief Set the position of the light
         *  @param pPos is the position of the light
         */
        void position(const vec3f & pPos);

        /*!
         *  \brief Set a coefficient defining the attenuation
         *  @param pType is to select an attenuation coeffient
         *  @param pVale is the value that the corresponding coefficient will be set to
         */
        void attenuation(ATTENUATION_TYPE pType, float pValue);

        /*!
         *  \brief Get the position of the light (read only)
         *  @return the position of the light
         */
        const vec3f & position(void) const;

        /*!
         *  \brief Get a coefficient defining the attenuation (read only)
         *  @param pType is to select an attenuation coeffient
         *  @return pVale is the value corresponding to the coefficient
         */
        float attenuation(ATTENUATION_TYPE pType) const;

        /*!
         *  \brief Get the position of the light (read/write)
         *  @return the position of the light
         */
        vec3f & position(void);

        /*!
         *  \brief Get a coefficient defining the attenuation (read/write)
         *  @param pType is to select an attenuation coeffient
         *  @return pVale is the value corresponding to the coefficient
         */
        float & attenuation(ATTENUATION_TYPE pType);

    protected:
        /*!
         *  \brief Implementation of a virtual method from BaseLight
         */
        virtual void resetImplementation(void);

    private:
        vec3f mPosition;
        vec3f mAttenuation;

    }; // class PointLight

} // namespace miniGL
