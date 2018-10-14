//===============================================================================================//
/*!
 *  \file      LightingBase.hpp
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
#include "BaseLight.hpp"

#include "Shader.hpp"
#include "Constants.hpp"
#include "EnumClassCast.hpp"
#include "Exceptions.hpp"
#include "GLUtils.hpp"
#include "DirectionalLight.hpp"
#include "PointLight.hpp"
#include "SpotLight.hpp"

namespace miniGL
{
    /*!
     *  \brief   This class is the base class for lighting renderers.
     *  \details This class adds all the methods used to handle the different lights (directional, point and spot)
     */
    template<typename T>
    class LightingBase : public T
    {
    public:
        enum class DIRECTIONAL_LIGHT_PARAM : size_t
        {
            COLOR,
            DIRECTION,
            AMBIANT_INTENSITY,
            DIFFUSE_INTENSITY
        };

        enum class POINT_LIGHT_PARAM : size_t
        {
            COLOR,
            AMBIANT_INTENSITY,
            DIFFUSE_INTENSITY,
            POSITION,
            ATTENUATION_CONSTANT_COEFF,
            ATTENUATION_LINEAR_COEFF,
            ATTENUATION_EXPONENTIAL_COEFF
        };

        enum class SPOT_LIGHT_PARAM : size_t
        {
            COLOR,
            AMBIANT_INTENSITY,
            DIFFUSE_INTENSITY,
            POSITION,
            DIRECTION,
            ATTENUATION_CONSTANT_COEFF,
            ATTENUATION_LINEAR_COEFF,
            ATTENUATION_EXPONENTIAL_COEFF,
            CUTOFF_ANGLE
        };

    public:
        /*!
         *  \brief Default constructor
         */
        LightingBase(void);

        /*!
         *  \brief Destructor
         */
        virtual ~LightingBase(void);

        /*!
         *  \brief Update the lighting state in the GPU
         */
        void updateLightsState(const std::vector<std::shared_ptr<BaseLight>> & pLights);

        /*!
         *  \brief Get the uniform location IDs for all the lights parameters
         */
        void initLightParametersLocations(void);

        /*!
         *  \brief Set the number of point lights that the shaders of the class deriving from LightingBase will use
         *  @param pCount is the number of point lights. Refer to the limit specified by the shaders
         */
        void pointLights(unsigned int pCount);

        /*!
         *  \brief Set the number of spot lights that the shaders of the class deriving from LightingBase will use
         *  @param pCount is the number of spot lights. Refer to the limit specified by the shaders
         */
        void spotLights(unsigned int pCount);

        /*!
         *  \brief Set the index of any light that will be used when rendering using a specific technique
         *  @param pIndex is the index of the light in the container for all lights
         */
        void lightToUseDuringRender(unsigned int pIndex);

    protected:
        /*!
         *  \brief Implementation of a virtual method from Program
         */
        virtual bool checkUniformLocations(void) const override;

    private:
        /*!
         *  \brief Initialize the location of a uniform variable for the directional light
         *  @param pParam in the index used to store the location of the corresponding uniform parameter in a container, use an enum value.
         *  @param pName is the name of the uniform parameter in the shader
         */
        void initUniformLocation(DIRECTIONAL_LIGHT_PARAM pParameter, const char* pName);

        /*!
         *  \brief Initialize the location of a uniform variable for one of the added point lights
         *  @param pIndex corresponds to the order in which the points lights were added (first index is 0)
         *  @param pParam in the index used to store the location of the corresponding uniform parameter in a container, use an enum value.
         *  @param pName is the name of the uniform parameter in the shader
         *  @return the ID of the location
         */
        void initUniformLocation(size_t pPointLightIdx, POINT_LIGHT_PARAM pParameter, const char* pName);

        /*!
         *  \brief Initialize the location of the uniform variable for one of the added spot lights
         *  @param pIndex corresponds to the order in which the points lights were added (first index is 0)
         *  @param pParam in the index used to store the location of the corresponding uniform parameter in a container, use an enum value.
         *  @param pName is the name of the uniform parameter in the shader
         *  @return the ID of the location
         */
        void initUniformLocation(size_t pSpotLightIdx, SPOT_LIGHT_PARAM pParameter, const char* pName);

    private:
        using directionalLightParamsLoc = std::array<GLuint, 4>; // container for the directional light parameter locations
        using pointLightParamsLoc = std::array<GLuint, 7>; // container for the point lights parameter locations
        using spotLightParamsLoc = std::array<GLuint, 9>; // container for the spot lights parameter locations

        directionalLightParamsLoc mDirectionalLightLocations;
        std::vector<pointLightParamsLoc> mPointLightLocations;
        std::vector<spotLightParamsLoc> mSpotLightLocations;

        std::vector<unsigned int> mLightIndices;

        GLuint mPointLightCountLocation;
        GLuint mSpotLightCountLocation;
    }; // class LightingBase

    template<typename T>
    LightingBase<T>::LightingBase(void)
    :T()
    {
        // Only reset the uniform locations for the directional light and the total number of point/spot lights
        // since we don't know yet how many point or spot lights this rendering technique will need to support

        // Init all the location indices to invalid location
        for (auto & param : mDirectionalLightLocations)
            param = Constants::invalidUniformLocation<GLuint>();

        mPointLightCountLocation = Constants::invalidUniformLocation<GLuint>();
        mSpotLightCountLocation = Constants::invalidUniformLocation<GLuint>();
    }

    template<typename T>
    LightingBase<T>::~LightingBase(void)
    {
    }

    template<typename T>
    bool LightingBase<T>::checkUniformLocations(void) const
    {
        bool lRes = true;

        // Check the parameters of the directional light
        for(const auto param : mDirectionalLightLocations)
            lRes &= (param != Constants::invalidUniformLocation<GLuint>());

        // Check the number of point lights
        lRes &= (mPointLightCountLocation != Constants::invalidUniformLocation<GLuint>());

        // Check the parameters of each point light
        for(const auto light : mPointLightLocations)
        {
            for(auto param : light)
                lRes &= (param != Constants::invalidUniformLocation<GLuint>());
        }

        // Check the number of spot lights
        lRes &= (mSpotLightCountLocation != Constants::invalidUniformLocation<GLuint>());

        // Check the parameters of each spot light
        for(const auto light : mSpotLightLocations)
        {
            for(auto param : light)
                lRes &= (param != Constants::invalidUniformLocation<GLuint>());
        }

        return lRes;
    }

    template<typename T>
    void LightingBase<T>::initLightParametersLocations(void)
    {
        if (mPointLightLocations.size() == 0)
            std::cout << "Warning: Initializing light parameter location with no point light" << std::endl;

        if (mSpotLightLocations.size() == 0)
            std::cout << "Warning: Initializing light parameter location with no spot light" << std::endl;

        // Directional light parameters
        initUniformLocation(LightingBase::DIRECTIONAL_LIGHT_PARAM::COLOR, "uDirectionalLight.base.color");
        initUniformLocation(LightingBase::DIRECTIONAL_LIGHT_PARAM::AMBIANT_INTENSITY, "uDirectionalLight.base.ambientIntensity");
        initUniformLocation(LightingBase::DIRECTIONAL_LIGHT_PARAM::DIRECTION, "uDirectionalLight.direction");
        initUniformLocation(LightingBase::DIRECTIONAL_LIGHT_PARAM::DIFFUSE_INTENSITY, "uDirectionalLight.base.diffuseIntensity");

        // Point Lights
        mPointLightCountLocation = T::uniformLocation("uPointLightCount");

        for (size_t i = 0; i < mPointLightLocations.size(); i++)
        {
            std::string lParamName;

            std::string lPointLightName("uPointLight[");
            lPointLightName.append(std::to_string(i));
            lPointLightName.append("]");

            lParamName = lPointLightName + std::string(".base.color");
            initUniformLocation(i, LightingBase::POINT_LIGHT_PARAM::COLOR, lParamName.c_str());

            lParamName = lPointLightName + std::string(".base.ambientIntensity");
            initUniformLocation(i, LightingBase::POINT_LIGHT_PARAM::AMBIANT_INTENSITY, lParamName.c_str());

            lParamName = lPointLightName + std::string(".base.diffuseIntensity");
            initUniformLocation(i, LightingBase::POINT_LIGHT_PARAM::DIFFUSE_INTENSITY, lParamName.c_str());

            lParamName = lPointLightName + std::string(".position");
            initUniformLocation(i, LightingBase::POINT_LIGHT_PARAM::POSITION, lParamName.c_str());

            lParamName = lPointLightName + std::string(".attenuation.constant");
            initUniformLocation(i, LightingBase::POINT_LIGHT_PARAM::ATTENUATION_CONSTANT_COEFF, lParamName.c_str());

            lParamName = lPointLightName + std::string(".attenuation.linear");
            initUniformLocation(i, LightingBase::POINT_LIGHT_PARAM::ATTENUATION_LINEAR_COEFF, lParamName.c_str());

            lParamName = lPointLightName + std::string(".attenuation.exponential");
            initUniformLocation(i, LightingBase::POINT_LIGHT_PARAM::ATTENUATION_EXPONENTIAL_COEFF, lParamName.c_str());
        }

        // Spot lights
        mSpotLightCountLocation = T::uniformLocation("uSpotLightCount");

        for (size_t i = 0; i < mSpotLightLocations.size(); i++)
        {
            std::string lParamName;

            std::string lSpotLightName("uSpotLight[");
            lSpotLightName.append(std::to_string(i));
            lSpotLightName.append("]");

            lParamName = lSpotLightName + std::string(".basepl.base.color");
            initUniformLocation(i, LightingBase::SPOT_LIGHT_PARAM::COLOR, lParamName.c_str());

            lParamName = lSpotLightName + std::string(".basepl.base.ambientIntensity");
            initUniformLocation(i, LightingBase::SPOT_LIGHT_PARAM::AMBIANT_INTENSITY, lParamName.c_str());

            lParamName = lSpotLightName + std::string(".basepl.base.diffuseIntensity");
            initUniformLocation(i, LightingBase::SPOT_LIGHT_PARAM::DIFFUSE_INTENSITY, lParamName.c_str());

            lParamName = lSpotLightName + std::string(".basepl.position");
            initUniformLocation(i, LightingBase::SPOT_LIGHT_PARAM::POSITION, lParamName.c_str());

            lParamName = lSpotLightName + std::string(".direction");
            initUniformLocation(i, LightingBase::SPOT_LIGHT_PARAM::DIRECTION, lParamName.c_str());

            lParamName = lSpotLightName + std::string(".basepl.attenuation.constant");
            initUniformLocation(i, LightingBase::SPOT_LIGHT_PARAM::ATTENUATION_CONSTANT_COEFF, lParamName.c_str());

            lParamName = lSpotLightName + std::string(".basepl.attenuation.linear");
            initUniformLocation(i, LightingBase::SPOT_LIGHT_PARAM::ATTENUATION_LINEAR_COEFF, lParamName.c_str());

            lParamName = lSpotLightName + std::string(".basepl.attenuation.exponential");
            initUniformLocation(i, LightingBase::SPOT_LIGHT_PARAM::ATTENUATION_EXPONENTIAL_COEFF, lParamName.c_str());

            lParamName = lSpotLightName + std::string(".cutoff");
            initUniformLocation(i, LightingBase::SPOT_LIGHT_PARAM::CUTOFF_ANGLE, lParamName.c_str());
        }

        // Check if we correctly initialized the uniform variables
        if (!LightingBase::checkUniformLocations())
            throw Exceptions("Not all uniform locations were updated", __FILE__, __LINE__);
    }

    template<typename T>
    void LightingBase<T>::updateLightsState(const std::vector<std::shared_ptr<BaseLight>> & pLights)
    {
        unsigned int lPointLightIdx = 0;
        unsigned int lSpotLightIdx = 0;

        for (const auto i : mLightIndices)
        {
            switch (pLights[i]->type())
            {
                case BaseLight::EType::DIRECTIONAL:
                {
                    auto lDirectionalLight = std::static_pointer_cast<DirectionalLight>(pLights[i]);

                    // Update directional light
                    auto lColor = lDirectionalLight->color();
                    glUniform3f(mDirectionalLightLocations[toUT(DIRECTIONAL_LIGHT_PARAM::COLOR)], lColor.x(), lColor.y(), lColor.z());
                    glUniform1f(mDirectionalLightLocations[toUT(DIRECTIONAL_LIGHT_PARAM::AMBIANT_INTENSITY)], lDirectionalLight->ambientIntensity());

                    auto lDirection = lDirectionalLight->direction();
                    lDirection.normalize();
                    glUniform3f(mDirectionalLightLocations[toUT(DIRECTIONAL_LIGHT_PARAM::DIRECTION)], lDirection.x(), lDirection.y(), lDirection.z());
                    glUniform1f(mDirectionalLightLocations[toUT(DIRECTIONAL_LIGHT_PARAM::DIFFUSE_INTENSITY)], lDirectionalLight->diffuseIntensity());
                }   break;

                case BaseLight::EType::POINT:
                    // Ensure we're accessing a point light that was accounted for
                    if (lPointLightIdx < mPointLightLocations.size())
                    {
                        auto lPointLight = std::static_pointer_cast<PointLight>(pLights[i]);

                        // Update point lights
                        glUniform1i(mPointLightCountLocation, static_cast<GLint>(mPointLightLocations.size()));

                        auto lColor = lPointLight->color();
                        glUniform3f(mPointLightLocations[lPointLightIdx][toUT(POINT_LIGHT_PARAM::COLOR)], lColor.x(), lColor.y(), lColor.z());
                        glUniform1f(mPointLightLocations[lPointLightIdx][toUT(POINT_LIGHT_PARAM::AMBIANT_INTENSITY)], lPointLight->ambientIntensity());
                        glUniform1f(mPointLightLocations[lPointLightIdx][toUT(POINT_LIGHT_PARAM::DIFFUSE_INTENSITY)], lPointLight->diffuseIntensity());
                        glUniform3f(mPointLightLocations[lPointLightIdx][toUT(POINT_LIGHT_PARAM::POSITION)], lPointLight->position().x(), lPointLight->position().y(), lPointLight->position().z());
                        glUniform1f(mPointLightLocations[lPointLightIdx][toUT(POINT_LIGHT_PARAM::ATTENUATION_CONSTANT_COEFF)], lPointLight->attenuation(PointLight::ATTENUATION_TYPE::CONSTANT));
                        glUniform1f(mPointLightLocations[lPointLightIdx][toUT(POINT_LIGHT_PARAM::ATTENUATION_LINEAR_COEFF)], lPointLight->attenuation(PointLight::ATTENUATION_TYPE::LINEAR));
                        glUniform1f(mPointLightLocations[lPointLightIdx][toUT(POINT_LIGHT_PARAM::ATTENUATION_EXPONENTIAL_COEFF)], lPointLight->attenuation(PointLight::ATTENUATION_TYPE::EXPONENTIAL));

                        ++lPointLightIdx;
                    }
                    break;

                case BaseLight::EType::SPOT:
                    if (lSpotLightIdx < mSpotLightLocations.size())
                    {
                        auto lSpotLight = std::static_pointer_cast<SpotLight>(pLights[i]);

                        // Update spot lights
                        glUniform1i(mSpotLightCountLocation, static_cast<GLint>(mSpotLightLocations.size()));

                        auto lColor = lSpotLight->color();
                        glUniform3f(mSpotLightLocations[lSpotLightIdx][toUT(SPOT_LIGHT_PARAM::COLOR)], lColor.x(), lColor.y(), lColor.z());
                        glUniform1f(mSpotLightLocations[lSpotLightIdx][toUT(SPOT_LIGHT_PARAM::AMBIANT_INTENSITY)], lSpotLight->ambientIntensity());
                        glUniform1f(mSpotLightLocations[lSpotLightIdx][toUT(SPOT_LIGHT_PARAM::DIFFUSE_INTENSITY)], lSpotLight->diffuseIntensity());
                        glUniform3f(mSpotLightLocations[lSpotLightIdx][toUT(SPOT_LIGHT_PARAM::POSITION)], lSpotLight->position().x(), lSpotLight->position().y(), lSpotLight->position().z());
                        glUniform1f(mSpotLightLocations[lSpotLightIdx][toUT(SPOT_LIGHT_PARAM::ATTENUATION_CONSTANT_COEFF)], lSpotLight->attenuation(SpotLight::ATTENUATION_TYPE::CONSTANT));
                        glUniform1f(mSpotLightLocations[lSpotLightIdx][toUT(SPOT_LIGHT_PARAM::ATTENUATION_LINEAR_COEFF)], lSpotLight->attenuation(SpotLight::ATTENUATION_TYPE::LINEAR));
                        glUniform1f(mSpotLightLocations[lSpotLightIdx][toUT(SPOT_LIGHT_PARAM::ATTENUATION_EXPONENTIAL_COEFF)], lSpotLight->attenuation(SpotLight::ATTENUATION_TYPE::EXPONENTIAL));

                        auto lDirection = lSpotLight->direction();
                        lDirection.normalize();
                        glUniform3f(mSpotLightLocations[lSpotLightIdx][toUT(SPOT_LIGHT_PARAM::DIRECTION)], lDirection.x(), lDirection.y(), lDirection.z());
                        glUniform1f(mSpotLightLocations[lSpotLightIdx][toUT(SPOT_LIGHT_PARAM::CUTOFF_ANGLE)], cosf(lSpotLight->cutoff().toRadian()));

                        ++lSpotLightIdx;
                    }
                    break;

                case BaseLight::EType::UNDEFINED:
                default:
                    assert(false && "Undefined light type");
                    break;
            }
        }
    }

    template<typename T>
    void LightingBase<T>::pointLights(unsigned int pCount)
    {
        // Add parameter locations in container
        mPointLightLocations.assign(pCount, pointLightParamsLoc());

        // Set the initial ID to invalid location
        for (auto & light : mPointLightLocations)
        {
            for (auto & param : light)
                param = Constants::invalidUniformLocation<GLuint>();
        }
    }

    template<typename T>
    void LightingBase<T>::spotLights(unsigned int pCount)
    {
        // Add parameter locations in container
        mSpotLightLocations.assign(pCount, spotLightParamsLoc());

        // Set the initial ID to invalid location
        for (auto & light : mSpotLightLocations)
        {
            for (auto & param : light)
                param = Constants::invalidUniformLocation<GLuint>();
        }
    }

    template<typename T>
    void LightingBase<T>::lightToUseDuringRender(unsigned int pIndex)
    {
        mLightIndices.emplace_back(pIndex);
    }

    template<typename T>
    void LightingBase<T>::initUniformLocation(DIRECTIONAL_LIGHT_PARAM pParameter, const char* pName)
    {
        mDirectionalLightLocations[toUT(pParameter)] = T::uniformLocation(pName);
    }

    template<typename T>
    void LightingBase<T>::initUniformLocation(size_t pPointLightIdx, POINT_LIGHT_PARAM pParameter, const char* pName)
    {
        mPointLightLocations[pPointLightIdx][toUT(pParameter)] = T::uniformLocation(pName);
    }

    template<typename T>
    void LightingBase<T>::initUniformLocation(size_t pSpotLightIdx, SPOT_LIGHT_PARAM pParameter, const char* pName)
    {
        mSpotLightLocations[pSpotLightIdx][toUT(pParameter)] = T::uniformLocation(pName);
    }
} // namespace miniGL




