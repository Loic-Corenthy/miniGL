//===============================================================================================//
/*!
 *  \file      BaseLight.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

#include "Algebra.hpp"

namespace miniGL
{
    /*!
     *  \brief   This class is a base class for the common attributes to all types of lights
     *  \details This class cannot be used directly, a derived class must be created.
     */
    class BaseLight
    {
    public:
        enum class EType
        {
            UNDEFINED,
            DIRECTIONAL,
            POINT,
            SPOT
        };

    public:
        /*!
         *  \brief Destructor
         */
         virtual ~BaseLight(void) = 0;

        /*!
         *  \brief Set the color of the light
         *  @param pRGB is a vector whose components represent the color components: x = red, y = green, z = blue
         */
        void color(const vec3f & pRGB);

        /*!
         *  \brief Set the ambiant intensity coefficient
         *  @param pValue is a coefficient in the range [0,1]
         */
        void ambientIntensity(float pValue);

        /*!
         *  \brief Set the diffuse intensity coefficient
         *  @param pValue is a coefficient in the range [0,1]
         */
        void diffuseIntensity(float pValue);

        /*!
         *  \brief Get the color of the light(read only)
         *  @return a vector whose components represent the color components: x = red, y = green, z = blue
         */
        const vec3f & color(void) const;

        /*!
         *  \brief Get the ambiant intensity coefficient of the light (read only)
         *  @return a coefficient in the range [0,1]
         */
        float ambientIntensity(void) const;

        /*!
         *  \brief Get the diffuse intensity coefficient of the light (read only)
         *  @return a coefficient in the range [0,1]
         */
        float diffuseIntensity(void) const;

        /*!
         *  \brief Get the color of the light (read/write)
         *  @return a vector whose components represent the color components: x = red, y = green, z = blue
         */
        vec3f & color(void);

        /*!
         *  \brief Get the ambiant intensity coefficient of the light (read/write)
         *  @return a coefficient in the range [0,1]
         */
        float & ambientIntensity(void);

        /*!
         *  \brief Get the diffuse intensity coefficient of the light (read/write)
         *  @return a coefficient in the range [0,1]
         */
        float & diffuseIntensity(void);

        /*!
         *  \brief Get the type of light. This type is set by derived classes
         *  @return an enum with the light type
         */
        EType type(void) const noexcept;

        /*!
         *  \brief Get the name of the light. This name is set by derived classes
         *  @return a string with the name of the light
         */
        std::string name(void) const;

        /*!
         *  \brief Set the name of the light. By default, a name is set by derived classes
         *  @param pName is the name of the light
         */
        void name(const std::string & pName);

        /*!
         *  \brief Reset all the parameters of the light to their default values
         */
        void reset(void);

    protected:
        /*!
         *  \brief Constructor with type parameter
         *  @param pType is the type of light (directional, point or spot)
         */
        BaseLight(EType pType);

        /*!
         *  \brief Constructor with attributes parameters
         *  @param pRGB is a vector whose components represent the color components: x = red, y = green, z = blue
         *  @param pAmbientIntensity is a coefficient in the range [0,1]
         *  @param pDiffuseIntensity is a coefficient in the range [0,1]
         */
        BaseLight(EType pType, const vec3f & pRGB, float pAmbientIntensity, float pDiffuseIntensity);

        /*!
         *  \brief Default copy constructor
         */
        BaseLight(const BaseLight & pBaseLight) = default;

        /*!
         *  \brief Set the type of light. This type is set by derived classes
         *  @param enum with the light type
         */
        void type(EType pType) noexcept;

        /*!
         *  \brief Actual implementation of the reset method. Derived class must implement as needed.
         */
        virtual void resetImplementation(void);

    private:
        std::string mName;
        vec3f mColor;
        float mAmbientIntensity;
        float mDiffuseIntensity;
        EType mType = EType::UNDEFINED;

    }; // class BaseLight

} // namespace miniGL

