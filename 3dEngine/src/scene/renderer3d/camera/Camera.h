#pragma once

#include <UrchinCommon.h>

namespace urchin {

    class Camera {
        public:
            Camera(float, float, float);
            virtual ~Camera() = default;

            void initialize(unsigned int, unsigned int);
            void onResize(unsigned int, unsigned int);

            void resetPreviousMousePosition(double previousMouseX = std::numeric_limits<double>::max(), double previousMouseY = std::numeric_limits<double>::max());
            void useMouseToMoveCamera(bool);
            bool isUseMouseToMoveCamera() const;
            void setMouseSensitivityPercentage(float);
            void setInvertYAxis(bool);
            void setDistance(float);
            void setMaxRotationX(float);

            const Matrix4<float>& getViewMatrix() const;
            const Matrix4<float>& getProjectionMatrix() const;
            const Frustum<float>& getFrustum() const;

            const Point3<float>& getPosition() const;
            const Vector3<float>& getView() const;
            const Vector3<float>& getUp() const;

            unsigned int getSceneWidth() const;
            unsigned int getSceneHeight() const;
            float getAngle() const;
            float getNearPlane() const;
            float getFarPlane() const;

            void moveTo(const Point3<float>&);
            void moveOnLocalXAxis(float);
            void moveOnLocalZAxis(float);
            void lookAt(const Vector3<float>&);
            void rotate(const Quaternion<float>&);

            virtual bool onKeyPress(unsigned int);
            virtual bool onKeyRelease(unsigned int);
            virtual void updateCameraView(float) = 0;

            virtual bool onMouseMove(double, double);

        private:
            void initializeOrUpdate(unsigned int, unsigned int);
            void updateViewMatrix();

            const float MOUSE_SENSITIVITY_FACTOR;
            Matrix4<float> mView, mProjection;

            float angle, nearPlane, farPlane;
            Frustum<float> baseFrustum; //base frustum (without any matrix transformation)
            Frustum<float> frustum;

            Point3<float> position;
            Vector3<float> view, up;
            float maxRotationX;
            float distance; //distance between the camera and the rotation point (0 : first person camera | >0 : third person camera)

            bool bUseMouse; //true if the cursor is used to move the camera
            float mouseSensitivityPercentage;
            bool invertYAxis;
            unsigned int sceneWidth, sceneHeight;
            double previousMouseX, previousMouseY;
    };

}
