//===============================================================================================//
/*!
 *  \file      Constants.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

namespace miniGL
{
    /*!
     *  \brief This class only contains static methods used to define constant values with a specific type.
     *  \details The method are constexpr to provide those constant values during compile time whenever possible
     *           No need to instanciate this class, it should contain only static methods
     */
    class Constants
    {
    public:
        /*!
         * \brief Default constructor, prevent from instanciating this class
         */
        Constants(void) = delete;

        /*!
         * \brief Get the number pi with the type specified as template parameter
         * @return pi
         */
        template<typename T>
        constexpr static T pi(void) noexcept;

        /*!
         * \brief Get pi * pValue with the type specified as template parameter
         * @param pValue is the value used to multiply pi
         * @return pi * pValue
         */
        template<typename T>
        constexpr static T piTimes(const T pValue) noexcept;

        /*!
         * \brief Get pi / pValue with the type specified as template parameter
         * @param pValue is the value used to divide pi
         * @return pi / pValue
         */
        template<typename T>
        constexpr static T piOver(const T pValue) noexcept;

        /*!
         * \brief Get the number representing an invalid material with the type specified as template parameter
         * @return the value 0xffffffff casted to the specified type
         */
        template<typename T>
        constexpr static T invalidMaterial(void) noexcept;

        /*!
         * \brief Get the number representing an invalid uniform location with the type specified as template parameter
         * @return the value 0xffffffff casted to the specified type
         */
        template<typename T>
        constexpr static T invalidUniformLocation(void) noexcept;

        /*!
         * \brief Get the number representing an invalid buffer index with the type specified as template parameter
         * @return the value 0xffffffff casted to the specified type
         */
        template<typename T>
        constexpr static T invalidBufferIndex(void) noexcept;

    }; // class Constants

    template<typename T>
    constexpr T Constants::pi(void) noexcept
    {
        return static_cast<T>(3.141592653589793238463);
    }

    template<typename T>
    constexpr T Constants::piTimes(const T pValue) noexcept
    {
        return static_cast<T>(3.141592653589793238463)*pValue;
    }

    template<typename T>
    constexpr T Constants::piOver(const T pValue) noexcept
    {
        return static_cast<T>(3.141592653589793238463)/pValue;
    }

    template<typename T>
    constexpr T Constants::invalidMaterial(void) noexcept
    {
        return static_cast<T>(0xffffffff);
    }

    template<typename T>
    constexpr T Constants::invalidUniformLocation(void) noexcept
    {
        return static_cast<T>(0xffffffff);
    }

    template<typename T>
    constexpr T Constants::invalidBufferIndex(void) noexcept
    {
        return static_cast<T>(0xffffffff);
    }

} // namespace miniGL
