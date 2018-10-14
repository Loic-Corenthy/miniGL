//===============================================================================================//
/*!
 *  \file      Vector.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

#include <array>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <cassert>
#include <type_traits>

#include "InternalMathType.hpp"

namespace miniGL
{
    /*!
     *  \brief This class is the base class for vectors of different sizes
     *  \details This template class is the base class for vectors. It must be compiled  with the speed optimization
     *         to ensure to unroll the for loops.
     */
    template<typename T, typename SIZE_TYPE, unsigned int SIZE>
    class Vector
    {
    public:
        /*!
         * \brief Default constructor
         */
        Vector(void);

        /*!
         * \brief Constructor with same scalar parameter for all coefficients
         * @param pScalar is the scalar value copied to all the coefficients
         */
        explicit Vector(T pScalar);

        /*!
         * \brief Specific constructor for vectors with 3 coordinates
         * @param pX is the x coordinate
         * @param pY is the y coordinate
         */
        template<typename INTERNAL = SIZE_TYPE>
        Vector(typename std::enable_if<(std::is_same<INTERNAL, TWO>::value), T>::type pX, T pY)
        :mCoefficients{{pX, pY}}
        {
        }

        /*!
         * \brief Specific constructor for vectors with 3 coordinates
         * @param pX is the x coordinate
         * @param pY is the y coordinate
         * @param pZ is the z coordinate
         */
        template<typename INTERNAL = SIZE_TYPE>
        Vector(typename std::enable_if<(std::is_same<INTERNAL,THREE>::value), T>::type pX, T pY, T pZ)
        :mCoefficients{{pX, pY, pZ}}
        {

        }

        /*!
         * \brief Specific constructor for vectors with 4 coordinates
         * @param pX is the x coordinate
         * @param pY is the y coordinate
         * @param pZ is the z coordinate
         * @param pW is the w coordinate
         */
        template<typename INTERNAL = SIZE_TYPE>
        Vector(typename std::enable_if<(std::is_same<INTERNAL, FOUR>::value), T>::type pX, T pY, T pZ, T pW)
        :mCoefficients{{pX, pY, pZ, pW}}
        {

        }

        /*!
         * \brief Constructor with same scalar parameter for all coefficients
         * @param pCoefficients is a list of coefficients that will be used to set the coordinates of the vector.
         *        There must be as many coefficients as SIZE
         */
        explicit Vector(const std::initializer_list<T> & pCoefficients);

        /*!
         * \brief Copy constructor
         * @param pVector is the vector to copy coefficients from
         */
        Vector(const Vector<T, SIZE_TYPE, SIZE> & pVector);

        /*!
         * \brief Move constructor
         * @param pVector is the vector to move coefficients from
         */
        Vector(Vector<T, SIZE_TYPE, SIZE> && pVector);

        /*!
         * \brief Copy operator
         * @param pVector is the vector to copy coefficients from
         * @return a reference on this object
         */
        Vector<T, SIZE_TYPE, SIZE> & operator=(const Vector<T, SIZE_TYPE, SIZE> & pVector);

        /*!
         * \brief Destructor
         */
        ~Vector(void);

        /*!
         * \brief Comparision operator
         * @param pVector is the vector to compare coefficients from
         * @return true if all coordinates of this vector and pVector are equal
         */
        bool operator==(const Vector<T, SIZE_TYPE, SIZE> & pVector);

        /*!
         * \brief Accessor (read/write)
         * @param pIndex in the index of the coefficient to access
         * @return a reference on the corresponding coefficient
         */
        T & operator[](size_t pIndex);

        /*!
         * \brief Accessor (read only)
         * @param pIndex in the index of the coefficient to access
         * @return the corresponding coefficient
         */
        T operator[](size_t pIndex) const;

        /*!
         * \brief Addition operator. Do not modify this object.
         * @param pVector is the vector to add to this one
         * @return a vector corresponding to this vector plus pVector
         */
        Vector<T, SIZE_TYPE, SIZE> operator+(const Vector<T, SIZE_TYPE, SIZE> & pVector) const;

        /*!
         * \brief Substraction operator. Do not modify this object.
         * @param pVector is the vector to substract from this one
         * @return a vector corresponding to this vector minus pVector
         */
        Vector<T, SIZE_TYPE, SIZE> operator-(const Vector<T, SIZE_TYPE, SIZE> & pVector) const;

        /*!
         * \brief Multiplication operator. Do not modify this object.
         * @param pScalar is the value that will multiply each coefficient of this object
         * @return a vector corresponding to this vector times pScalar
         */
        Vector<T, SIZE_TYPE, SIZE> operator*(T pScalar) const;

        /*!
         * \brief Division operator. Do not modify this object.
         * @param pScalar is the value that will divide each coefficient of this object
         * @return a vector corresponding to this vector divided by pScalar
         */
        Vector<T, SIZE_TYPE, SIZE> operator/(T pScalar) const;

        /*!
         * \brief Addition with a vector. Modify this object
         * @param pVector is the vector to add to this one
         * @return a reference on this object after the operation
         */
        Vector<T, SIZE_TYPE, SIZE> & operator+=(const Vector<T, SIZE_TYPE, SIZE> & pVector);

        /*!
         * \brief Substraction operator. Do not modify this object.
         * @param pVector is the vector to substract from this one
         * @return a reference on this object after the operation
         */
        Vector<T, SIZE_TYPE, SIZE> & operator-=(const Vector<T, SIZE_TYPE, SIZE> & pVector);

        /*!
         * \brief Multiplication by a scalar operator
         * @param pScalar is the scalar value that will multiply all the coefficients
         * @return a reference on this object after the operation
         */
        Vector<T, SIZE_TYPE, SIZE> & operator*=(T pScalar);

        /*!
         * \brief Division by a scalar operator
         * @param pScalar is the scalar value from which all the coefficients will be divided
         * @return a reference on this object after the operation
         */
        Vector<T, SIZE_TYPE, SIZE> & operator/=(T pScalar);

        /*!
         * \brief Get the first coefficient (read only)
         * @return the value of the first coefficient
         */
        T x(void) const noexcept
        {
            return mCoefficients[0];
        }

        /*!
         * \brief Get the first coefficient (read/write)
         * @return the value of the first coefficient
         */
        T & x(void) noexcept
        {
            return mCoefficients[0];
        }

        /*!
         * \brief Get the second coefficient (read only)
         * @return the value of the second coefficient (according to dimension of the vector)
         */
        template<typename INTERNAL = SIZE_TYPE>
        typename std::enable_if<!std::is_same<INTERNAL,ONE>::value ,T>::type y(void) const noexcept
        {
            return mCoefficients[1];
        }

        /*!
         * \brief Get the second coefficient (read/write)
         * @return the value of the second coefficient (according to dimension of the vector)
         */
        template<typename INTERNAL = SIZE_TYPE>
        typename std::enable_if<!std::is_same<INTERNAL,ONE>::value ,T>::type & y(void) noexcept
        {
            return mCoefficients[1];
        }

        /*!
         * \brief Get the third coefficient (read only)
         * @return the value of the third coefficient (according to dimension of the vector)
         */
        template<typename INTERNAL = SIZE_TYPE>
        typename std::enable_if<!(std::is_same<INTERNAL,ONE>::value || std::is_same<INTERNAL,TWO>::value) , T>::type z(void) const noexcept
        {
            return mCoefficients[2];
        }

        /*!
         * \brief Get the third coefficient (read/write)
         * @return the value of the third coefficient (according to dimension of the vector)
         */
        template<typename INTERNAL = SIZE_TYPE>
        typename std::enable_if<!(std::is_same<INTERNAL,ONE>::value || std::is_same<INTERNAL,TWO>::value) , T>::type & z(void) noexcept
        {
            return mCoefficients[2];
        }

        /*!
         * \brief Get the fourth coefficient (read only)
         * @return the value of the fourth coefficient (according to dimension of the vector)
         */
        template<typename INTERNAL = SIZE_TYPE>
        typename std::enable_if<!(std::is_same<INTERNAL,ONE>::value || std::is_same<INTERNAL,TWO>::value || std::is_same<INTERNAL,THREE>::value) ,T>::type w(void) const noexcept
        {
            return mCoefficients[3];
        }

        /*!
         * \brief Get the fourth coefficient (read/write)
         * @return the value of the fourth coefficient (according to dimension of the vector)
         */
        template<typename INTERNAL = SIZE_TYPE>
        typename std::enable_if<!(std::is_same<INTERNAL,ONE>::value || std::is_same<INTERNAL,TWO>::value || std::is_same<INTERNAL,THREE>::value) ,T>::type & w(void) noexcept
        {
            return mCoefficients[3];
        }

        /*!
         * \brief Compute the length of the vector
         * @return the length of the vector
         */
        double length(void) const;

        /*!
         * \brief Normalize this vector
         */
        void normalize(void);

        /*!
         * \brief Create a new vector which corresponds to the normalized version of this vector.
         *        Do not modify this object.
         * @return a normalized version of this vector
         */
        Vector<T, SIZE_TYPE, SIZE> normalized(void) const;

        /*!
         * \brief Compute a dot product
         * @param pVector is the right hand side of the dot product
         * @return a scalar value
         */
        T dot(const Vector<T, SIZE_TYPE, SIZE> & pVector);

        /*!
         * \brief Compute the cross product between 2 vectors
         * @param pVec is the right hand side of the product
         * @return a new vector which is the cross product of this vector and pVec
         */
        template<typename INTERNAL = SIZE_TYPE>
        typename std::enable_if<std::is_same<INTERNAL,THREE>::value , Vector<T, SIZE_TYPE, SIZE>>::type cross(const Vector<T, SIZE_TYPE, SIZE> & pVec) const
        {
             Vector<T, SIZE_TYPE, SIZE> lRes({mCoefficients[1] * pVec.mCoefficients[2] - mCoefficients[2] * pVec.mCoefficients[1],
                                              mCoefficients[2] * pVec.mCoefficients[0] - mCoefficients[0] * pVec.mCoefficients[2],
                                              mCoefficients[0] * pVec.mCoefficients[1] - mCoefficients[1] * pVec.mCoefficients[0]});

            return lRes;
        }

        /*!
         * \brief Display the coefficients of a vector using cout
         * @param pBlancLine determine if endl will be called twice (if true) or once (if false)
         * @param pWidth is the number of characters used to display each coefficient (for alignment purposes on screen)
         */
        void display(bool pBlancLine = true, unsigned int pWidth = 5) const;

    protected:
        std::array<T, SIZE> mCoefficients;

    }; // class Vector


    template<typename T, typename SIZE_TYPE, unsigned int SIZE>
    Vector<T, SIZE_TYPE, SIZE>::Vector(void)
    :mCoefficients{{0}}
    {
    }

    template<typename T, typename SIZE_TYPE, unsigned int SIZE>
    Vector<T, SIZE_TYPE, SIZE>::Vector(T pScalar)
    {
        for(size_t i = 0; i < SIZE; ++i)
            mCoefficients[i] = pScalar;
    }

    template<typename T, typename SIZE_TYPE, unsigned int SIZE>
    Vector<T, SIZE_TYPE, SIZE>::Vector(const std::initializer_list<T> & pCoefficients)
    {
        size_t i = 0;
        for(auto coeff : pCoefficients)
            mCoefficients[i++] = coeff;
    }

    template<typename T, typename SIZE_TYPE, unsigned int SIZE>
    Vector<T, SIZE_TYPE, SIZE>::Vector(const Vector<T, SIZE_TYPE, SIZE> & pVector)
    :mCoefficients(pVector.mCoefficients)
    {

    }

    template<typename T, typename SIZE_TYPE, unsigned int SIZE>
    Vector<T, SIZE_TYPE, SIZE>::Vector(Vector<T, SIZE_TYPE, SIZE> && pVector)
    :mCoefficients(std::move(pVector.mCoefficients))
    {

    }

    template<typename T, typename SIZE_TYPE, unsigned int SIZE>
    Vector<T, SIZE_TYPE, SIZE> & Vector<T, SIZE_TYPE, SIZE>::operator=(const Vector<T, SIZE_TYPE, SIZE> & pVector)
    {
        if (this != & pVector)
        {
            for(size_t i = 0; i < SIZE; ++i)
                mCoefficients[i] = pVector.mCoefficients[i];
        }

        return *this;
    }

    template<typename T, typename SIZE_TYPE, unsigned int SIZE>
    Vector<T, SIZE_TYPE, SIZE>::~Vector(void)
    {

    }

    template<typename T, typename SIZE_TYPE, unsigned int SIZE>
    bool Vector<T, SIZE_TYPE, SIZE>::operator==(const Vector<T, SIZE_TYPE, SIZE> & pVector)
    {
        bool lRes = true;

        for (size_t i = 0; i < SIZE; ++i)
            lRes &= (mCoefficients[i] == pVector[i]);

        return lRes;
    }

    template<typename T, typename SIZE_TYPE, unsigned int SIZE>
    inline T & Vector<T, SIZE_TYPE, SIZE>::operator[](size_t pIndex)
    {
        return mCoefficients[pIndex];
    }

    template<typename T, typename SIZE_TYPE, unsigned int SIZE>
    inline T Vector<T, SIZE_TYPE, SIZE>::operator[](size_t pIndex) const
    {
        return mCoefficients[pIndex];
    }

    template<typename T, typename SIZE_TYPE, unsigned int SIZE>
    inline Vector<T, SIZE_TYPE, SIZE> Vector<T, SIZE_TYPE, SIZE>::operator+(const Vector<T, SIZE_TYPE, SIZE> & pVector) const
    {
        Vector<T, SIZE_TYPE, SIZE> lRes;

        for (size_t i = 0; i < SIZE; ++i)
            lRes[i] = mCoefficients[i] + pVector.mCoefficients[i];

        return lRes;
    }

    template<typename T, typename SIZE_TYPE, unsigned int SIZE>
    inline Vector<T, SIZE_TYPE, SIZE> Vector<T, SIZE_TYPE, SIZE>::operator-(const Vector<T, SIZE_TYPE, SIZE> & pVector) const
    {
        Vector<T, SIZE_TYPE, SIZE> lRes;

        for (size_t i = 0; i < SIZE; ++i)
            lRes[i] = mCoefficients[i] - pVector.mCoefficients[i];

        return lRes;
    }

    template<typename T, typename SIZE_TYPE, unsigned int SIZE>
    Vector<T, SIZE_TYPE, SIZE> Vector<T, SIZE_TYPE, SIZE>::operator*(T pScalar) const
    {
        Vector<T, SIZE_TYPE, SIZE> lRes;

        for (size_t i = 0; i < SIZE; ++i)
            lRes[i] = mCoefficients[i] * pScalar;

        return lRes;
    }

    template<typename T, typename SIZE_TYPE, unsigned int SIZE>
    Vector<T, SIZE_TYPE, SIZE> Vector<T, SIZE_TYPE, SIZE>::operator/(T pScalar) const
    {
        assert(pScalar != 0.0);

        Vector<T, SIZE_TYPE, SIZE> lRes;

        for (size_t i = 0; i < SIZE; ++i)
            lRes[i] = mCoefficients[i] / pScalar;

        return lRes;
    }

    template<typename T, typename SIZE_TYPE, unsigned int SIZE>
    Vector<T, SIZE_TYPE, SIZE> & Vector<T, SIZE_TYPE, SIZE>::operator+=(const Vector<T, SIZE_TYPE, SIZE> & pVector)
    {
        for (size_t i = 0; i < SIZE; ++i)
            mCoefficients[i] += pVector.mCoefficients[i];

        return *this;
    }

    template<typename T, typename SIZE_TYPE, unsigned int SIZE>
    Vector<T, SIZE_TYPE, SIZE> & Vector<T, SIZE_TYPE, SIZE>::operator-=(const Vector<T, SIZE_TYPE, SIZE> & pVector)
    {
        for (size_t i = 0; i < SIZE; ++i)
            mCoefficients[i] -= pVector.mCoefficients[i];

        return *this;
    }

    template<typename T, typename SIZE_TYPE, unsigned int SIZE>
    inline Vector<T, SIZE_TYPE, SIZE> & Vector<T, SIZE_TYPE, SIZE>::operator*=(T pScalar)
    {
        for (size_t i = 0; i < SIZE; ++i)
            mCoefficients[i] *= pScalar;

        return *this;
    }

    template<typename T, typename SIZE_TYPE, unsigned int SIZE>
    Vector<T, SIZE_TYPE, SIZE> & Vector<T, SIZE_TYPE, SIZE>::operator/=(T pScalar)
    {
        assert(pScalar != 0.0);

        for (size_t i = 0; i < SIZE; ++i)
            mCoefficients[i] /= pScalar;

        return *this;
    }

    template<typename T, typename SIZE_TYPE, unsigned int SIZE>
    inline double Vector<T, SIZE_TYPE, SIZE>::length(void) const
    {
        T lSum = 0;

        for (size_t i = 0; i < SIZE; ++i)
            lSum += mCoefficients[i]*mCoefficients[i];

        return std::sqrt(lSum);
    }

    template<typename T, typename SIZE_TYPE, unsigned int SIZE>
    void Vector<T, SIZE_TYPE, SIZE>::normalize(void)
    {
        const double lLength = length();

        if (lLength != 0.0)
        {
            const double lInv = 1.0/lLength;

            for (size_t i = 0; i < SIZE; ++i)
                mCoefficients[i] *= lInv;
        }
    }

    template<typename T, typename SIZE_TYPE, unsigned int SIZE>
    inline Vector<T, SIZE_TYPE, SIZE> Vector<T, SIZE_TYPE, SIZE>::normalized(void) const
    {
        Vector<T, SIZE_TYPE, SIZE> lRes(*this);

        lRes.normalize();

        return lRes;
    }

    template<typename T, typename SIZE_TYPE, unsigned int SIZE>
    inline T Vector<T, SIZE_TYPE, SIZE>::dot(const Vector<T, SIZE_TYPE, SIZE> & pVector)
    {
        T lRes = 0;

        for (size_t i = 0; i < SIZE; ++i)
            lRes += mCoefficients[i]*pVector[i];

        return lRes;
    }

    template<typename T, typename SIZE_TYPE, unsigned int SIZE>
    void Vector<T, SIZE_TYPE, SIZE>::display(bool pBlancLine, unsigned int pWidth) const
    {
        for (size_t i = 0; i < SIZE; ++i)
            std::cout << std::setw(pWidth) << mCoefficients[i] << " " ;

        std::cout << std::endl;

        if (pBlancLine)
            std::cout << std::endl;
    }

} // namespace miniGL
