//===============================================================================================//
/*!
 *  \file      MeshBase.cpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#include "MeshBase.hpp"

#include <cassert>
#include <iostream>

#include "Constants.hpp"
#include "Exceptions.hpp"
#include "EngineCommon.hpp"

using std::vector;
using std::string;
using std::cout;
using std::endl;
using Assimp::Importer;
using miniGL::MeshBase;
using miniGL::Constants;
using miniGL::Exceptions;

MeshBase::MeshBase(const std::string & pName)
:mName(pName)
{

}

MeshBase::~MeshBase(void)
{
    clearTextures();
    clearVAOs();
}

GLuint MeshBase::createVAO(void)
{
    GLuint lVAO = 0;
    glGenVertexArrays(1, & lVAO);
    mVAOs.push_back(lVAO);

    return lVAO;
}

void MeshBase::bindVAO(unsigned int pIndex)
{
    assert(mVAOs[pIndex] != 0 && "Impossible to bind a VAO with a value of 0");
    glBindVertexArray(mVAOs[pIndex]);
}

void MeshBase::unbindVAO(void)
{
    glBindVertexArray(0);
}

std::string MeshBase::name(void) const
{
    return mName;
}

void MeshBase::name(const std::string & pValue)
{
    mName = pValue;
}

void MeshBase::frontFace(GLenum pValue) noexcept
{
    assert(pValue == GL_CCW || pValue == GL_CW);
    mOrientation = pValue;
}

GLenum MeshBase::frontFace(void) const noexcept
{
    return mOrientation;
}

bool MeshBase::initMaterials(const aiScene* pScene, const string & pFile)
{
    bool lInitMaterialOk = false;

    // Initialize the materials
    for (unsigned int i = 0; i < pScene->mNumMaterials; i++)
    {
        const aiMaterial* pMaterial = pScene->mMaterials[i];

        if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0)
        {
            aiString lPath;

            if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &lPath, nullptr, nullptr, nullptr, nullptr, nullptr) == AI_SUCCESS)
            {
                string lTmpPath(lPath.data);

                if (lTmpPath.size() >= 3 && *(lTmpPath.end() - 3) == 'p' && *(lTmpPath.end() - 2) == 'c' && *(lTmpPath.end() - 1) == 'x')
                {
                    *(lTmpPath.end() - 3) = 'j';
                    *(lTmpPath.end() - 2) = 'p';
                    *(lTmpPath.end() - 1) = 'g';
                }

                mTextures[i] = new Texture(GL_TEXTURE_2D, lTmpPath);

                if (!mTextures[i]->loadImage())
                {
                    delete mTextures[i];
                    mTextures[i] = nullptr;
                    lInitMaterialOk =  false;

                    throw Exceptions(lTmpPath, __FILE__, __LINE__);
                }
                else
                    lInitMaterialOk = true;
            }
        }

        lInitMaterialOk = true;
    }

    return lInitMaterialOk;
}

void MeshBase::clearTextures(void)
{
    for (auto & it : mTextures)
    {
        if(it != nullptr)
        {
            delete it;
            it = nullptr;
        }
    }
}

void MeshBase::clearVAOs(void)
{
    for (auto & it : mVAOs)
    {
        if(it != 0)
        {
            glDeleteVertexArrays(1, & it);
            it = 0;
        }
    }
}

MeshBase::EOptions MeshBase::loadOption(void) const noexcept
{
    return mLoadOptions;
}
