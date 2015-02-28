#pragma once
#ifndef Camera_h__
#define Camera_h__

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <string>
#include "gametimer.h"


class Camera {
public:
    enum FrustrumPlane {
        RIGHT_PLANE, LEFT_PLANE, BOTTOM_PLANE, TOP_PLANE, BACK_PLANE, FRONT_PLANE
    };
    float m_clipMatrix[16];
    float m_frustum[6][4];

    Camera(float aspectRatio, float fieldOfView, glm::vec3 lookAt, glm::vec3 up, float nearPlane, float farPlane);
    Camera(float aspectRation, glm::vec3 lookAt);
    Camera();

    glm::vec3 Up, Forward, Backward, Left, Right;
    bool projectionMatrixDirty = true, viewMatrixDirty = true;
    glm::mat4 projection = glm::mat4(1), view = glm::mat4(1), model = glm::mat4(1), MVP = glm::mat4(1), VP = glm::mat4(1);

    float MinPitch = 0.001f;
    float MaxPitch = glm::half_pi<float>() - 0.3f;
    float pitch = 0.001f;
    glm::vec4 viewport;

    float yaw = 0, roll = 0;

    float fieldOfView;
    float aspectRatio;

    float nearPlane;
    float farPlane;

    float MinZoom = 5;
    float MaxZoom = 100;
    float zoom = 30;

    glm::vec3 position;
    glm::vec3 lookAt;

    void CalculateFrustum(const glm::mat4 &projectionMatrix, const glm::mat4 &modelViewMatrix);
    bool BoxWithinFrustum(const glm::vec3 &min, const glm::vec3 &max);
    void Update();
    void ReCreateViewMatrix();
    void ReCreateProjectionMatrix();
    void SetPosition(const glm::vec3 &_p);
    void SetLookAt(const glm::vec3 &_p);
    std::string getFullDebugDescription();
    void SetViewport(const glm::vec4 &_p);
    void Reset();
};
#endif // Camera_h_
