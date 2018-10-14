//===============================================================================================//
/*!
 *  \file      BackendGLUT.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

#include "BaseBackend.hpp"

namespace miniGL
{
    /*!
     *  \brief Use GLUT as window backend
     *  \details TBD
     */
    class BackendGLUT : public BaseBackend
    {
    public:
        /*!
         *  \brief Constructor with callbacks parameter
         *  @param pCallbacks is a pointer on the callbacks used by the application associated to this window
         */
        BackendGLUT(CallbacksInterface* pCallbacks);

        /*!
         *  \brief Copy contructor, not accessible
         *  @param pWindow is the object to copy parameters from
         */
        BackendGLUT(const BackendGLUT & pBackendGLUT) = delete;

        /*!
         *  \brief Copy assignment operator, not accessible
         *  @param pWindow is the object to copy parameters from
         *  @return a reference on this
         */
        BackendGLUT & operator=(const BackendGLUT & pBackendGLUT) = delete;

        /*!
         *  \brief Destructor
         */
        ~BackendGLUT(void);

        /*!
         * \brief Implementation of a virtual method from BaseBackend
         */
        virtual void init(int pWidth, int pHeight,  bool pFullScreen, const std::string & pTitle) final;

        /*!
         * \brief Implementation of a virtual method from BaseBackend
         */
        virtual void run(void) final;

        /*!
         * \brief Implementation of a virtual method from BaseBackend
         */
        virtual void swapBuffer(void) final;

        /*!
         * \brief Implementation of a virtual method from BaseBackend
         */
        virtual void terminate(void) final;

    private:

    }; // class BackendGLUT

} // namespace miniGL
