//===============================================================================================//
/*!
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#include "DeferredShadingSpotLightPass.hpp"

#include <string>

#include "Shader.hpp"
#include "Exceptions.hpp"
#include "Algebra.hpp"

using std::string;
using std::to_string;
using miniGL::DeferredShadingSpotLightPass;
using miniGL::Constants;
using miniGL::Shader;
using miniGL::Exceptions;
using miniGL::SpotLight;

DeferredShadingSpotLightPass::DeferredShadingSpotLightPass(void)
{
    for (auto & it : mParameterLocations)
        it = Constants::invalidUniformLocation<GLuint>();
}

DeferredShadingSpotLightPass::~DeferredShadingSpotLightPass(void)
{

}

void DeferredShadingSpotLightPass::init(void)
{
    // Create, load and compile a vertex shader
    Shader lVS(Shader::ETYPE::VERTEX);

    lVS.loadText("Shaders/LightPass.vert");
    lVS.compile();

    // Create, load and compile a fragment shader
    Shader lFS(Shader::ETYPE::FRAGMENT);

    lFS.loadText("Shaders/SpotLightPass.frag");
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

    mParameterLocations[toUT(EParam::COLOR)] = Program::uniformLocation("uSpotLight.basepl.base.color");
    mParameterLocations[toUT(EParam::AMBIANT_INTENSITY)] = Program::uniformLocation("uSpotLight.basepl.base.ambientIntensity");
    mParameterLocations[toUT(EParam::DIFFUSE_INTENSITY)] = Program::uniformLocation("uSpotLight.basepl.base.diffuseIntensity");
    mParameterLocations[toUT(EParam::POSITION)] = Program::uniformLocation("uSpotLight.basepl.position");
    mParameterLocations[toUT(EParam::DIRECTION)] = Program::uniformLocation("uSpotLight.direction");
    mParameterLocations[toUT(EParam::ATTENUATION_CONSTANT_COEFF)] = Program::uniformLocation("uSpotLight.basepl.attenuation.constant");
    mParameterLocations[toUT(EParam::ATTENUATION_LINEAR_COEFF)] = Program::uniformLocation("uSpotLight.basepl.attenuation.linear");
    mParameterLocations[toUT(EParam::ATTENUATION_EXPONENTIAL_COEFF)] = Program::uniformLocation("uSpotLight.basepl.attenuation.exponential");
    mParameterLocations[toUT(EParam::CUTOFF)] = Program::uniformLocation("uSpotLight.cutoff");

    // Check if we correctly initialized the uniform variables
    if (!DeferredShadingSpotLightPass::checkUniformLocations())
        throw Exceptions("Not all uniform locations were updated", __FILE__, __LINE__);
}

bool DeferredShadingSpotLightPass::checkUniformLocations(void) const
{
    bool lRes = true;

    for(auto param : mParameterLocations)
        lRes &= (param != Constants::invalidUniformLocation<GLuint>());

    return lRes;
}

void DeferredShadingSpotLightPass::updateLightState(const SpotLight & pLight)
{
    const vec3f lColor = pLight.color();
    glUniform3f(mParameterLocations[toUT(EParam::COLOR)], lColor.x(), lColor.y(), lColor.z());
    glUniform1f(mParameterLocations[toUT(EParam::AMBIANT_INTENSITY)], pLight.ambientIntensity());
    glUniform1f(mParameterLocations[toUT(EParam::DIFFUSE_INTENSITY)], pLight.diffuseIntensity());
    glUniform3f(mParameterLocations[toUT(EParam::POSITION)], pLight.position().x(), pLight.position().y(), pLight.position().z());
    glUniform1f(mParameterLocations[toUT(EParam::ATTENUATION_CONSTANT_COEFF)], pLight.attenuation(SpotLight::ATTENUATION_TYPE::CONSTANT));
    glUniform1f(mParameterLocations[toUT(EParam::ATTENUATION_LINEAR_COEFF)], pLight.attenuation(SpotLight::ATTENUATION_TYPE::LINEAR));
    glUniform1f(mParameterLocations[toUT(EParam::ATTENUATION_EXPONENTIAL_COEFF)], pLight.attenuation(SpotLight::ATTENUATION_TYPE::EXPONENTIAL));

    auto lDirection = pLight.direction();
    lDirection.normalize();
    glUniform3f(mParameterLocations[toUT(EParam::DIRECTION)], lDirection.x(), lDirection.y(), lDirection.z());
    glUniform1f(mParameterLocations[toUT(EParam::CUTOFF)], cosf(pLight.cutoff().toRadian()));
}
