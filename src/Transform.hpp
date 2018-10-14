//===============================================================================================//
/*!
 *  \file      Transform.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

#include <array>

#include "EnumClassCast.hpp"
#include "Constants.hpp"
#include "Algebra.hpp"
#include "Angle.hpp"

namespace miniGL
{
    /*!
     *  \brief   This class encapsulates transformation matrix operations
     *  \details This class allows to define translations, scaling and rotations matrices with a simple interface.
     *           It also computes a final transformation which combines all the previously mentioned transformations.
     */
    class Transform
    {
    public:
        /*!
         * \brief Default constructor
         */
        Transform(void);

        /*!
         * \brief Set the scaling matrix
         * @param pScaleMatrix is a 4x4 matrix defining the scaling transformation
         */
        void scaling(const mat4f & pScaleMatrix) noexcept;

        /*!
         * \brief Set the scaling as a vec3 (scaling factor in each direction)
         * @param pScaleX is the scaling factor in x
         * @param pScaleY is the scaling factor in y
         * @param pScaleZ is the scaling factor in z
         */
        void scaling(float pFactorX, float pFactorY, float pFactorZ);

        /*!
         * \brief Set the rotation matrix
         * @param pRotationMatrix is a 4x4 matrix defining the rotation transformation
         */
        void rotation(const mat4f & pRotationMatrix) noexcept;

        /*!
         * \brief Set the rotation as 3 rotation angles
         * @param pAngleX is the angle of the rotation around the x axis in degrees
         * @param pAngleY is the angle of the rotation around the y axis in degrees
         * @param pAngleZ is the angle of the rotation around the z axis in degrees
         */
        void rotation(degreef pAngleX, degreef pAngleY, degreef pAngleZ);

        /*!
         * \brief Set the translation matrix
         * @param pTranslationMatrix is a 4x4 matrix defining the translation transformation
         */
        void translation(const mat4f & pTranslationMatrix) noexcept;

        /*!
         * \brief Set the translation as a vec3
         * @param pTranslation is a 3D vector describing the translation
         */
        void translation(float pX, float pY, float pZ);

        /*!
         * \brief Get the scaling matrix
         * @return a 4x4 matrix corresponding to the scaling that have been previously defined (identity matrix otherwise)
         */
        mat4f scaling(void) const noexcept;

        /*!
         * \brief Get the rotation matrix
         * @return a 4x4 matrix corresponding to the rotation that have been previously defined (identity matrix otherwise)
         */
        mat4f rotation(void) const noexcept;

        /*!
         * \brief Get the translation matrix
         * @return a 4x4 matrix corresponding to the translation that have been previously defined (identity matrix otherwise)
         */
        mat4f translation(void) const noexcept;

        /*!
         * \brief Get final transformation
         * @return a 4x4 matrix corresponding to the product translation * rotation * scaling
         */
        mat4f final(void) const noexcept;

    private:
        mat4f mScaling;
        mat4f mRotation;
        mat4f mTranslation;
        mutable mat4f mFinal;
        mutable bool mUpdated = true;
        static mat4f mIdentity; /*! \todo identity matrix should be part of the mat4f class */

    }; // class Transform

} // namespace miniGL
