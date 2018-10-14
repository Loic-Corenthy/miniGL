//===============================================================================================//
/*!
 *  \file      BillboardList.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

#include <GL/glew.h>

#include <string>
#include <vector>

#include "Billboard.hpp"
#include "Texture.hpp"
#include "Constants.hpp"
#include "Algebra.hpp"
#include "MeshBase.hpp"
#include "Camera.hpp"

namespace miniGL
{
    /*!
     *  \brief   This class encapsulates the use of the Billboarding technique
     *  \details This class takes care of the different positions where the billboards should be created as
     *         well as the the texture that is displayed on each quad.
     */
    class BillboardList
    {
    public:
        /*!
         *  \brief Default constructor
         */
        BillboardList(void) = default;

        /*!
         *  \brief Copy constructor not accessible
         */
        BillboardList(const BillboardList & pBillboardList) = delete;

        /*!
         *  \brief Copy assignment operator not accessible
         */
        BillboardList operator=(const BillboardList & pBillboardList) = delete;

        /*!
         *  \brief Destructor
         */
        ~BillboardList(void);

        /*!
         *  \brief Initialize the list of billboards that will be displayed
         *  \param pTextureFilename is the complete path and filename of the texture that will be used on the billboards
         *  \param pOrigin is the point around which the billboards will be generated
         *  \param pRow is the number of billboards to create per row
         *  \param pCol is the number of billboards to create per column
         *  \param pSize is the scalar that will multiply the size of each billboard
         */
        void init(const std::string & pTextureFilename, const vec3f & pOrigin, int pRow, int pCol, float pSize);

        /*!
         *  \brief Set the size of the billboard
         *  \param pFactor scales the billboard
         */
        void size(float pFactor) noexcept;

        /*!
         *  \brief Render the billboards with their associated texture
         */
        void render(void);

        /*!
         *  \brief Set a reference to the camera
         *  \param pCamera is the main camera which parameters will be used to compute the view projection matrix
         */
        void camera(std::shared_ptr<Camera> pCamera);

    private:
        /*!
         *  \brief Compute the position of the different billboard and store them in the vertex buffer
         *  \param pOrigin is the around which the billboards will be generated
         *  \param pSize is the size of the billboard
         */
        void _createPositionBuffer(const vec3f & pOrigin, float pSize);

    private:
        Billboard mRenderer;
        std::vector<vec3f> mPositions;
        std::shared_ptr<Camera> mCamera;
        std::unique_ptr<Texture> mBillboardTexture;
        GLuint mVertexBuffer = Constants::invalidBufferIndex<GLuint>();
        GLuint mVAO = 0;
        int mRow = 1;
        int mCol = 1;

    }; // class BillboardList

    inline void BillboardList::size(float pFactor) noexcept
    {
        mRenderer.size(pFactor);
    }

} // namespace miniGL
