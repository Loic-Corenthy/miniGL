//===============================================================================================//
/*!
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#include "DeferredShadingDirectionalLightPass.hpp"

#include "Shader.hpp"
#include "EnumClassCast.hpp"
#include "Constants.hpp"
#include "Algebra.hpp"
#include "Exceptions.hpp"

using std::shared_ptr;
using miniGL::Shader;
using miniGL::DeferredShadingLightPass;
using miniGL::DeferredShadingDirectionalLightPass;
using miniGL::Constants;
using miniGL::Exceptions;
using miniGL::DirectionalLight;

DeferredShadingDirectionalLightPass::DeferredShadingDirectionalLightPass(void)
:DeferredShadingLightPass()
{
    mBaseColorLocation = Constants::invalidUniformLocation<GLuint>();
    mBaseAmbiantIntensityLocation = Constants::invalidUniformLocation<GLuint>();
    mBaseDiffuseIntensityLocation = Constants::invalidUniformLocation<GLuint>();
    mDirectionLocation = Constants::invalidUniformLocation<GLuint>();
}

DeferredShadingDirectionalLightPass::~DeferredShadingDirectionalLightPass(void)
{

}

void DeferredShadingDirectionalLightPass::init(void)
{
    // Create, load and compile a vertex shader
    Shader lVS(Shader::ETYPE::VERTEX);

    lVS.loadText("Shaders/LightPass.vert");
    lVS.compile();

    // Create, load and compile a fragment shader
    Shader lFS(Shader::ETYPE::FRAGMENT);

    lFS.loadText("Shaders/DirectionalLightPass.frag");
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

    // Directional light parameter locations
    mBaseColorLocation = Program::uniformLocation("uDirectionalLight.base.color");
    mBaseAmbiantIntensityLocation = Program::uniformLocation("uDirectionalLight.base.ambientIntensity");
    mDirectionLocation = Program::uniformLocation("uDirectionalLight.direction");
    mBaseDiffuseIntensityLocation = Program::uniformLocation("uDirectionalLight.base.diffuseIntensity");

    // Check if we correctly initialized the uniform variables
    if (!DeferredShadingDirectionalLightPass::checkUniformLocations())
        throw Exceptions("Not all uniform locations were updated", __FILE__, __LINE__);
}

void DeferredShadingDirectionalLightPass::updateLightsState(const shared_ptr<DirectionalLight> pLight)
{
    glUniform3f(mBaseColorLocation, pLight->color().x(), pLight->color().y(), pLight->color().z());
    glUniform1f(mBaseAmbiantIntensityLocation, pLight->ambientIntensity());
    vec3f lDirection = pLight->direction().normalized();
    glUniform3f(mDirectionLocation, lDirection.x(), lDirection.y(), lDirection.z());
    glUniform1f(mBaseDiffuseIntensityLocation, pLight->diffuseIntensity());
}

bool DeferredShadingDirectionalLightPass::checkUniformLocations(void) const
{
    return (mBaseColorLocation != Constants::invalidUniformLocation<GLuint>()            &&
            mBaseAmbiantIntensityLocation != Constants::invalidUniformLocation<GLuint>() &&
            mBaseDiffuseIntensityLocation != Constants::invalidUniformLocation<GLuint>() &&
            mDirectionLocation != Constants::invalidUniformLocation<GLuint>()            );
}
