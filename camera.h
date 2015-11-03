/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#ifndef Camera_h__
#define Camera_h__

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <string>
#include "gametimer.h"
#include "helper.h"
#include "gametimer.h"

class Camera {
public:
    Camera(float m_fieldOfView, glm::vec3 m_lookAt, glm::vec3 up, float m_nearPlane, float m_farPlane);
    Camera(glm::vec3 m_lookAt);
    Camera();

    enum CameraDirection {
        UP, DOWN, LEFT, RIGHT, FORWARD, BACKWARD
    };

    void CalculateFrustum();
    bool BoxWithinFrustum(const glm::vec3 &min, const glm::vec3 &max) const;
    void Update(const GameTimer &gt, bool update_frustum = false);

    void Reset();

    std::string getFullDebugDescription();

    const glm::mat4 &Projection() const;
    const glm::mat4 &View() const;
    const glm::mat4 &Model() const;
    const glm::mat4 &MVP() const;
    const glm::mat4 &VP() const;

    const glm::vec3 &LookAt() const;
    void LookAt(const glm::vec3 &value);

    const glm::vec3 &Position() const;
    void Position(const glm::vec3 &value);

    float Pitch() const;
    void Pitch(float value);

    float Yaw() const;
    void Yaw(float value);

    float Roll() const;
    void Roll(float value);

    float Zoom() const;
    void Zoom(float value);

    const glm::vec4 &Viewport() const;
    void Viewport(const glm::vec4 &value);

    glm::vec2 Project(const glm::vec3 &pos) const;
    sge::ray unProject(const glm::vec2 &pos) const;

    glm::vec3 Up() const;
    glm::vec3 Right() const;
    glm::vec3 Forward() const;

    float camera_scale = 1;

    void Move(CameraDirection dir);

    glm::vec3 camera_position_delta;
private:
    float m_pitch = 0;
    glm::vec4 m_viewport;

    float m_yaw = 0, m_roll = 0;

    float m_fieldOfView;

    float m_nearPlane;
    float m_farPlane;

    float MinZoom = 5;
    float MaxZoom = 100;
    float zoom = 30;
    glm::quat m_rotation_quaternion;

    glm::vec3 m_position;
    glm::vec3 m_lookAt;

    glm::vec3 m_camera_direction = glm::vec3(1, 0, 0),
              m_camera_position = glm::vec3(0),
              m_camera_up = glm::vec3(0, 1, 0),
              m_camera_right = glm::vec3(0),
              m_camera_look_at = glm::vec3(1, 0, 0);


    bool m_projection_matrix_dirty = true, m_view_matrix_dirty = true;
    glm::mat4 m_projection = glm::mat4(1), m_view = glm::mat4(1), m_model = glm::mat4(1), m_MVP = glm::mat4(1), m_VP = glm::mat4(1);

    enum FrustrumPlane {
        RIGHT_PLANE, LEFT_PLANE, BOTTOM_PLANE, TOP_PLANE, BACK_PLANE, FRONT_PLANE
    };
    float m_clipMatrix[16] = {};

    float m_frustum[6][4] = {};

    void ReCreateViewMatrix(const GameTimer &gt);
    void ReCreateProjectionMatrix();

};
#endif // Camera_h_
