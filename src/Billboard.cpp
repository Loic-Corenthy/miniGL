//===============================================================================================//
/*!
 *  \file      Billboard.cpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#include "Billboard.hpp"

#include "Shader.hpp"
#include "Exceptions.hpp"

using std::string;
using miniGL::Billboard;
using miniGL::Shader;
using miniGL::Constants;
using miniGL::Exceptions;

Billboard::Billboard(void)
:Program()
{

}

void Billboard::init(const string & pPathVS, const string & pPathGS, const string & pPathFS)
{
    // Create, load and compile a vertex shader
    Shader lVS(Shader::ETYPE::VERTEX);

    lVS.loadText(pPathVS.c_str());
    lVS.compile();

    // Create, load and compile a geometry shader
    Shader lGS(Shader::ETYPE::GEOMETRY);

    lGS.loadText(pPathGS.c_str());
    lGS.compile();

    // Create, load and compile a fragment shader
    Shader lFS(Shader::ETYPE::FRAGMENT);

    lFS.loadText(pPathFS.c_str());
    lFS.compile();

    // Load the vertex, geometry and fragment shader into our program, link and validate
    attachShader(lVS);
    attachShader(lGS);
    attachShader(lFS);

    link();

    detachAndDeleteShader(lVS);
    detachAndDeleteShader(lGS);
    detachAndDeleteShader(lFS);

    // Use our program
    use();

    mVPLocation = Program::uniformLocation("uVP");
    mCameraPositionLocation = Program::uniformLocation("uCameraPosition");
    mColorMapLocation = Program::uniformLocation("uColorMap");
    mSizeLocation = Program::uniformLocation("uBillboardSize");
    mCutoffMax = Program::uniformLocation("uCutoffMax");
    mActivateCutoffMax = Program::uniformLocation("uActivateCutoffMax");
    mCutoffMin = Program::uniformLocation("uCutoffMin");
    mActivateCutoffMin = Program::uniformLocation("uActivateCutoffMin");

    if(!checkUniformLocations())
        throw Exceptions("Not all uniform locations were updated", __FILE__, __LINE__);

    // By default, deactivate cutoff
    activateCutoff(Billboard::ECutoff::MIN, false);
    cutoff(Billboard::ECutoff::MIN, 0.0);
    activateCutoff(Billboard::ECutoff::MAX, false);
    cutoff(Billboard::ECutoff::MAX, 1.0);
}

void Billboard::VP(const mat4f & pVP)
{
    glUniformMatrix4fv(mVPLocation, 1, GL_TRUE, const_cast<mat4f &>(pVP).data());
}

void Billboard::eyeWorldPosition(const vec3f & pPos)
{
    glUniform3f(mCameraPositionLocation, pPos.x(), pPos.y(), pPos.z());
}

void Billboard::textureUnit(unsigned int pTextureUnit)
{
    glUniform1i(mColorMapLocation, pTextureUnit);
}

void Billboard::activateCutoff(ECutoff pLimit, bool pActivate)
{
    switch (pLimit)
    {
        case ECutoff::MIN:
            glUniform1i(mActivateCutoffMin, pActivate?1:0);
            break;

        case ECutoff::MAX:
            glUniform1i(mActivateCutoffMax, pActivate?1:0);
            break;
    }
}

void Billboard::cutoff(ECutoff pLimit, float pValue)
{
    switch (pLimit)
    {
        case ECutoff::MIN:
            glUniform1f(mCutoffMin, pValue);
            break;

        case ECutoff::MAX:
            glUniform1f(mCutoffMax, pValue);
            break;
    }
}

void Billboard::size(float pValue)
{
    glUniform1f(mSizeLocation, pValue);
}

bool Billboard::checkUniformLocations(void) const
{
    return (mVPLocation != Constants::invalidUniformLocation<GLuint>()              &&
            mCameraPositionLocation != Constants::invalidUniformLocation<GLuint>()  &&
            mColorMapLocation != Constants::invalidUniformLocation<GLuint>()        &&
            mSizeLocation != Constants::invalidUniformLocation<GLuint>()            &&
            mCutoffMax != Constants::invalidUniformLocation<GLuint>()               &&
            mActivateCutoffMax != Constants::invalidUniformLocation<GLuint>()       &&
            mCutoffMin != Constants::invalidUniformLocation<GLuint>()               &&
            mActivateCutoffMin != Constants::invalidUniformLocation<GLuint>());
}
