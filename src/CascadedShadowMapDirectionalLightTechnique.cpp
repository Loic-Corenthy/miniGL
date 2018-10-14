//===============================================================================================//
/*!
 *  \file      CascadedShadowMapDirectionalLightTechnique.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#include "CascadedShadowMapDirectionalLightTechnique.hpp"

#include <numeric>

#include "Exceptions.hpp"
#include "EngineCommon.hpp"
#include "DirectionalLight.hpp"

using std::map;
using std::vector;
using std::string;
using std::make_unique;
using std::make_shared;
using std::shared_ptr;
using std::static_pointer_cast;
using std::get;
using std::tuple;
using std::numeric_limits;
using miniGL::CascadedShadowMapDirectionalLightTechnique;
using miniGL::RenderingTechniqueBase;
using miniGL::MeshAndTransform;
using miniGL::BaseLight;
using miniGL::DirectionalLight;

CascadedShadowMapDirectionalLightTechnique::CascadedShadowMapDirectionalLightTechnique(void)
:RenderingTechniqueBase("CascadedShadowMapDirectionalLightTechnique")
{
}

void CascadedShadowMapDirectionalLightTechnique::init(const tuple<unsigned int, unsigned int> & pFramebufferDimensions)
{
    if (!mCamera)
    {
        throw Exceptions("You must set a reference to the camera prior to initialize this rendering technique", __FILE__, __LINE__);
    }

    mCascadedShadowMapFBO = make_unique<CascadedShadowMapFBO>();
    mCascadedShadowMapFBO->init(get<0>(pFramebufferDimensions), get<1>(pFramebufferDimensions));

    mCascadedShadowMapDirectionalLight = make_unique<CascadedShadowMapDirectionalLight>();
    mCascadedShadowMapDirectionalLight->init();

    mCascadedShadowMapDirectionalLightLighting = make_unique<CascadedShadowMapDirectionalLightLighting>();
    mCascadedShadowMapDirectionalLightLighting->init();
    mCascadedShadowMapDirectionalLightLighting->use();
    mCascadedShadowMapDirectionalLightLighting->colorTextureUnit(COLOR_TEXTURE_UNIT_INDEX);
    mCascadedShadowMapDirectionalLightLighting->shadowTextureUnit();
    mCascadedShadowMapDirectionalLightLighting->materialSpecularIntensity(0.0f);
    mCascadedShadowMapDirectionalLightLighting->materialSpecularPower(0.0f);

    // Limits of each cascade between near and far planes in the frustrum
    mCascadeEnds[0] = mCamera->nearPlane();
    mCascadeEnds[1] = 25.0f;
    mCascadeEnds[2] = 90.0f;
    mCascadeEnds[3] = mCamera->farPlane();

    auto lPerspectiveProjection = mCamera->projection();
    for (size_t i = 0; i < mCascadeEnds.size() - 1; ++i)
    {
        auto lClipSpaceCoordinate = lPerspectiveProjection * vec4f(0.0f, 0.0f, mCascadeEnds[i + 1], 1.0f);
        mCascadedShadowMapDirectionalLightLighting->cascadeEncClipSpace(i, lClipSpaceCoordinate.z());
    }
}

void CascadedShadowMapDirectionalLightTechnique::render(const map<string, MeshAndTransform> & pMeshes, const vector<shared_ptr<BaseLight>> & pLights)
{
    // Find the graphical meshes to render
    auto lMeshReferences = findMeshesToRender(pMeshes);

    // Find the mesh representing the "floor"
    auto lFloorIterator = pMeshes.find(mFloorMeshName);
    assert(lFloorIterator != pMeshes.end());

    assert(pLights[0]->type() == BaseLight::EType::DIRECTIONAL);
    auto lDirectionalLight = static_pointer_cast<DirectionalLight>(pLights[0]);

    _shadowPass(lMeshReferences, lDirectionalLight);
    _renderPass(lMeshReferences, lFloorIterator, lDirectionalLight);
}

void CascadedShadowMapDirectionalLightTechnique::floor(const string & pName)
{
    mFloorMeshName = pName;
}
void CascadedShadowMapDirectionalLightTechnique::_computeOrthogonalProjection(shared_ptr<DirectionalLight> pLight)
{
    auto lViewInverse = mCamera->view();
    lViewInverse.inverse();

    Camera lTmpCamera = *mCamera;
    lTmpCamera.position(vec3f(0.0f, 0.0f, 0.0f));
    lTmpCamera.lookAt(pLight->direction());
    lTmpCamera.up(vec3f(0.0f, 1.0f, 0.0f));

    auto lLightView = lTmpCamera.view();

    float lTanHalfHorizontalFOV = tanf(mCamera->verticalFoV() * mCamera->aspectRatio() / 2.0f);
    float lTanHalfVerticalFOV = tanf(mCamera->verticalFoV() / 2.0f);

    const auto lFrustrumCornerCount = 8u;

    for (size_t i = 0; i < mCascadedShadowMapFBO->size(); ++i)
    {
        float lXN = mCascadeEnds[i]     * lTanHalfHorizontalFOV;
        float lXF = mCascadeEnds[i + 1] * lTanHalfHorizontalFOV;
        float lYN = mCascadeEnds[i]     * lTanHalfVerticalFOV;
        float lYF = mCascadeEnds[i + 1] * lTanHalfVerticalFOV;

        vec4f lFrustrumCorners[lFrustrumCornerCount] =
        {
            // Near plane
            vec4f( lXN,  lYN, mCascadeEnds[i], 1.0f),
            vec4f(-lXN,  lYN, mCascadeEnds[i], 1.0f),
            vec4f( lXN, -lYN, mCascadeEnds[i], 1.0f),
            vec4f(-lXN, -lYN, mCascadeEnds[i], 1.0f),

            // Far plane
            vec4f( lXF,  lYF, mCascadeEnds[i + 1], 1.0f),
            vec4f(-lXF,  lYF, mCascadeEnds[i + 1], 1.0f),
            vec4f( lXF, -lYF, mCascadeEnds[i + 1], 1.0f),
            vec4f(-lXF, -lYF, mCascadeEnds[i + 1], 1.0f)
        };

        float lMinX = numeric_limits<float>::max();
        float lMaxX = numeric_limits<float>::min();
        float lMinY = numeric_limits<float>::max();
        float lMaxY = numeric_limits<float>::min();
        float lMinZ = numeric_limits<float>::max();
        float lMaxZ = numeric_limits<float>::min();

        vec4f lFrustrumCornersLight[lFrustrumCornerCount];

        for (size_t j = 0; j < lFrustrumCornerCount; ++j)
        {
            auto lCornerWorld = lViewInverse * lFrustrumCorners[j];
            lFrustrumCornersLight[j] = lLightView * lCornerWorld;

            lMinX = std::min(lMinX, lFrustrumCornersLight[j].x());
            lMaxX = std::max(lMaxX, lFrustrumCornersLight[j].x());
            lMinY = std::min(lMinY, lFrustrumCornersLight[j].y());
            lMaxY = std::max(lMaxY, lFrustrumCornersLight[j].y());
            lMinZ = std::min(lMinZ, lFrustrumCornersLight[j].z());
            lMaxZ = std::max(lMaxZ, lFrustrumCornersLight[j].z());
        }

        mCamera->orthogonalProjectionParameter(i, Camera::EOrthogonalProjection::LEFT, lMinX);
        mCamera->orthogonalProjectionParameter(i, Camera::EOrthogonalProjection::RIGHT, lMaxX);
        mCamera->orthogonalProjectionParameter(i, Camera::EOrthogonalProjection::TOP, lMaxY);
        mCamera->orthogonalProjectionParameter(i, Camera::EOrthogonalProjection::BOTTOM, lMinY);
        mCamera->orthogonalProjectionParameter(i, Camera::EOrthogonalProjection::NEAR, lMinZ);
        mCamera->orthogonalProjectionParameter(i, Camera::EOrthogonalProjection::FAR, lMaxZ);
    }
}

void CascadedShadowMapDirectionalLightTechnique::_shadowPass(vector<map<string, MeshAndTransform>::const_iterator> & pMeshIterators, shared_ptr<DirectionalLight> pLight)
{
    _computeOrthogonalProjection(pLight);

    mCascadedShadowMapDirectionalLight->use();

    // Set the tmp camera as the directional light
    Camera lTmpCamera = *mCamera;
    lTmpCamera.position(vec3f(0.0f, 0.0f, 0.0f));
    lTmpCamera.lookAt(pLight->direction());
    lTmpCamera.up(vec3f(0.0f, 1.0f, 0.0f));

    for (size_t i = 0; i < mCascadedShadowMapFBO->size(); ++i)
    {
        // Bind and clear the depth buffer for the current cascade
        mCascadedShadowMapFBO->bindForWriting(i);
        glClear(GL_DEPTH_BUFFER_BIT);

        for (auto it : pMeshIterators)
        {
            for (auto transform : it->second.transform)
            {
                mat4f lWorld = transform.final();
                mat4f lWVP = lTmpCamera.orthogonalProjection(i) * lTmpCamera.view() * lWorld;

                mCascadedShadowMapDirectionalLight->WVP(lWVP);

                it->second.mesh->render();
            }
        }
    }
}

void CascadedShadowMapDirectionalLightTechnique::_renderPass(vector<map<string, MeshAndTransform>::const_iterator> & pMeshIterators, map<string, MeshAndTransform>::const_iterator & pFloorIterator, shared_ptr<DirectionalLight> pLight)
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mCascadedShadowMapDirectionalLightLighting->use();
    mCascadedShadowMapDirectionalLightLighting->eyeWorldPosition(mCamera->position());

    mCascadedShadowMapFBO->bindForReading();

    // Set the tmp camera as the directional light
    Camera lTmpCamera = *mCamera;
    lTmpCamera.position(vec3f(0.0f, 0.0f, 0.0f));
    lTmpCamera.lookAt(pLight->direction());
    lTmpCamera.up(vec3f(0.0f, 1.0f, 0.0f));

    // Configure the WVP for the light with the orthogonal projection
    mat4f lWorld = pFloorIterator->second.transform[0].final();

    for (size_t i = 0; i < mCascadedShadowMapFBO->size(); i++)
    {
        mat4f lLightWVP = lTmpCamera.orthogonalProjection(i) * lTmpCamera.view() * lWorld;
        mCascadedShadowMapDirectionalLightLighting->lightWVP(i, lLightWVP);
    }

    // Update the parameters of the directional light to the GPU
    mCascadedShadowMapDirectionalLightLighting->updateDirectionalLightState(pLight);

    // Configure the WVP for the quad, with the normal perspective projection of the camera
    mat4f lWVP = mCamera->projection() * mCamera->view() * lWorld;
    mCascadedShadowMapDirectionalLightLighting->world(lWorld);
    mCascadedShadowMapDirectionalLightLighting->WVP(lWVP);

    // Render the shadow on the floor
    pFloorIterator->second.mesh->render();

    for (auto it : pMeshIterators)
    {
        for (auto transform : it->second.transform)
        {
            mat4f lWorld = transform.final();
            mat4f lWVP = mCamera->projection() * mCamera->view()  * lWorld;

            mCascadedShadowMapDirectionalLightLighting->world(lWorld);
            mCascadedShadowMapDirectionalLightLighting->WVP(lWVP);

            it->second.mesh->render();
        }
    }
}
