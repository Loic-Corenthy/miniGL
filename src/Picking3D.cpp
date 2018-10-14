//===============================================================================================//
/*!
 *  \file      Picking3D.cpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#include "Picking3D.hpp"

using std::make_unique;
using std::shared_ptr;
using std::string;
using std::string;
using std::map;
using std::get;
using std::tuple;
using miniGL::Picking3D;
using miniGL::MeshAndTransform;
using miniGL::Camera;

void Picking3D::init(const tuple<int, int> & pFramebufferDimensions, const tuple<int, int> & pWindowDimensions)
{
    mPickingRender = make_unique<PickingRender>();
    mPickingRender->init();

    mFrameBufferDimensions = pFramebufferDimensions;

    enum E { width, height};
    mPickingTexture.init(get<E::width>(mFrameBufferDimensions), get<E::height>(pFramebufferDimensions));

    /*! \todo Need to add a method to handle the case where the window is resized at run time */
    mScaling.x() = static_cast<double>(get<E::width>(mFrameBufferDimensions))/static_cast<double>(get<E::width>(pWindowDimensions));
    mScaling.y() = static_cast<double>(get<E::height>(mFrameBufferDimensions))/static_cast<double>(get<E::height>(pWindowDimensions));

    mSimpleColor = make_unique<SimpleColorRender>();
    mSimpleColor->init();
}

void Picking3D::pickingPhase(map<string, MeshAndTransform> & pMeshes)
{
    mPickingTexture.enableWritting();

    mPickingRender->use();

    auto lMesh = pMeshes.at(mMeshToRenderName);

    for (size_t i = 0; i < lMesh.transform.size(); ++i)
    {
        mPickingRender->objectIndex(static_cast<unsigned int>(i));

        mat4f lWVP = mCamera->projection() * mCamera->view() * lMesh.transform[i].final();

        mPickingRender->WVP(lWVP);

        lMesh.mesh->render(MeshBase::EPrimitiveType::TRIANGLE, mPickingRender.get());
    }

    mPickingTexture.disableWritting();
}

void Picking3D::render(map<string, MeshAndTransform> & pMeshes, const vec2i & pMousePosition)
{
    const auto lMousePosInFrameBufferX = static_cast<unsigned int>(static_cast<double>(pMousePosition.x()) * mScaling.x());
    const auto lMousePosInFrameBufferY = get<1>(mFrameBufferDimensions) - static_cast<unsigned int>(static_cast<double>(pMousePosition.y()) * mScaling.y()) - 1;

    PickingTexture::PixelInfo lPixelInfo = mPickingTexture.readPixel(lMousePosInFrameBufferX, lMousePosInFrameBufferY);

    if (!(isnan(lPixelInfo.drawID) || isnan(lPixelInfo.objectID) || isnan(lPixelInfo.primitiveID)) && lPixelInfo.primitiveID != 0)
    {
        auto lMesh = pMeshes.at(mMeshToRenderName);

        mSimpleColor->use();

        assert(lPixelInfo.objectID < lMesh.transform.size());

        mat4f lWVP = mCamera->projection() * mCamera->view() * lMesh.transform.at(static_cast<unsigned int>(lPixelInfo.objectID)).final();

        mSimpleColor->WVP(lWVP);
        mSimpleColor->color(0.3f, 0.4f, 0.9f);
        // Compensate for the "artificial" increment of the primitive ID in the fragment shader
        lMesh.mesh->render(static_cast<unsigned int>(lPixelInfo.drawID), static_cast<unsigned int>(lPixelInfo.primitiveID) - 1);
    }
}

void Picking3D::camera(const shared_ptr<Camera> pCamera)
{
    mCamera = pCamera;
}

void Picking3D::meshToRender(const string & pName)
{
    mMeshToRenderName = pName;
}
