//===============================================================================================//
/*!
 *  \file      RenderingTechniqueBase.cpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#include "RenderingTechniqueBase.hpp"

using std::shared_ptr;
using std::string;
using std::vector;
using std::map;
using miniGL::RenderingTechniqueBase;
using miniGL::MeshAndTransform;
using miniGL::Camera;

RenderingTechniqueBase::RenderingTechniqueBase(const string & pName)
:mName(pName)
{
}

void RenderingTechniqueBase::camera(const shared_ptr<Camera> pCamera)
{
    mCamera = pCamera;
}

void RenderingTechniqueBase::addMeshToRender(const string & pName)
{
    mMeshToRenderNames.push_back(pName);
}

const vector<string> & RenderingTechniqueBase::meshToRenderNames(void) const
{
    return mMeshToRenderNames;
}

string RenderingTechniqueBase::name(void) const noexcept
{
    return mName;
}

void RenderingTechniqueBase::name(const string & pName)
{
    mName = pName;
}

vector<map<string, MeshAndTransform>::const_iterator> RenderingTechniqueBase::findMeshesToRender(const map<string, MeshAndTransform> & pMeshes) const
{
    vector<map<string, MeshAndTransform>::const_iterator> lMeshReferences;
    lMeshReferences.reserve(mMeshToRenderNames.size());

    for (auto it = pMeshes.cbegin(); it != pMeshes.cend(); it++)
    {
        for (const auto name : mMeshToRenderNames)
        {
            if (it->first == name)
            {
                lMeshReferences.push_back(it);
            }
        }
    }

    return lMeshReferences;
}
