//===============================================================================================//
/*!
 *  \file      Radian.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

namespace miniGL
{
    /*!
     *  \brief   Litteral class to represent angles in Radians
     *  \details The goal of this class is to enforce the use of proper units using types
     */
    template <typename T>
    class Radian
    {
    public:
        /*!
         * \brief Default constructor
         */
        constexpr Radian(void) = default;

        /*!
         * \brief Constructor with value
         */
        constexpr Radian(T pValue);

        /*!
         * \brief Get the angle
         */
        constexpr operator T() const;

        /*!
         * \brief Get the angle in degrees
         * @return a conversion of the current angle in degrees
         */
        constexpr T toDegree(void) const;

    private:
        T mValue = 0;

    }; // class Radian

    template <typename T>
    constexpr Radian<T>::Radian(T pValue)
    :mValue(pValue)
    {
    }

    template <typename T>
    constexpr Radian<T>::operator T() const
    {
        return mValue;
    }

    template <typename T>
    constexpr T Radian<T>::toDegree(void) const
    {
        return mValue * static_cast<T>(180) / static_cast<T>(3.141592653589793238463) ;
    }

} // namespace miniGL
