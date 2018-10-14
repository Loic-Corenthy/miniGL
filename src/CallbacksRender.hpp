//===============================================================================================//
/*!
 *  \file      CallbacksRender.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

namespace miniGL
{
    /*!
     *  \brief Other classes can inherit from CallbacksRender to be able to add functionalities before each draw call
     *  \details This class is an abstract class defining callbacks that can be used before rendering a mesh.
     */
    class CallbacksRender
    {
    public:
        /*!
         *  \brief Destructor
         */
        virtual ~CallbacksRender(void);

        /*!
         *  \brief This method will be called before rendering a mesh
         *  \param pDrawIndex
         */
        virtual void drawStartCallback(unsigned int pDrawIndex) = 0;

    }; // class CallbacksRender

} // namespace miniGL
