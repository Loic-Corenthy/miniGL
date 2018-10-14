//===============================================================================================//
/*!
 *  \file      Degree.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

namespace miniGL
{
    /*!
     *  \brief   Litteral class to represent angles in degrees
     *  \details The goal of this class is to enforce the use of proper units using types
     */
    template <typename T>
    class Degree
    {
    public:
        /*!
         * \brief Default constructor
         */
        constexpr Degree(void) = default;

        /*!
         * \brief Constructor with value
         */
        constexpr Degree(T pValue);

        /*!
         * \brief Get the angle
         */
        constexpr operator T() const;

        /*!
         * \brief Get the angle in radians
         * @return a conversion of the current angle in radians
         */
        constexpr T toRadian(void) const;

        /*!
         * \brief Add an angle to this one
         * @return the sum of pValue and this
         */
        Degree operator+=(Degree pValue);

        /*!
         * \brief Substract an angle to this one
         * @return this minus pValue
         */
        Degree operator-=(Degree pValue);

    private:
        T mValue = 0;

    }; // class Degree

    template <typename T>
    constexpr Degree<T>::Degree(T pValue)
    :mValue(pValue)
    {
    }

    template <typename T>
    constexpr Degree<T>::operator T() const
    {
        return mValue;
    }

    template <typename T>
    constexpr T Degree<T>::toRadian(void) const
    {
        return mValue * static_cast<T>(3.141592653589793238463) / static_cast<T>(180);
    }

    template <typename T>
    Degree<T> Degree<T>::operator+=(Degree pValue)
    {
        mValue += pValue;
        return mValue;
    }

    template <typename T>
    Degree<T> Degree<T>::operator-=(Degree pValue)
    {
        mValue -= pValue;
        return mValue;
    }
} // namespace miniGL
