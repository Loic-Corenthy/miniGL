//===============================================================================================//
/*!
 *  \file      Quaternion.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

#include <array>
#include <type_traits>

#include "Vector.hpp"
#include "Matrix.hpp"

namespace miniGL
{
    /*!
     *  \brief This class implements a quaternion with a template type for the coefficients
     *  \details This template class represents a quaternion. It hanldes the main simple operations.
     *           A quaternion Q is defined as Q = x * i + y * j + z * k + w
     */
    template<typename T>
    class Quaternion
    {
        public:
        /*!
         * \brief Default constructor
         */
        Quaternion(void);

        /*!
         * \brief Constructor with parameters for each coefficient
         * @param pX is the value of x (imaginary component)
         * @param pY is the value of y (imaginary component)
         * @param pZ is the value of z (imaginary component)
         * @param pW is the value of w (angle)
         */
        explicit Quaternion(T pX, T pY, T pZ, T pW);

        /*!
         * \brief Copy constructor
         * @param pQuaternion is the quaternion to copy coefficients from
         */
        Quaternion(const Quaternion<T> & pQuaternion);

        /*!
         * \brief Copy operator
         * @param pQuaternion is the quaternion to copy coefficients from
         * @return a reference on this object
         */
        Quaternion<T> & operator=(const Quaternion<T> & pQuaternion);

        /*!
         * \brief Destructor
         */
        ~Quaternion(void);

        /*!
         * \brief Comparision operator
         * @param pQuaternion is the quaternion to compare coefficients from
         * @return true if all coordinates of this quaternion and pQuaternion are equal.
         *            (We do not take into account the case pQuaternion = -this)
         */
        bool operator==(const Quaternion<T> & pQuaternion) const;

        /*!
         * \brief Multiplication with a quaternion operator
         * @param pQuat is the right and side of the multiplication
         * @return a new quaternion as the product of the two quaternions
         */
        Quaternion<T> operator*(const Quaternion<T> & pQuat);

        /*!
         * \brief Multiplication with a vector operator
         * @param pVec is the right and side of the multiplication
         * @return a new quaternion as the product of this quaternions and a vector
         */
        Quaternion<T> operator*(const Vector<T, THREE, 3> & pVec);

        /*!
         * \brief Cast our quaternion into an equivalent 4x4 matrix
         */
        operator Matrix<T, FOUR, FOUR, 4, 4>(void);

        /*!
         * \brief Compute the length of the quaternion
         * @return the length of the quaternion
         */
        double length(void) const;

        /*!
         * \brief Normalize this quaternion: divide each coefficient by the length.
         */
        void normalize(void);

        /*!
         * \brief Create a new vector which corresponds to the normalized version of this quaternion.
         *        Do not modify this object.
         * @return a normalized version of this quaternion
         */
        Quaternion<T> normalized(void) const;

        /*!
         * \brief Conjugate this quaternion, i.e. multiply the imaginary coefficient by -1
         */
        void conjugate(void);

        /*!
         * \brief Create a new vector which corresponds to the conjugated version of this quaternion.
         *        Do not modify this object.
         * @return the conjugate of this quaternion
         */
        Quaternion<T> conjugated(void) const;

        /*!
         * \brief Display the coefficients of a quaternion using cout
         * @param pBlancLine determine if endl will be called twice (if true) or once (if false)
         * @param pWidth is the number of characters used to display each coefficient (for alignment purposes on screen)
         */
        void display(bool pBlancLine = true, unsigned int pWidth = 5) const;

        /*!
         * \brief Get the first imaginary coefficient (read only)
         * @return the value of the first coefficient
         */
        T x(void) const noexcept;

        /*!
         * \brief Get the first imaginary coefficient (read/write)
         * @return the value of the first coefficient
         */
        T & x(void) noexcept;

        /*!
         * \brief Get the second imaginary coefficient (read only)
         * @return the value of the second coefficient
         */
        T y(void) const noexcept;

        /*!
         * \brief Get the second imaginary coefficient (read/write)
         * @return the value of the second coefficient
         */
        T & y(void) noexcept;

        /*!
         * \brief Get the third imaginary coefficient (read only)
         * @return the value of the third coefficient
         */
        T z(void) const noexcept;

        /*!
         * \brief Get the third imaginary coefficient (read/write)
         * @return the value of the third coefficient
         */
        T & z(void) noexcept;

        /*!
         * \brief Get the real coefficient (read only)
         * @return the value of the fourth coefficient
         */
        T w(void) const noexcept;

        /*!
         * \brief Get the real coefficient (read/write)
         * @return the value of the fourth coefficient
         */
        T & w(void) noexcept;

    private:
        std::array<T, 4> mCoeff;

    }; // class Quaternion

    template<typename T>
    Quaternion<T>::Quaternion(void)
    :mCoeff({0})
    {

    }

    template<typename T>
    Quaternion<T>::Quaternion(T pX, T pY, T pZ, T pW)
    {
        mCoeff[0] = pX;
        mCoeff[1] = pY;
        mCoeff[2] = pZ;
        mCoeff[3] = pW;
    }

    template<typename T>
    Quaternion<T>::Quaternion(const Quaternion<T> & pQuaternion)
    :mCoeff(pQuaternion.mCoeff)
    {

    }

    template<typename T>
    Quaternion<T> & Quaternion<T>::operator=(const Quaternion<T> & pQuaternion)
    {
        if (this != & pQuaternion)
            mCoeff = pQuaternion.mCoeff;

        return *this;
    }

    template<typename T>
    Quaternion<T>::~Quaternion(void)
    {

    }

    template<typename T>
    bool Quaternion<T>::operator==(const Quaternion<T> & pQuaternion) const
    {
        return (mCoeff[0] == pQuaternion.mCoeff[0] && mCoeff[1] == pQuaternion.mCoeff[1] && mCoeff[2] == pQuaternion.mCoeff[2] && mCoeff[3] == pQuaternion.mCoeff[3]);
    }

    template<typename T>
    Quaternion<T> Quaternion<T>::operator*(const Quaternion<T> & pQuat)
    {
        return Quaternion<T>((mCoeff[0] * pQuat.mCoeff[3]) + (mCoeff[3] * pQuat.mCoeff[0]) + (mCoeff[1] * pQuat.mCoeff[2]) - (mCoeff[2] * pQuat.mCoeff[1]),
                             (mCoeff[1] * pQuat.mCoeff[3]) + (mCoeff[3] * pQuat.mCoeff[1]) + (mCoeff[2] * pQuat.mCoeff[0]) - (mCoeff[0] * pQuat.mCoeff[2]),
                             (mCoeff[2] * pQuat.mCoeff[3]) + (mCoeff[3] * pQuat.mCoeff[2]) + (mCoeff[0] * pQuat.mCoeff[1]) - (mCoeff[1] * pQuat.mCoeff[0]),
                             (mCoeff[3] * pQuat.mCoeff[3]) - (mCoeff[0] * pQuat.mCoeff[0]) - (mCoeff[1] * pQuat.mCoeff[1]) - (mCoeff[2] * pQuat.mCoeff[2]));
    }

    template<typename T>
    Quaternion<T> Quaternion<T>::operator*(const Vector<T, THREE, 3> & pVec)
    {
        return Quaternion<T>((mCoeff[3] * pVec.x()) + (mCoeff[1] * pVec.z()) - (mCoeff[2] * pVec.y()),
                             (mCoeff[3] * pVec.y()) + (mCoeff[2] * pVec.x()) - (mCoeff[0] * pVec.z()),
                             (mCoeff[3] * pVec.z()) + (mCoeff[0] * pVec.y()) - (mCoeff[1] * pVec.x()),
                           - (mCoeff[0] * pVec.x()) - (mCoeff[1] * pVec.y()) - (mCoeff[2] * pVec.z()));
    }

    template<typename T>
    Quaternion<T>::operator Matrix<T, FOUR, FOUR, 4, 4>(void)
    {
        /*! \todo We should make sure the quaternion is normalized before contructing the equivalent 4x4 rotation matrix */
        /*! \todo Test this method */
        assert(abs(length() - 1.0) < 0.001 && "Quaternion should be normalized before contructing the equivalent 4x4 rotation matrix");

        Matrix<T, FOUR, FOUR, 4, 4> lRes;

        const T lASquare = mCoeff[3] * mCoeff[3];
        const T lBSquare = mCoeff[0] * mCoeff[0];
        const T lCSquare = mCoeff[1] * mCoeff[1];
        const T lDSquare = mCoeff[2] * mCoeff[2];

        const T lAB = mCoeff[3] * mCoeff[0];
        const T lAC = mCoeff[3] * mCoeff[1];
        const T lAD = mCoeff[3] * mCoeff[2];
        const T lBC = mCoeff[0] * mCoeff[1];
        const T lBD = mCoeff[0] * mCoeff[2];
        const T lCD = mCoeff[1] * mCoeff[2];

        lRes(0,0) = lASquare + lBSquare - lCSquare - lDSquare;  lRes(0,1) = 2.0 * (lBC - lAD);                          lRes(0,2) = 2.0 * (lBD + lAC);                         lRes(0,3) = 0.0;
        lRes(1,0) = 2.0 * (lBC + lAD);                          lRes(1,1) = lASquare - lBSquare + lCSquare - lDSquare;  lRes(1,2) = 2.0 * (lCD - lAB);                         lRes(1,3) = 0.0;
        lRes(2,0) = 2.0 * (lBD - lAC);                          lRes(2,1) = 2.0 * (lCD + lAB);                          lRes(2,2) = lASquare - lBSquare - lCSquare + lDSquare; lRes(2,3) = 0.0;
        lRes(3,0) = 0.0;                                        lRes(3,1) = 0.0;                                        lRes(3,2) = 0.0;                                       lRes(3,3) = 1.0;

        return lRes;
    }

    template<typename T>
    double Quaternion<T>::length(void) const
    {
        return sqrt(mCoeff[0]*mCoeff[0] + mCoeff[1]*mCoeff[1] + mCoeff[2]*mCoeff[2] + mCoeff[3]*mCoeff[3]);
    }

    template<typename T>
    void Quaternion<T>::normalize(void)
    {
        if (mCoeff[0] != 0 || mCoeff[1] != 0 || mCoeff[2] != 0 || mCoeff[3] != 0)
        {
            double lInvLength = 1.0 / length();

            mCoeff[0] *= lInvLength;
            mCoeff[1] *= lInvLength;
            mCoeff[2] *= lInvLength;
            mCoeff[3] *= lInvLength;
        }
    }

    template<typename T>
    Quaternion<T> Quaternion<T>::normalized(void) const
    {
        Quaternion<T> lRes(*this);

        if (mCoeff[0] != 0 || mCoeff[1] != 0 || mCoeff[2] != 0 || mCoeff[3] != 0)
        {
            double lInvLength = 1.0 / length();

            lRes.mCoeff[0] *= lInvLength;
            lRes.mCoeff[1] *= lInvLength;
            lRes.mCoeff[2] *= lInvLength;
            lRes.mCoeff[3] *= lInvLength;
        }

        return lRes;
    }

    template<typename T>
    void Quaternion<T>::conjugate(void)
    {
        static_assert(!std::is_unsigned<T>(), "Cannot compute the conjugate using an unsigned type");
        mCoeff[0] = -mCoeff[0];
        mCoeff[1] = -mCoeff[1];
        mCoeff[2] = -mCoeff[2];
    }

    template<typename T>
    Quaternion<T> Quaternion<T>::conjugated(void) const
    {
        static_assert(!std::is_unsigned<T>(), "Cannot compute the conjugate using an unsigned type");
        return Quaternion<T>(-mCoeff[0], -mCoeff[1], -mCoeff[2], mCoeff[3]);
    }

    template<typename T>
    void Quaternion<T>::display(bool pBlancLine, unsigned int pWidth) const
    {
        for (const auto it : mCoeff)
            std::cout << std::setw(pWidth) << it  << " " ;

        std::cout << std::endl;

        if (pBlancLine)
            std::cout << std::endl;
    }

    template<typename T>
    T Quaternion<T>::x(void) const noexcept
    {
        return mCoeff[0];
    }

    template<typename T>
    T & Quaternion<T>::x(void) noexcept
    {
        return mCoeff[0];
    }

    template<typename T>
    T Quaternion<T>::y(void) const noexcept
    {
        return mCoeff[1];
    }

    template<typename T>
    T & Quaternion<T>::y(void) noexcept
    {
        return mCoeff[1];
    }

    template<typename T>
    T Quaternion<T>::z(void) const noexcept
    {
        return mCoeff[2];
    }

    template<typename T>
    T & Quaternion<T>::z(void) noexcept
    {
        return mCoeff[2];
    }

    template<typename T>
    T Quaternion<T>::w(void) const noexcept
    {
        return mCoeff[3];
    }

    template<typename T>
    T & Quaternion<T>::w(void) noexcept
    {
        return mCoeff[3];
    }

} // namespace miniGL
