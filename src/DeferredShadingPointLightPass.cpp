//===============================================================================================//
/*!
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#include "DeferredShadingPointLightPass.hpp"

#include <string>

#include "Shader.hpp"
#include "Exceptions.hpp"
#include "Algebra.hpp"

using std::string;
using std::to_string;
using miniGL::DeferredShadingPointLightPass;
using miniGL::Constants;
using miniGL::Shader;
using miniGL::Exceptions;
using miniGL::PointLight;

DeferredShadingPointLightPass::DeferredShadingPointLightPass(void)
{
    for (auto & it : mParameterLocations)
        it = Constants::invalidUniformLocation<GLuint>();
}

DeferredShadingPointLightPass::~DeferredShadingPointLightPass(void)
{

}

void DeferredShadingPointLightPass::init(void)
{
    // Create, load and compile a vertex shader
    Shader lVS(Shader::ETYPE::VERTEX);

    lVS.loadText("Shaders/LightPass.vert");
    lVS.compile();

    // Create, load and compile a fragment shader
    Shader lFS(Shader::ETYPE::FRAGMENT);

    lFS.loadText("Shaders/PointLightPass.frag");
    lFS.compile();

    // Load the vertex and fragment shader into our program, link and validate
    attachShader(lVS);
    attachShader(lFS);

    link();

    detachAndDeleteShader(lVS);
    detachAndDeleteShader(lFS);

    // Use our program
    use();

    DeferredShadingLightPass::init();

    mParameterLocations[toUT(EParam::COLOR)] = Program::uniformLocation("uPointLight.base.color");
    mParameterLocations[toUT(EParam::AMBIANT_INTENSITY)] = Program::uniformLocation("uPointLight.base.ambientIntensity");
    mParameterLocations[toUT(EParam::DIFFUSE_INTENSITY)] = Program::uniformLocation("uPointLight.base.diffuseIntensity");
    mParameterLocations[toUT(EParam::POSITION)] = Program::uniformLocation("uPointLight.position");
    mParameterLocations[toUT(EParam::ATTENUATION_CONSTANT_COEFF)] = Program::uniformLocation("uPointLight.attenuation.constant");
    mParameterLocations[toUT(EParam::ATTENUATION_LINEAR_COEFF)] = Program::uniformLocation("uPointLight.attenuation.linear");
    mParameterLocations[toUT(EParam::ATTENUATION_EXPONENTIAL_COEFF)] = Program::uniformLocation("uPointLight.attenuation.exponential");

    // Check if we correctly initialized the uniform variables
    if (!DeferredShadingPointLightPass::checkUniformLocations())
        throw Exceptions("Not all uniform locations were updated", __FILE__, __LINE__);
}

bool DeferredShadingPointLightPass::checkUniformLocations(void) const
{
    bool lRes = true;

    for(auto param : mParameterLocations)
        lRes &= (param != Constants::invalidUniformLocation<GLuint>());

    return lRes;
}

void DeferredShadingPointLightPass::updateLightState(const PointLight & pLight)
{
    const vec3f lColor = pLight.color();
    glUniform3f(mParameterLocations[toUT(EParam::COLOR)], lColor.x(), lColor.y(), lColor.z());
    glUniform1f(mParameterLocations[toUT(EParam::AMBIANT_INTENSITY)], pLight.ambientIntensity());
    glUniform1f(mParameterLocations[toUT(EParam::DIFFUSE_INTENSITY)], pLight.diffuseIntensity());
    glUniform3f(mParameterLocations[toUT(EParam::POSITION)], pLight.position().x(), pLight.position().y(), pLight.position().z());
    glUniform1f(mParameterLocations[toUT(EParam::ATTENUATION_CONSTANT_COEFF)], pLight.attenuation(PointLight::ATTENUATION_TYPE::CONSTANT));
    glUniform1f(mParameterLocations[toUT(EParam::ATTENUATION_LINEAR_COEFF)], pLight.attenuation(PointLight::ATTENUATION_TYPE::LINEAR));
    glUniform1f(mParameterLocations[toUT(EParam::ATTENUATION_EXPONENTIAL_COEFF)], pLight.attenuation(PointLight::ATTENUATION_TYPE::EXPONENTIAL));
}
