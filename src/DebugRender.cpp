//===============================================================================================//
/*!
 *  \file      DebugRender.cpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#include "DebugRender.hpp"

#include "Shader.hpp"
#include "Exceptions.hpp"

using std::string;
using miniGL::DebugRender;
using miniGL::Shader;
using miniGL::Constants;
using miniGL::Exceptions;

DebugRender::DebugRender(void)
:Program()
{

}

void DebugRender::init(void)
{
    // Create, load and compile a vertex shader
    Shader lVS(Shader::ETYPE::VERTEX);

    lVS.loadText(R"(./Shaders/Debug.vert)");
    lVS.compile();

    // Create, load and compile a fragment shader
    Shader lFS(Shader::ETYPE::FRAGMENT);

    lFS.loadText(R"(./Shaders/Debug.frag)");
    lFS.compile();

    // Load the vertex, geometry and fragment shader into our program, link and validate
    attachShader(lVS);
    attachShader(lFS);

    link();

    detachAndDeleteShader(lVS);
    detachAndDeleteShader(lFS);

    // Use our program
    use();

    mWVPLocation = Program::uniformLocation("uWVP");

    if(!checkUniformLocations())
        throw Exceptions("Not all uniform locations were updated", __FILE__, __LINE__);
}
