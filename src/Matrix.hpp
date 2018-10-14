//===============================================================================================//
/*!
 *  \file      Matrix.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

#include <cassert>
#include <iostream>
#include <iomanip>
#include <utility>

#include "InternalMathType.hpp"
#include "Vector.hpp"

namespace miniGL
{
    /*!
     *  \brief This class is the base class for matrices of different sizes
     *  \details This template class is the base class for matrices of type T with ROW number of rows and COL number of
     *           columns. It must be compiled with the speed optimization to ensure to unroll the for loops.
     */
    template<typename T, typename ROW_TYPE, typename COL_TYPE, unsigned int ROW, unsigned int COL>
    class Matrix
    {
    public:
        /*!
         * \brief Default constructor.
         */
        Matrix(void);

        /*!
         * \brief Constructor with scalar parameter.
         * @param pScalar is the scalar value that will be set to the diagonal coefficients
         */
        explicit Matrix(T pScalar);

        /*!
         * \brief Copy constructor
         * @param pMatrix is the object to copy parameters from
         */
        Matrix(const Matrix<T, ROW_TYPE, COL_TYPE, ROW, COL> & pMatrix);

        /*!
         * \brief Destructor
         */
        ~Matrix(void);

        /*!
         * \brief Copy operator
         * @param pMatrix is the object to copy parameters from
         * @return a pointer on this object
         */
        Matrix & operator=(const Matrix<T, ROW_TYPE, COL_TYPE, ROW, COL> & pMatrix);

        /*!
         * \brief Comparision operator
         * @param pMatrix is the matrix to compare coefficients from
         * @return true if all coordinates of this matrix and pMatrix are equal
         */
        bool operator==(const Matrix<T, ROW_TYPE, COL_TYPE, ROW, COL> & pMatrix);

        /*!
         * \brief Accessor (read/write)
         * @param pIndex in the index of the coefficient to access
         * @return a reference on the corresponding coefficient
         */
        T & operator()(size_t pRow, size_t pCol);

        /*!
         * \brief Accessor (read only)
         * @param pIndex in the index of the coefficient to access
         * @return the corresponding coefficient
         */
        T operator()(size_t pRow, size_t pCol) const;

        /*!
         * \brief Addition operator. Do not modify this object.
         * @param pVector is the vector to add to this one
         * @return a vector corresponding to this vector plus pVector
         */
        Matrix<T, ROW_TYPE, COL_TYPE, ROW, COL> operator+(const Matrix<T, ROW_TYPE, COL_TYPE, ROW, COL> & pMatrix) const;

        /*!
         * \brief Substraction operator. Do not modify this object.
         * @param pVector is the vector to substract from this one
         * @return a vector corresponding to this vector minus pVector
         */
        Matrix<T, ROW_TYPE, COL_TYPE, ROW, COL> operator-(const Matrix<T, ROW_TYPE, COL_TYPE, ROW, COL> & pMatrix) const;

        /*!
         * \brief Multiplication by a matrix operator. Do not modify this object.
         * @param pMatrix is the right hand side of the multiplication
         * @return a matrix corresponding to the multiplication of this * pMatrix
         */
        Matrix<T, ROW_TYPE, COL_TYPE, ROW, COL> operator*(const Matrix<T, ROW_TYPE, COL_TYPE, ROW, COL> & pMatrix) const;

        /*!
         * \brief Multiplication by a vector operator. Do not modify this object.
         * @param pVector is the right hand side of the multiplication
         * @return a vector corresponding to the multiplication of this * pVector
         */
        template<typename SIZE_TYPE>
        Vector<T, SIZE_TYPE, COL> operator*(const Vector<T, SIZE_TYPE, COL> & pVector) const
        {
            Vector<T, SIZE_TYPE, COL> lRes;

            for (size_t j = 0; j < COL; ++j)
                for (size_t i = 0; i < ROW; ++i)
                    lRes[j] += mCoeff[j][i]*pVector[i];

            return lRes;
        }

        /*!
         * \brief Multiplication by a scalar operator
         * @param pScalar is the scalar value that will multiply all the coefficients
         * @return a new matrix after the operation, does not modify this object
         */
        Matrix<T, ROW_TYPE, COL_TYPE, ROW, COL> operator*(T pScalar) const;

        /*!
         * \brief Division by a scalar operator
         * @param pScalar is the scalar value from which all the coefficients will be divided
         * @return a new matrix after the operation, does not modify this object
         */
        Matrix<T, ROW_TYPE, COL_TYPE, ROW, COL> operator/(T pScalar) const;

        /*!
         * \brief Multiplication by a scalar operator
         * @param pScalar is the scalar value that will multiply all the coefficients
         * @return a reference on this object after the operation
         */
        Matrix<T, ROW_TYPE, COL_TYPE, ROW, COL> & operator*=(T pScalar);

        /*!
         * \brief Division by a scalar operator
         * @param pScalar is the scalar value from which all the coefficients will be divided
         * @return a reference on this object after the operation
         */
        Matrix<T, ROW_TYPE, COL_TYPE, ROW, COL> & operator/=(T pScalar);

        /*!
         * \brief Transpose this matrix
         * @return a reference on this object after the operation
         */
        Matrix<T, ROW_TYPE, COL_TYPE, ROW, COL> & transpose(void);

        /*!
         * \brief Get a pointer on the first element of the array storing the coefficients (read only)
         * @return a pointer to access the coefficients of the matrix
         */
        T* data(void);

        /*!
         * \brief Compute the determinant of this matrix ONLY IF IT IS A 4x4 matrix
         * @return the determinant, a scalar value
         */
        template<typename INTERNAL_ROW = ROW_TYPE, typename INTERNAL_COL = COL_TYPE>
        typename std::enable_if<std::is_same<INTERNAL_ROW, miniGL::FOUR>::value && std::is_same<INTERNAL_COL, miniGL::FOUR>::value && !std::is_same<T, double>::value, double>::type determinant(void) const
        {
            // To get correct results, the determinant has to be computed using doubles! This version is for the case where T is not double: we have to make a copy of the values as double to perform the computation
            double lTmp[4][4];

            for (unsigned int i = 0; i < 4; ++i)
                for (unsigned int j = 0; j < 4; ++j)
                        lTmp[i][j] = static_cast<double>(mCoeff[i][j]);

            return  ( lTmp[0][3]*lTmp[1][2]*lTmp[2][1]*lTmp[3][0] - lTmp[0][2]*lTmp[1][3]*lTmp[2][1]*lTmp[3][0] - lTmp[0][3]*lTmp[1][1]*lTmp[2][2]*lTmp[3][0] + lTmp[0][1]*lTmp[1][3]*lTmp[2][2]*lTmp[3][0]
                    + lTmp[0][2]*lTmp[1][1]*lTmp[2][3]*lTmp[3][0] - lTmp[0][1]*lTmp[1][2]*lTmp[2][3]*lTmp[3][0] - lTmp[0][3]*lTmp[1][2]*lTmp[2][0]*lTmp[3][1] + lTmp[0][2]*lTmp[1][3]*lTmp[2][0]*lTmp[3][1]
                    + lTmp[0][3]*lTmp[1][0]*lTmp[2][2]*lTmp[3][1] - lTmp[0][0]*lTmp[1][3]*lTmp[2][2]*lTmp[3][1] - lTmp[0][2]*lTmp[1][0]*lTmp[2][3]*lTmp[3][1] + lTmp[0][0]*lTmp[1][2]*lTmp[2][3]*lTmp[3][1]
                    + lTmp[0][3]*lTmp[1][1]*lTmp[2][0]*lTmp[3][2] - lTmp[0][1]*lTmp[1][3]*lTmp[2][0]*lTmp[3][2] - lTmp[0][3]*lTmp[1][0]*lTmp[2][1]*lTmp[3][2] + lTmp[0][0]*lTmp[1][3]*lTmp[2][1]*lTmp[3][2]
                    + lTmp[0][1]*lTmp[1][0]*lTmp[2][3]*lTmp[3][2] - lTmp[0][0]*lTmp[1][1]*lTmp[2][3]*lTmp[3][2] - lTmp[0][2]*lTmp[1][1]*lTmp[2][0]*lTmp[3][3] + lTmp[0][1]*lTmp[1][2]*lTmp[2][0]*lTmp[3][3]
                    + lTmp[0][2]*lTmp[1][0]*lTmp[2][1]*lTmp[3][3] - lTmp[0][0]*lTmp[1][2]*lTmp[2][1]*lTmp[3][3] - lTmp[0][1]*lTmp[1][0]*lTmp[2][2]*lTmp[3][3] + lTmp[0][0]*lTmp[1][1]*lTmp[2][2]*lTmp[3][3]);
        }

        /*!
         * \brief Compute the determinant of this matrix ONLY IF IT IS A 4x4 matrix
         * @return the determinant, a scalar value
         */
        template<typename INTERNAL_ROW = ROW_TYPE, typename INTERNAL_COL = COL_TYPE>
        typename std::enable_if<std::is_same<INTERNAL_ROW, miniGL::FOUR>::value && std::is_same<INTERNAL_COL, miniGL::FOUR>::value && std::is_same<T, double>::value, T>::type determinant(void) const
        {
            // To get correct results, the determinant has to be computed using doubles! This version of the method is specific for the case where T = double
            return  ( mCoeff[0][3]*mCoeff[1][2]*mCoeff[2][1]*mCoeff[3][0] - mCoeff[0][2]*mCoeff[1][3]*mCoeff[2][1]*mCoeff[3][0] - mCoeff[0][3]*mCoeff[1][1]*mCoeff[2][2]*mCoeff[3][0] + mCoeff[0][1]*mCoeff[1][3]*mCoeff[2][2]*mCoeff[3][0]
                    + mCoeff[0][2]*mCoeff[1][1]*mCoeff[2][3]*mCoeff[3][0] - mCoeff[0][1]*mCoeff[1][2]*mCoeff[2][3]*mCoeff[3][0] - mCoeff[0][3]*mCoeff[1][2]*mCoeff[2][0]*mCoeff[3][1] + mCoeff[0][2]*mCoeff[1][3]*mCoeff[2][0]*mCoeff[3][1]
                    + mCoeff[0][3]*mCoeff[1][0]*mCoeff[2][2]*mCoeff[3][1] - mCoeff[0][0]*mCoeff[1][3]*mCoeff[2][2]*mCoeff[3][1] - mCoeff[0][2]*mCoeff[1][0]*mCoeff[2][3]*mCoeff[3][1] + mCoeff[0][0]*mCoeff[1][2]*mCoeff[2][3]*mCoeff[3][1]
                    + mCoeff[0][3]*mCoeff[1][1]*mCoeff[2][0]*mCoeff[3][2] - mCoeff[0][1]*mCoeff[1][3]*mCoeff[2][0]*mCoeff[3][2] - mCoeff[0][3]*mCoeff[1][0]*mCoeff[2][1]*mCoeff[3][2] + mCoeff[0][0]*mCoeff[1][3]*mCoeff[2][1]*mCoeff[3][2]
                    + mCoeff[0][1]*mCoeff[1][0]*mCoeff[2][3]*mCoeff[3][2] - mCoeff[0][0]*mCoeff[1][1]*mCoeff[2][3]*mCoeff[3][2] - mCoeff[0][2]*mCoeff[1][1]*mCoeff[2][0]*mCoeff[3][3] + mCoeff[0][1]*mCoeff[1][2]*mCoeff[2][0]*mCoeff[3][3]
                    + mCoeff[0][2]*mCoeff[1][0]*mCoeff[2][1]*mCoeff[3][3] - mCoeff[0][0]*mCoeff[1][2]*mCoeff[2][1]*mCoeff[3][3] - mCoeff[0][1]*mCoeff[1][0]*mCoeff[2][2]*mCoeff[3][3] + mCoeff[0][0]*mCoeff[1][1]*mCoeff[2][2]*mCoeff[3][3]);
        }

        /*!
         * \brief Inverse this matrix ONLY IF IT IS A 4x4 matrix
         */
        template<typename INTERNAL_ROW = ROW_TYPE, typename INTERNAL_COL = COL_TYPE>
        typename std::enable_if<std::is_same<INTERNAL_ROW, miniGL::FOUR>::value && std::is_same<INTERNAL_COL, miniGL::FOUR>::value, void>::type inverse(void)
        {
            double lDeterminant = determinant();

            if (lDeterminant != 0.0)
                *this = _computeInverse(lDeterminant);
        }

        /*!
         * \brief Compute the inverse of this matrix (this matrix is not modified) ONLY IF IT IS A 4x4 matrix
         * @return the inverse matrix if it was possible to compute it, the null matrix otherwise
         */
        template<typename INTERNAL_ROW = ROW_TYPE, typename INTERNAL_COL = COL_TYPE>
        typename std::enable_if<std::is_same<INTERNAL_ROW, miniGL::FOUR>::value && std::is_same<INTERNAL_COL, miniGL::FOUR>::value, Matrix<T, miniGL::FOUR, miniGL::FOUR, 4, 4>>::type inversed(void) const
        {
            double lDeterminant = determinant();

            if (lDeterminant == 0.0)
                return Matrix<T, miniGL::FOUR, miniGL::FOUR, 4, 4>();
            else
                return _computeInverse(lDeterminant);
        }

        /*!
         * \brief Display the coefficients of a matrix using cout
         * @param pWidth is the number of characters used to display each coefficient (for alignment purposes on screen)
         */
        void display(bool pBlancLine = true, unsigned int pWidth = 5) const;

    private:
        /*!
         * \brief Helper method that actually does the computation of the inverse matrix ONLY IF IT IS A 4x4 matrix
         * @param pDeterminant is the determinant of this matrix. pDeterminant MUST BE different from 0
         */
        template<typename INTERNAL_ROW = ROW_TYPE, typename INTERNAL_COL = COL_TYPE>
        typename std::enable_if<std::is_same<INTERNAL_ROW,miniGL::FOUR>::value && std::is_same<INTERNAL_COL,miniGL::FOUR>::value, Matrix<T, miniGL::FOUR, miniGL::FOUR, 4, 4>>::type _computeInverse(double pDeterminant) const
        {
            double lInvDet = 1.0 / pDeterminant;

            Matrix<T, miniGL::FOUR, miniGL::FOUR, 4, 4> lRes;

            lRes.mCoeff[0][0] = lInvDet * (-mCoeff[1][3] * mCoeff[2][2] * mCoeff[3][1]  +  mCoeff[1][2] * mCoeff[2][3] * mCoeff[3][1]  +  mCoeff[1][3] * mCoeff[2][1] * mCoeff[3][2]  -  mCoeff[1][1] * mCoeff[2][3] * mCoeff[3][2]  -  mCoeff[1][2] * mCoeff[2][1] * mCoeff[3][3]  +  mCoeff[1][1] * mCoeff[2][2] * mCoeff[3][3]);
            lRes.mCoeff[1][0] = lInvDet * ( mCoeff[1][3] * mCoeff[2][2] * mCoeff[3][0]  -  mCoeff[1][2] * mCoeff[2][3] * mCoeff[3][0]  -  mCoeff[1][3] * mCoeff[2][0] * mCoeff[3][2]  +  mCoeff[1][0] * mCoeff[2][3] * mCoeff[3][2]  +  mCoeff[1][2] * mCoeff[2][0] * mCoeff[3][3]  -  mCoeff[1][0] * mCoeff[2][2] * mCoeff[3][3]);
            lRes.mCoeff[2][0] = lInvDet * (-mCoeff[1][3] * mCoeff[2][1] * mCoeff[3][0]  +  mCoeff[1][1] * mCoeff[2][3] * mCoeff[3][0]  +  mCoeff[1][3] * mCoeff[2][0] * mCoeff[3][1]  -  mCoeff[1][0] * mCoeff[2][3] * mCoeff[3][1]  -  mCoeff[1][1] * mCoeff[2][0] * mCoeff[3][3]  +  mCoeff[1][0] * mCoeff[2][1] * mCoeff[3][3]);
            lRes.mCoeff[3][0] = lInvDet * ( mCoeff[1][2] * mCoeff[2][1] * mCoeff[3][0]  -  mCoeff[1][1] * mCoeff[2][2] * mCoeff[3][0]  -  mCoeff[1][2] * mCoeff[2][0] * mCoeff[3][1]  +  mCoeff[1][0] * mCoeff[2][2] * mCoeff[3][1]  +  mCoeff[1][1] * mCoeff[2][0] * mCoeff[3][2]  -  mCoeff[1][0] * mCoeff[2][1] * mCoeff[3][2]);

            lRes.mCoeff[0][1] = lInvDet * ( mCoeff[0][3] * mCoeff[2][2] * mCoeff[3][1]  -  mCoeff[0][2] * mCoeff[2][3] * mCoeff[3][1]  -  mCoeff[0][3] * mCoeff[2][1] * mCoeff[3][2]  +  mCoeff[0][1] * mCoeff[2][3] * mCoeff[3][2]  +  mCoeff[0][2] * mCoeff[2][1] * mCoeff[3][3]  -  mCoeff[0][1] * mCoeff[2][2] * mCoeff[3][3]);
            lRes.mCoeff[1][1] = lInvDet * (-mCoeff[0][3] * mCoeff[2][2] * mCoeff[3][0]  +  mCoeff[0][2] * mCoeff[2][3] * mCoeff[3][0]  +  mCoeff[0][3] * mCoeff[2][0] * mCoeff[3][2]  -  mCoeff[0][0] * mCoeff[2][3] * mCoeff[3][2]  -  mCoeff[0][2] * mCoeff[2][0] * mCoeff[3][3]  +  mCoeff[0][0] * mCoeff[2][2] * mCoeff[3][3]);
            lRes.mCoeff[2][1] = lInvDet * ( mCoeff[0][3] * mCoeff[2][1] * mCoeff[3][0]  -  mCoeff[0][1] * mCoeff[2][3] * mCoeff[3][0]  -  mCoeff[0][3] * mCoeff[2][0] * mCoeff[3][1]  +  mCoeff[0][0] * mCoeff[2][3] * mCoeff[3][1]  +  mCoeff[0][1] * mCoeff[2][0] * mCoeff[3][3]  -  mCoeff[0][0] * mCoeff[2][1] * mCoeff[3][3]);
            lRes.mCoeff[3][1] = lInvDet * (-mCoeff[0][2] * mCoeff[2][1] * mCoeff[3][0]  +  mCoeff[0][1] * mCoeff[2][2] * mCoeff[3][0]  +  mCoeff[0][2] * mCoeff[2][0] * mCoeff[3][1]  -  mCoeff[0][0] * mCoeff[2][2] * mCoeff[3][1]  -  mCoeff[0][1] * mCoeff[2][0] * mCoeff[3][2]  +  mCoeff[0][0] * mCoeff[2][1] * mCoeff[3][2]);

            lRes.mCoeff[0][2] = lInvDet * (-mCoeff[0][3] * mCoeff[1][2] * mCoeff[3][1]  +  mCoeff[0][2] * mCoeff[1][3] * mCoeff[3][1]  +  mCoeff[0][3] * mCoeff[1][1] * mCoeff[3][2]  -  mCoeff[0][1] * mCoeff[1][3] * mCoeff[3][2]  -  mCoeff[0][2] * mCoeff[1][1] * mCoeff[3][3]  +  mCoeff[0][1] * mCoeff[1][2] * mCoeff[3][3]);
            lRes.mCoeff[1][2] = lInvDet * ( mCoeff[0][3] * mCoeff[1][2] * mCoeff[3][0]  -  mCoeff[0][2] * mCoeff[1][3] * mCoeff[3][0]  -  mCoeff[0][3] * mCoeff[1][0] * mCoeff[3][2]  +  mCoeff[0][0] * mCoeff[1][3] * mCoeff[3][2]  +  mCoeff[0][2] * mCoeff[1][0] * mCoeff[3][3]  -  mCoeff[0][0] * mCoeff[1][2] * mCoeff[3][3]);
            lRes.mCoeff[2][2] = lInvDet * (-mCoeff[0][3] * mCoeff[1][1] * mCoeff[3][0]  +  mCoeff[0][1] * mCoeff[1][3] * mCoeff[3][0]  +  mCoeff[0][3] * mCoeff[1][0] * mCoeff[3][1]  -  mCoeff[0][0] * mCoeff[1][3] * mCoeff[3][1]  -  mCoeff[0][1] * mCoeff[1][0] * mCoeff[3][3]  +  mCoeff[0][0] * mCoeff[1][1] * mCoeff[3][3]);
            lRes.mCoeff[3][2] = lInvDet * ( mCoeff[0][2] * mCoeff[1][1] * mCoeff[3][0]  -  mCoeff[0][1] * mCoeff[1][2] * mCoeff[3][0]  -  mCoeff[0][2] * mCoeff[1][0] * mCoeff[3][1]  +  mCoeff[0][0] * mCoeff[1][2] * mCoeff[3][1]  +  mCoeff[0][1] * mCoeff[1][0] * mCoeff[3][2]  -  mCoeff[0][0] * mCoeff[1][1] * mCoeff[3][2]);

            lRes.mCoeff[0][3] = lInvDet * ( mCoeff[0][3] * mCoeff[1][2] * mCoeff[2][1]  -  mCoeff[0][2] * mCoeff[1][3] * mCoeff[2][1]  -  mCoeff[0][3] * mCoeff[1][1] * mCoeff[2][2]  +  mCoeff[0][1] * mCoeff[1][3] * mCoeff[2][2]  +  mCoeff[0][2] * mCoeff[1][1] * mCoeff[2][3]  -  mCoeff[0][1] * mCoeff[1][2] * mCoeff[2][3]);
            lRes.mCoeff[1][3] = lInvDet * (-mCoeff[0][3] * mCoeff[1][2] * mCoeff[2][0]  +  mCoeff[0][2] * mCoeff[1][3] * mCoeff[2][0]  +  mCoeff[0][3] * mCoeff[1][0] * mCoeff[2][2]  -  mCoeff[0][0] * mCoeff[1][3] * mCoeff[2][2]  -  mCoeff[0][2] * mCoeff[1][0] * mCoeff[2][3]  +  mCoeff[0][0] * mCoeff[1][2] * mCoeff[2][3]);
            lRes.mCoeff[2][3] = lInvDet * ( mCoeff[0][3] * mCoeff[1][1] * mCoeff[2][0]  -  mCoeff[0][1] * mCoeff[1][3] * mCoeff[2][0]  -  mCoeff[0][3] * mCoeff[1][0] * mCoeff[2][1]  +  mCoeff[0][0] * mCoeff[1][3] * mCoeff[2][1]  +  mCoeff[0][1] * mCoeff[1][0] * mCoeff[2][3]  -  mCoeff[0][0] * mCoeff[1][1] * mCoeff[2][3]);
            lRes.mCoeff[3][3] = lInvDet * (-mCoeff[0][2] * mCoeff[1][1] * mCoeff[2][0]  +  mCoeff[0][1] * mCoeff[1][2] * mCoeff[2][0]  +  mCoeff[0][2] * mCoeff[1][0] * mCoeff[2][1]  -  mCoeff[0][0] * mCoeff[1][2] * mCoeff[2][1]  -  mCoeff[0][1] * mCoeff[1][0] * mCoeff[2][2]  +  mCoeff[0][0] * mCoeff[1][1] * mCoeff[2][2]);

            return lRes;
        }

    private:
        T mCoeff[ROW][COL];

    }; // class Matrix

    template<typename T, typename ROW_TYPE, typename COL_TYPE, unsigned int ROW, unsigned int COL>
    Matrix<T, ROW_TYPE, COL_TYPE, ROW, COL>::Matrix(void)
    {
        for (size_t i = 0; i < ROW; ++i)
            for (size_t j = 0; j < COL; ++j)
                mCoeff[i][j] = 0;
    }

    template<typename T, typename ROW_TYPE, typename COL_TYPE, unsigned int ROW, unsigned int COL>
    Matrix<T, ROW_TYPE, COL_TYPE, ROW, COL>::Matrix(T pScalar)
    {
        for (size_t i = 0; i < ROW; ++i)
            for (size_t j = 0; j < COL; ++j)
                mCoeff[i][j] = (i == j)?pScalar:0;
    }

    template<typename T, typename ROW_TYPE, typename COL_TYPE, unsigned int ROW, unsigned int COL>
    Matrix<T, ROW_TYPE, COL_TYPE, ROW, COL>::Matrix(const Matrix<T, ROW_TYPE, COL_TYPE, ROW, COL> & pMatrix)
    {
        for (size_t i = 0; i < ROW; ++i)
            for (size_t j = 0; j < COL; ++j)
                mCoeff[i][j] = pMatrix.mCoeff[i][j];
    }

    template<typename T, typename ROW_TYPE, typename COL_TYPE, unsigned int ROW, unsigned int COL>
    Matrix<T, ROW_TYPE, COL_TYPE, ROW, COL>::~Matrix(void)
    {
    }

    template<typename T, typename ROW_TYPE, typename COL_TYPE, unsigned int ROW, unsigned int COL>
    Matrix<T, ROW_TYPE, COL_TYPE, ROW, COL> & Matrix<T, ROW_TYPE, COL_TYPE, ROW, COL>::operator=(const Matrix<T, ROW_TYPE, COL_TYPE, ROW, COL> & pMatrix)
    {
        if (this != & pMatrix)
        {
            for (size_t i = 0; i < ROW; ++i)
                for (size_t j = 0; j < COL; ++j)
                    mCoeff[i][j] = pMatrix.mCoeff[i][j];
        }

        return *this;
    }

    template<typename T, typename ROW_TYPE, typename COL_TYPE, unsigned int ROW, unsigned int COL>
    bool Matrix<T, ROW_TYPE, COL_TYPE, ROW, COL>::operator==(const Matrix<T, ROW_TYPE, COL_TYPE, ROW, COL> & pMatrix)
    {
        bool lRes = true;

        for (size_t i = 0; i < ROW; ++i)
            for (size_t j = 0; j < COL; ++j)
                lRes &= (mCoeff[i][j] == pMatrix.mCoeff[i][j]);

        return lRes;
    }

    template<typename T, typename ROW_TYPE, typename COL_TYPE, unsigned int ROW, unsigned int COL>
    T & Matrix<T, ROW_TYPE, COL_TYPE, ROW, COL>::operator()(size_t pRow, size_t pCol)
    {
        return mCoeff[pRow][pCol];
    }

    template<typename T, typename ROW_TYPE, typename COL_TYPE, unsigned int ROW, unsigned int COL>
    T Matrix<T, ROW_TYPE, COL_TYPE, ROW, COL>::operator()(size_t pRow, size_t pCol) const
    {
        return mCoeff[pRow][pCol];
    }

    template<typename T, typename ROW_TYPE, typename COL_TYPE, unsigned int ROW, unsigned int COL>
    Matrix<T, ROW_TYPE, COL_TYPE, ROW, COL> Matrix<T, ROW_TYPE, COL_TYPE, ROW, COL>::operator+(const Matrix<T, ROW_TYPE, COL_TYPE, ROW, COL> & pMatrix) const
    {
        Matrix<T, ROW_TYPE, COL_TYPE, ROW, COL> lRes;

        for (size_t i = 0; i < ROW; ++i)
            for (size_t j = 0; j < COL; ++j)
                lRes.mCoeff[i][j] = mCoeff[i][j] + pMatrix.mCoeff[i][j];

        return lRes;
    }

    template<typename T, typename ROW_TYPE, typename COL_TYPE, unsigned int ROW, unsigned int COL>
    Matrix<T, ROW_TYPE, COL_TYPE, ROW, COL> Matrix<T, ROW_TYPE, COL_TYPE, ROW, COL>::operator-(const Matrix<T, ROW_TYPE, COL_TYPE, ROW, COL> & pMatrix) const
    {
        Matrix<T, ROW_TYPE, COL_TYPE, ROW, COL> lRes;

        for (size_t i = 0; i < ROW; ++i)
            for (size_t j = 0; j < COL; ++j)
                lRes.mCoeff[i][j] = mCoeff[i][j] - pMatrix.mCoeff[i][j];

        return lRes;
    }

    template<typename T, typename ROW_TYPE, typename COL_TYPE, unsigned int ROW, unsigned int COL>
    Matrix<T, ROW_TYPE, COL_TYPE, ROW, COL> Matrix<T, ROW_TYPE, COL_TYPE, ROW, COL>::operator*(const Matrix<T, ROW_TYPE, COL_TYPE, ROW, COL> & pMatrix) const
    {
        Matrix<T, ROW_TYPE, COL_TYPE, ROW, COL> lRes;

        for (size_t i = 0; i < ROW; ++i)
            for (size_t j = 0; j < COL; ++j)
                for (size_t k = 0; k < COL; ++k)
                    lRes.mCoeff[i][j] += mCoeff[i][k] * pMatrix.mCoeff[k][j];

        return lRes;
    }

    template<typename T, typename ROW_TYPE, typename COL_TYPE, unsigned int ROW, unsigned int COL>
    Matrix<T, ROW_TYPE, COL_TYPE, ROW, COL> Matrix<T, ROW_TYPE, COL_TYPE, ROW, COL>::operator*(T pScalar) const
    {
        Matrix<T, ROW_TYPE, COL_TYPE, ROW, COL> lRes;

        for (size_t i = 0; i < ROW; ++i)
            for (size_t j = 0; j < COL; ++j)
                lRes.mCoeff[i][j] = mCoeff[i][j]*pScalar;

        return lRes;
    }

    template<typename T, typename ROW_TYPE, typename COL_TYPE, unsigned int ROW, unsigned int COL>
    Matrix<T, ROW_TYPE, COL_TYPE, ROW, COL> Matrix<T, ROW_TYPE, COL_TYPE, ROW, COL>::operator/(T pScalar) const
    {
        assert(pScalar != 0);

        Matrix<T, ROW_TYPE, COL_TYPE, ROW, COL> lRes;

        // We do not precompute the inverse of pScalar because it seems to cause precision problems (see unit tests)
        for (size_t i = 0; i < ROW; ++i)
            for (size_t j = 0; j < COL; ++j)
                lRes.mCoeff[i][j] = mCoeff[i][j] / pScalar;

        return lRes;
    }

    template<typename T, typename ROW_TYPE, typename COL_TYPE, unsigned int ROW, unsigned int COL>
    Matrix<T, ROW_TYPE, COL_TYPE, ROW, COL> & Matrix<T, ROW_TYPE, COL_TYPE, ROW, COL>::operator*=(T pScalar)
    {
        for (size_t i = 0; i < ROW; ++i)
            for (size_t j = 0; j < COL; ++j)
                mCoeff[i][j] *= pScalar;

        return *this;
    }

    template<typename T, typename ROW_TYPE, typename COL_TYPE, unsigned int ROW, unsigned int COL>
    Matrix<T, ROW_TYPE, COL_TYPE, ROW, COL> & Matrix<T, ROW_TYPE, COL_TYPE, ROW, COL>::operator/=(T pScalar)
    {
        assert(pScalar != 0);

        // We do not precompute the inverse of pScalar because it seems to cause precision problems (see unit tests)
        for (size_t i = 0; i < ROW; ++i)
            for (size_t j = 0; j < COL; ++j)
                mCoeff[i][j] /= pScalar;

        return *this;
    }

    template<typename T, typename ROW_TYPE, typename COL_TYPE, unsigned int ROW, unsigned int COL>
    Matrix<T, ROW_TYPE, COL_TYPE, ROW, COL> & Matrix<T, ROW_TYPE, COL_TYPE, ROW, COL>::transpose(void)
    {
        assert(ROW == COL && "transpose implemented only for square matrices");

        if (ROW == 1)
            return *this;

        for (size_t i = 0; i < ROW; ++i)
        {
            for (size_t j = i; j < COL; ++j)
            {
                if(i != j)
                    std::swap(mCoeff[i][j], mCoeff[j][i]);
            }
        }

        return *this;
    }

    template<typename T, typename ROW_TYPE, typename COL_TYPE, unsigned int ROW, unsigned int COL>
    T* Matrix<T, ROW_TYPE, COL_TYPE, ROW, COL>::data(void)
    {
        return & mCoeff[0][0];
    }

    template<typename T, typename ROW_TYPE, typename COL_TYPE, unsigned int ROW, unsigned int COL>
    void Matrix<T, ROW_TYPE, COL_TYPE, ROW, COL>::display(bool pBlancLine, unsigned int pWidth) const
    {
        for (size_t i = 0; i < ROW; ++i)
        {
            for (size_t j = 0; j < COL; ++j)
                std::cout << std::setw(pWidth) << mCoeff[i][j] << " ";

            std::cout << std::endl;
        }

        if (pBlancLine)
            std::cout << std::endl;
    }

} // namespace miniGL
