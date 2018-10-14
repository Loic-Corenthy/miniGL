//===============================================================================================//
/*!
 *  \file      Skinning.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

#include <GL/glew.h>
#include <map>
#include <vector>
#include <string>
#include <array>

#include "Algebra.hpp"
#include "Program.hpp"
#include "LightingBase.hpp"

namespace miniGL
{
    /*!
     *  \brief This class manages the lighting in the scene (similar to the lighting class) but also handles skinning parameters
     *  \details This class handles the different transformations nodes associated to the bones linked to a vertex
     */
    class Skinning : public LightingBase<Program>
    {
    public:
        /*!
         *  \brief Default constructor
         */
        Skinning(void);

        /*!
         *  \brief Destructor
         */
        ~Skinning(void);

        /*!
         *  \brief Implementation of a virtual method from LightingBase
         */
        void init(unsigned int pPointLightCount, unsigned int pSpotLightCount, bool pActivateMotionBlur = false, const std::string & pPathVS = std::string(R"(./Shaders/Skinning.vert)"), const std::string & pPathFS = std::string(R"(./Shaders/Skinning.frag)"));

        /*!
         *  \brief Set the world view projection matrix
         *  @param pWVP is a 4x4 matrix
         */
        void WVP(const mat4f & pWVP);

        /*!
         *  \brief Set the world matrix
         *  @param pWorld is a 4x4 matrix
         */
        void worldMatrix(const mat4f & pWorld);

        /*!
         *  \brief Set the world view projection matrix from the light point of view
         *  @param pLightWVP is a 4x4 matrix
         */
        void lightWVP(const mat4f & pLightWVP);

        /*!
         *  \brief Set the position of the camera to calculate the specular component
         *  @param pPos is the position of the camera
         */
        void eyeWorldPosition(const vec3f & pPos);

        /*!
         *  \brief Set material specular intensity
         *  @param pValue is a parameter in the range [0,1]
         */
        void materialSpecularIntensity(float pValue);

        /*!
         *  \brief Set material specular power
         *  @param pValue is a parameter in the range [1,32]
         */
        void materialSpecularPower(float pValue);

        /*!
         *  \brief Activate the use of a texture to sample the final color from
         *  @param if pActivate is true, the final color is the product of the texel color and the current value of the light. Otherwise
         *         it is just the light
         */
        void useSampler(bool pActivate);

        /*!
         *  \brief Activate the computation of a shadow component to the color using the shadow map technique
         *  @param if pActivate is true, shasow is computed using the shadow map texture.
         */
        void useShadowMap(bool pActivate);

        /*!
         *  \brief Activate the interpolation of the local normal using the normal mapping method
         *  @param if pActivate is true, the normal is interpolated. Otherwise, simply use the normal provided in the mesh
         */
        void useNormalMap(bool pActivate);

        /*!
         *  \brief Set the transformation associated to a specific bone in the mesh
         *  @param pIndex is the bone index
         *  @param pTransform is a 4x4 matrix
         */
        void boneTransforms(unsigned int pIndex, const mat4f & pTransform);

        /*!
         *  \brief Set the transformation associated to a specific bone in the mesh for a previous frame
         *  @param pIndex is the bone index
         *  @param pTransform is a 4x4 matrix
         */
        void previousBoneTransforms(unsigned int pIndex, mat4f & pTransform);

    private:
        /*!
         *  \brief Implementation of a virtual method from Program
         */
        virtual bool checkUniformLocations(void) const final;

    private:
        GLuint mWVPLocation = Constants::invalidUniformLocation<GLuint>();
        GLuint mWorldLocation = Constants::invalidUniformLocation<GLuint>();
        GLuint mLightWVPLocation = Constants::invalidUniformLocation<GLuint>();
        GLuint mSamplerLocation = Constants::invalidUniformLocation<GLuint>();
        GLuint mUseSamplerLocation = Constants::invalidUniformLocation<GLuint>();
        GLuint mShadowMapLocation = Constants::invalidUniformLocation<GLuint>();
        GLuint mUseShadowMapLocation = Constants::invalidUniformLocation<GLuint>();
        GLuint mNormalMapLocation = Constants::invalidUniformLocation<GLuint>();
        GLuint mUseNormalMapLocation = Constants::invalidUniformLocation<GLuint>();
        GLuint mEyeWorldPosLocation = Constants::invalidUniformLocation<GLuint>();

        GLuint mMaterialSpecularIntensityLocation = Constants::invalidUniformLocation<GLuint>();
        GLuint mMaterialSpecularPowerLocation = Constants::invalidUniformLocation<GLuint>();

        std::vector<GLuint> mBoneLocations;
        std::vector<GLuint> mPreviousBoneLocations;

        bool mUsePreviousBones = false;

    }; // class Skinning

} // namespace miniGL




