//===============================================================================================//
/*!
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#include "DeferredShadingNullPass.hpp"

#include "Exceptions.hpp"
#include "Shader.hpp"

using std::string;
using std::to_string;
using miniGL::DeferredShadingNullPass;
using miniGL::Constants;
using miniGL::Shader;
using miniGL::Exceptions;

DeferredShadingNullPass::~DeferredShadingNullPass(void)
{

}

void DeferredShadingNullPass::init(void)
{
    // Create, load and compile a vertex shader
    Shader lVS(Shader::ETYPE::VERTEX);

    lVS.loadText("Shaders/NullPass.vert");
    lVS.compile();

    // Create, load and compile a fragment shader
    Shader lFS(Shader::ETYPE::FRAGMENT);

    lFS.loadText("Shaders/NullPass.frag");
    lFS.compile();

    // Load the vertex and fragment shader into our program, link and validate
    attachShader(lVS);
    attachShader(lFS);

    link();

    detachAndDeleteShader(lVS);
    detachAndDeleteShader(lFS);

    // Use our program
    use();

    mWVPLocation = Program::uniformLocation("uWVP");

    // Check if we correctly initialized the uniform variables
    if (!checkUniformLocations())
        throw Exceptions("Not all uniform locations were updated", __FILE__, __LINE__);
}
