//===============================================================================================//
/*!
 *  \file      EnumClassCast.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

namespace miniGL
{
    /*!
     *  \brief This function is used to cast enum class variables to their underlying type in order
     *         to be able to use them where a basic type is requiered (in arrays for example)
     */
    template<typename E>
    constexpr typename std::underlying_type<E>::type toUT(E pEnum) noexcept
    {
        return static_cast< typename std::underlying_type<E>::type>(pEnum);
    }
}

using miniGL::toUT;
