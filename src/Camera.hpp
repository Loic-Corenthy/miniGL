//===============================================================================================//
/*!
 *  \file      Camera.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

#include <array>
#include <vector>

#include "Constants.hpp"
#include "Algebra.hpp"
#include "Angle.hpp"

namespace miniGL
{
    /*!
     *  \brief  This class represents the camera used to render the images
     *  \details This class also provides some useful methods to link the camera with a mouse or handle
     *           orthogonal projections
     */
    class Camera
    {
    public:
        enum class EOrthogonalProjection
        {
            LEFT,
            RIGHT,
            BOTTOM,
            TOP,
            NEAR,
            FAR
        };

    public:
        /*!
         *  \brief Default constructor
         */
        Camera(void);

        /*!
         *  \brief Destructor
         */
        ~Camera(void);

        /*!
         *  \brief Copy constructor
         */
        Camera(const Camera & pCamera);

        /*!
         *  \brief Copy assignment operator
         *  @param pCamera is the camera to copy parameters from
         */
        Camera operator=(const Camera & pCamera);

        /*!
         *  \brief Get the aspect ratio
         *  @return the aspect ratio
         */
        float aspectRatio(void) const noexcept;

        /*!
         *  \brief Get the angle corresponding to the vertical field of View
         *  @return an angle in radian
         */
        radianf verticalFoV(void) const noexcept;

        /*!
         *  \brief Get the distance between the near plane and the camera
         *  @return a distance in arbitrary unit
         */
        float nearPlane(void) const noexcept;

        /*!
         *  \brief Get the distance between the far plane and the camera
         *  @return a distance in arbitrary unit
         */
        float farPlane(void) const noexcept;

        /*!
         *  \brief Get the focal distance of the camera
         *  @return a distance in arbitrary unit
         */
        float focalDist(void) const noexcept;

        /*!
         *  \brief Get the position of the camera
         *  @return a vector in world coordinates
         */
        const vec3f & position(void) const noexcept;

        /*!
         *  \brief Get the direction of the camera
         *  @return a vector in world coordinates
         */
        const vec3f & lookAt(void) const noexcept;

        /*!
         *  \brief Get the up direction of the camera
         *  @return a vector in world coordinates
         */
        const vec3f & up(void) const noexcept;

        /*!
         *  \brief Get the view matrix
         *  @return a 4x4 matrix representing the transformation representing the viewing angle of the camera
         */
        const mat4f & view(void);

        /*!
         *  \brief Get the projection matrix
         *  @return a 4x4 matrix representing the transformation associated to the projection of the 3D world into
         *          the 2D image plane of the camera
         */
        const mat4f & projection(void);

        /*!
         *  \brief Get an orthogonal projection matrix
         *  @param pIndex allows to select which orthogonal projection to get
         *  @return a 4x4 matrix representing the transformation associated to the orthogonal projection of the 3D world into
         *          the 2D image plane of the camera, i.e. no perspective
         */
        mat4f orthogonalProjection(size_t pIndex);

        /*!
         *  \brief Set the angle corresponding to the vertical field of View
         *  @param pAngle is the angle in degrees
         */
        void verticalFoV(radianf pAngle) noexcept;

        /*!
         *  \brief Set the distance between the near plane and the camera
         *  @param pValue is the distance in arbitrary unit
         */
        void nearPlane(float pValue) noexcept;

        /*!
         *  \brief Set the distance between the far plane and the camera
         *  @param pValue is the distance in arbitrary unit
         */
        void farPlane(float pValue) noexcept;

        /*!
         *  \brief Set the focal distance of the camera
         *  @param pValue is the distance in arbitrary unit
         */
        void focalDist(float pValue) noexcept;

        /*!
         *  \brief Set the position of the camera
         *  @param pPosition is the position of the camera in the 3D world
         */
        void position(const vec3f & pPosition) noexcept;

        /*!
         *  \brief Set the direction of the camera
         *  @param pLookAt is a vector indicating in which direction the camera is looking at
         */
        void lookAt(const vec3f & pLookat);

        /*!
         *  \brief Set the up direction of the camera
         *  @param pUp is a vector indicating the direction above the camera
         */
        void up(const vec3f & pUp);

        /*!
         *  \brief Set the dimensions of the frame buffer
         *  @param pWidth is the width of frame buffer in pixels
         *  @param pHeight is the height of frame buffer in pixels
         */
        void frameBufferDimensions(unsigned int pWidth, unsigned int pHeight) noexcept;

        /*!
         *  \brief Set the dimensions of the current window
         *  @param pWidth is the width of the window
         *  @param pHeight is the height of the window
         */
        void windowDimensions(unsigned int pWidth, unsigned int pHeight) noexcept;

        /*!
         *  \brief Rotate the camera using the coordinates of the mouse
         *  @param pX is the X coordinate of the mouse on screen
         *  @param pY is the Y coordinate of the mouse on screen
         */
        void rotateFromMouse(int pX, int pY);

        /*!
         *  \brief Set the sensibility of the camera roation using the mouse
         *  @param pValue is the sensibility parameter, between 1 and 10
         */
        void rotateFromMouseSensibility(float pValue);

        /*!
         *  \brief Update the orientation of the camera according to the current position of the mouse on the screen
         */
        void onRender(void);

        /*!
         * \brief Set the number of orthogonal projection for the camera
         * @param pValue is the number of orthogonal projection that the camera will store
         */
        void orthogonalProjectionParameterCount(size_t pValue);

        /*!
         * \brief Set the distances for each plane of the orthogonal projection
         * @param pSide is the projection direction defined by one of the enum values in EOrthogonalProjection
         * @param pValue is the distance value
         */
        void orthogonalProjectionParameter(size_t pIndex, EOrthogonalProjection pSide, float pValue);

    private:
        /*!
         *  \brief Update the view matrix
         */
        void _updateView(void);

        /*!
         *  \brief Update the projection matrix
         */
        void _updateProjection(void);

        /*!
         *  \brief Update the projection matrix
         */
        void _updateOrthogonalProjection(size_t pIndex);

        /*!
         *  \brief Update the orirentation of the camera according to a mouse or keyboard event
         */
        void _updateOrientation(void);

    private:
        std::vector<mat4f> mOrthogonalProjection;
        std::vector<std::array<float, 6>> mOrthogonalProjectionDirections;
        std::vector<bool> mOrthogonalProjectionHasChanged;

        mat4f mView;
        mat4f mProjection;

        mat4f mMouseRotation;

        vec3f mPosition;
        vec3f mLookAt;
        vec3f mUp;

        radianf mVerticalFoV;
        float mAspectRatio;
        float mNearPlane;
        float mFarPlane;
        float mFocalDist;

        unsigned int mFrameBufferWidth;
        unsigned int mFrameBufferHeight;
        unsigned int mWindowWidth;
        unsigned int mWindowHeight;

        vec2f mMousePos;
        float mMouseRotationSensibility;

        degreef mAngleH;
        degreef mAngleV;

        const float mEdgeStep = 0.5f;
        const int mMargin = 100;

        bool mViewHasChanged;
        bool mProjectionHasChanged;

    }; // class Camera

} // namespace miniGL

