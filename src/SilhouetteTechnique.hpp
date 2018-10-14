//===============================================================================================//
/*!
 *  \file      SilhouetteTechnique.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

#include <map>
#include <vector>

#include "RenderingTechniqueBase.hpp"
#include "MeshAndTransform.hpp"
#include "BaseLight.hpp"
#include "SilhouetteRender.hpp"
#include "Constants.hpp"

namespace miniGL
{
    /*!
     *  \brief   This class encapsulates all the details to handle silhouette detection
     *  \details Silhouette refers to the silhouette of a 3D object that is created when light falls
     *           upon it from an arbitrary direction. Moving the light will change the silhouette accordingly.
     */
    class SilhouetteTechnique : public RenderingTechniqueBase
    {
    public:
        /*!
         *  \brief Default constructor
         */
        SilhouetteTechnique(void);

        /*!
         *  \brief Initialize the rendering technique
         */
        void init(void);

        /*!
         *  \brief Render the meshes provided as parameter using this rendering technique
         *  @param pMeshes is a container with all the meshes
         *  @param pLights is a container with all the lights
         */
        void render(const std::map<std::string, MeshAndTransform> & pMeshes, const std::vector<std::shared_ptr<BaseLight>> & pLights);

        /*!
         *  \brief Index of the light which position will be used to compute the silhouette from
         *  @param pIndex is the index of a point or spot light in the lights container
         */
        void lightIndex(unsigned int pIndex);

    private:
        std::unique_ptr<SilhouetteRender> mSilhouetteRender;
        unsigned int mLightIndex = Constants::invalidBufferIndex<unsigned int>();

    }; // class SilhouetteTechnique

} // namespace miniGL
