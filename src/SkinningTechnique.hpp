//===============================================================================================//
/*!
 *  \file      SkinningTechnique.hpp
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
#include "Skinning.hpp"
#include "MotionBlur.hpp"
#include "IntermediateBuffer.hpp"

namespace miniGL
{
    /*!
     *  \brief  This class encapsulates all the details to handle skeletal animation on a mesh
     *  \details It is possible to simply animate the skeleton or to apply a motion blur on top of the skeletal animation
     */
    class SkinningTechnique : public RenderingTechniqueBase
    {
    public:
        /*!
         *  \brief Default constructor
         */
        SkinningTechnique(void);

        /*!
         *  \brief Initialize the rendering technique
         *  @param pPointLightCount is the number of point lights to render
         *  @param pSpotLightCount is the number of spot lights to render
         *  @param pActivateMotionBlur loads the shader with motion blur if set to true, loads the normal skinning shader otherwise
         *  @param pWindowWidth is used for the width of the textures in the intermediate buffers
         *  @param pWindowHeight is used for the height of the textures in the intermediate buffers
         */
        void init(unsigned int pPointLightCount, unsigned int pSpotLightCount, bool pActivateMotionBlur = false, const std::tuple<unsigned int, unsigned int> & pFramebufferDimensions = std::tuple<unsigned int, unsigned int>(0,0));

        /*!
         *  \brief Render the meshes provided as parameter using this rendering technique
         *  @param pMeshes is a container with all the meshes
         *  @param pLights is a container with all the lights
         */
        void render(const std::map<std::string, MeshAndTransform> & pMeshes, const std::vector<std::shared_ptr<BaseLight>> & pLights);

        /*!
         *  \brief Set the running time, i.e. the time since the application started
         *  @param pRunningTime is the time in seconds
         */
        void runningTime(float pRunningTime);

        /*!
         *  \brief Set the indices of the lights that will be used when rendering using a specific technique
         *  @param pFirstIndex is the index of the first light to be added to the rendering technique
         */
        template<typename T, typename... Args>
        void lightToUseDuringRender(T pFirstIndex, Args... otherIndices)
        {
            mSkinning->lightToUseDuringRender(pFirstIndex);
            lightToUseDuringRender(otherIndices...);
        }

        /*!
         *  \brief Set the indices of the last (or unique) light that will be used when rendering using a specific technique
         *  @param pIndex is the index of the light to be added to the rendering technique
         */
        template<typename T>
        void lightToUseDuringRender(T pIndex)
        {
            mSkinning->lightToUseDuringRender(pIndex);
        }

    private:
        std::unique_ptr<Skinning> mSkinning;
        std::unique_ptr<MotionBlur> mMotionBlur;
        IntermediateBuffer mIntermediateBuffer;
        std::vector<mat4f> mPreviousBoneTransforms;
        MeshAndTransform mQuad;
        float mRunningTime = 0.0f;
        bool mActivateMotionBlur = false;
        bool mInitializePreviousTransforms = true;

    }; // class SkinningTechnique

} // namespace miniGL
