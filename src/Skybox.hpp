//===============================================================================================//
/*!
 *  \file      Skybox.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

#include <string>
#include "Camera.hpp"
#include "MeshAOS.hpp"
#include "CubemapTexture.hpp"
#include "SkyBoxRender.hpp"

namespace miniGL
{
    /*!
     *  \brief   This class encapsulates the use of a skybox in the scene
     *  \details This class references the skybox renderer and the cube map textures
     */
    class SkyBox
    {
    public:
        /*!
         *  \brief Constructor
         *  @param pCamera is a pointer on the camera in the scene
         */
        SkyBox(std::shared_ptr<Camera> pCamera);

        /*!
         *  \brief Copy constructor not accessible.
         */
        SkyBox(const SkyBox & pSkybox) = delete;

        /*!
         *  \brief Copy operator not accessible.
         */
        SkyBox & operator=(const SkyBox & pSkybox) = delete;

        /*!
         *  \brief Destructor
         */
        ~SkyBox(void);

        /*!
         *  \brief Set the different textures to each face of the skybox
         *  @param pFilenames is a list of the 6 paths to the different textures.
         *         The textures should be ordered as follow: +X, -X, +Y, -Y, +Z, -Z.
         */
        void init(const std::initializer_list<std::string> & pFilenames);

        /*!
         *  \brief Render the skybox
         */
        void render(void);

    private:
        std::unique_ptr<SkyBoxRender> mRenderer;
        std::unique_ptr<CubemapTexture> mCubemapTexture;
        std::unique_ptr<MeshAOS> mBox;
        std::shared_ptr<Camera> mCamera;

    }; // class SkyBox

} // namespace miniGL
