//===============================================================================================//
/*!
 *  \file      ShadowMapDirectionalLightLighting.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

#include <GL/glew.h>
#include <string>

#include "Algebra.hpp"
#include "Program.hpp"
#include "DirectionalLight.hpp"
#include "Constants.hpp"

namespace miniGL
{
    /*!
     *  \brief   This class manages the shaders allowing shadow mapping for a directionas light
     *  \details There is only 1 directional light
     */
    class ShadowMapDirectionalLightLighting : public Program
    {
    public:
        /*!
         *  \brief Load shaders and get uniform IDS.
         */
        void init(const std::string & pPathVS = std::string("./Shaders/ShadowMapDirectionalLightLighting.vert"),
                  const std::string & pPathFS = std::string("./Shaders/ShadowMapDirectionalLightLighting.frag"));

        /*!
         *  \brief Update the lighting state in the GPU
         *  @param pLight is a pointer to a directional light
         */
        void updateDirectionalLightState(const std::shared_ptr<DirectionalLight> pLight);

        /*!
         *  \brief Set the world view projection matrix
         *  @param pWVP is a 4x4 matrix
         */
        void WVP(const mat4f & pWVP);

        /*!
         *  \brief Set the world view projection matrix from the light point of view
         *  @param pLightWVP is a 4x4 matrix
         */
        void lightWVP(const mat4f & pLightWVP);

        /*!
         *  \brief Set the world matrix
         *  @param pWorld is a 4x4 matrix
         */
        void world(const mat4f & pWorld);

        /*!
         *  \brief Set the point of view in the scene
         *  @param pPos is the position of the camera
         */
        void eyeWorldPosition(const vec3f & pPos);

        /*!
         *  \brief Set the index of the unit hodling the color texture
         *  @param pTexUnit is the texture unit index
         */
        void colorTextureUnit(unsigned int pTextureUnit);

        /*!
         *  \brief Activate the use of a texture to sample the final color from
         *  @param if pActivate is true, the final color is the product of the texel color
         *         and the current value of the light. Otherwise it is just the light
         */
        void useColorTexture(bool pActivate);

        /*!
         *  \brief Set the index of the unit hodling the shadow texture
         *  @param pTexUnit is the texture unit index
         */
        void shadowTextureUnit(unsigned int pTextureUnit);

        /*!
         *  \brief Activate the interpolation of the local normal using the normal mapping method
         *  @param if pActivate is true, the normal is interpolated. Otherwise, simply use the normal provided in the mesh
         */
        void useNormalMap(bool pActivate);

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

    private:
        /*!
         *  \brief Implementation of a virtual method from Program
         */
        virtual bool checkUniformLocations(void) const final;

    private:
        GLuint mDirectionalLightColorLocation = Constants::invalidUniformLocation<GLuint>();
        GLuint mDirectionalLightAmbientLocation = Constants::invalidUniformLocation<GLuint>();
        GLuint mDirectionalLightDirectionLocation = Constants::invalidUniformLocation<GLuint>();
        GLuint mDirectionalLightDiffuseLocation = Constants::invalidUniformLocation<GLuint>();

        GLuint mWVPLocation = Constants::invalidUniformLocation<GLuint>();
        GLuint mLightWVPLocation = Constants::invalidUniformLocation<GLuint>();
        GLuint mWorldLocation = Constants::invalidUniformLocation<GLuint>();
        GLuint mColorMapLocation = Constants::invalidUniformLocation<GLuint>();
        GLuint mUseColorMapLocation = Constants::invalidUniformLocation<GLuint>();
        GLuint mShadowMapLocation = Constants::invalidUniformLocation<GLuint>();
        GLuint mEyeWorldPosLocation = Constants::invalidUniformLocation<GLuint>();
        GLuint mMaterialSpecularIntensityLocation = Constants::invalidUniformLocation<GLuint>();
        GLuint mMaterialSpecularPowerLocation = Constants::invalidUniformLocation<GLuint>();

    }; // class ShadowMapDirectionalLightLighting

} // namespace miniGL
