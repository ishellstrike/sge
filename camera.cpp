#include "camera.h"
#include <glm/gtc/matrix_transform.inl>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/quaternion.hpp>
#include <string>
#include "helper.h"
#include <glm/gtc/quaternion.hpp>
#include "prefecences.h"
#include "logger.h"

Camera::Camera(glm::vec3 __lookAt) :
    m_fieldOfView(glm::quarter_pi<float>()),
    m_lookAt(__lookAt),
    m_nearPlane(0.001f),
    m_farPlane(1000)
{

}

Camera::Camera() :
       m_fieldOfView(glm::quarter_pi<float>()),
       m_lookAt(glm::vec3(0,0,0)),
       m_nearPlane(0.001f),
       m_farPlane(1000)
{
}

Camera::Camera(float __fieldOfView, glm::vec3 __lookAt, glm::vec3 /*__up*/, float __nearPlane, float __farPlane) :
    m_fieldOfView(__fieldOfView),
    m_lookAt(__lookAt),
    m_nearPlane(__nearPlane),
    m_farPlane(__farPlane)
{
}

void Camera::ReCreateViewMatrix(const GameTimer &gt) {
    const auto &yawq = glm::angleAxis(m_yaw, glm::vec3(0,1,0));
    const auto &pitchq = glm::angleAxis(m_pitch, glm::vec3(1,0,0));
    const auto &rollq = glm::angleAxis(m_roll, glm::vec3(0,0,1));

    m_rotation_quaternion = rollq * pitchq * yawq * m_rotation_quaternion;
    m_rotation_quaternion = normalize(m_rotation_quaternion);

    m_position += camera_position_delta;
    m_camera_look_at = m_position + m_camera_direction;


    m_yaw *= 0.5;
    m_pitch *= 0.5;
    m_roll *= 0.5;
    camera_position_delta = camera_position_delta * 0.f;

    m_view = translate(mat4_cast(m_rotation_quaternion), -m_position);//glm::lookAt(position, look_at, up);
    m_camera_up = glm::vec3(m_view[1][0],m_view[1][1],m_view[1][2]);
    m_camera_direction = glm::vec3(m_view[0][0],m_view[0][1],m_view[0][2]);
    m_MVP = m_projection * m_view * m_model;

    if(camera_position_delta.x + camera_position_delta.y + camera_position_delta.z + m_pitch + m_yaw < 0.01)
        m_view_matrix_dirty = false;
}

void Camera::Move(CameraDirection dir) {
    switch (dir) {
    case UP:
       camera_position_delta += glm::vec3(0, -camera_scale, 0) * m_rotation_quaternion;
        break;
    case DOWN:
        camera_position_delta += glm::vec3(0, camera_scale, 0) * m_rotation_quaternion;
        break;
    case LEFT:
       camera_position_delta += glm::vec3(-camera_scale, 0, 0) * m_rotation_quaternion;
        break;
    case RIGHT:
       camera_position_delta += glm::vec3(camera_scale, 0, 0) * m_rotation_quaternion;
        break;
    case FORWARD:
        camera_position_delta += glm::vec3(0, 0, -camera_scale) * m_rotation_quaternion;
        break;
    case BACKWARD:
       camera_position_delta += glm::vec3(0, 0, camera_scale) * m_rotation_quaternion;
        break;
    }
    m_view_matrix_dirty = true;
}

void Camera::ReCreateProjectionMatrix()
{
    m_projection = glm::perspectiveFov(m_fieldOfView, RESX_float, RESY_float, m_nearPlane, m_farPlane);
    m_projection_matrix_dirty = false;
}

std::string Camera::getFullDebugDescription()
{
    return std::to_string(m_frustum);
}

void Camera::Reset()
{
    m_yaw = 0;
    m_pitch = 0.001f;
    zoom = 30;
    m_view_matrix_dirty = true;
    m_projection_matrix_dirty = true;
}
const glm::mat4 &Camera::Projection() const
{
    return m_projection;
}
const glm::mat4 &Camera::View() const
{
    return m_view;
}
const glm::mat4 &Camera::Model() const
{
    return m_model;
}
const glm::mat4 &Camera::MVP() const
{
    return m_MVP;
}
const glm::mat4 &Camera::VP() const
{
    return m_VP;
}

const glm::vec3 &Camera::LookAt() const
{
    return m_lookAt;
}
void Camera::LookAt(const glm::vec3 &value)
{
    m_lookAt = value;
    m_view_matrix_dirty = true;
}

const glm::vec3 &Camera::Position() const
{
    return m_position;
}
void Camera::Position(const glm::vec3 &value)
{
    m_position = value;
    m_view_matrix_dirty = true;
}

float Camera::Pitch() const
{
    return m_pitch;
}
void Camera::Pitch(float degrees)
{
    if(degrees == 0) return;
    degrees = degrees/1000.f;

    if (degrees < -max_pitch_rate) {
        degrees = -max_pitch_rate;
    } else if (degrees > max_pitch_rate) {
        degrees = max_pitch_rate;
    }
    m_pitch += degrees;

    if (m_pitch > glm::two_pi<float>()) {
        m_pitch -= glm::two_pi<float>();
    } else if (m_pitch < -glm::two_pi<float>()) {
        m_pitch += glm::two_pi<float>();
    }
}

float Camera::Yaw() const
{
    return m_yaw;
}
void Camera::Yaw(float degrees)
{
    if(degrees == 0) return;
    degrees = degrees/1000.f;
    if (degrees < -max_yaw_rate) {
        degrees = -max_yaw_rate;
    } else if (degrees > max_yaw_rate) {
        degrees = max_yaw_rate;
    }

    if (m_pitch > glm::half_pi<float>() && m_pitch < glm::three_over_two_pi<float>() ||
            (m_pitch < -glm::half_pi<float>() && m_pitch > -glm::three_over_two_pi<float>())) {
        m_yaw -= degrees;
    } else {
        m_yaw += degrees;
    }

    if (m_yaw > glm::two_pi<float>()) {
        m_yaw -= glm::two_pi<float>();
    } else if (m_yaw < -glm::two_pi<float>()) {
        m_yaw += glm::two_pi<float>();
    }
    m_view_matrix_dirty = true;
}

float Camera::Roll() const
{
    return m_roll;
}

void Camera::Roll(float value)
{
    m_roll = value;
    m_view_matrix_dirty = true;
}

float Camera::Zoom() const
{
    return zoom;
}

void Camera::Zoom(float value)
{
    zoom = value;
    m_view_matrix_dirty = true;
}

const glm::vec4 &Camera::Viewport() const
{
    return m_viewport;
}
void Camera::Viewport(const glm::vec4 &value)
{
    m_viewport = value;
    m_projection_matrix_dirty = true;
}

glm::vec2 Camera::Project(const glm::vec3 &pos) const
{
    glm::vec3 v = glm::project(pos, Model() * View(), Projection(), Viewport());
    //v /= v.z;
    return glm::vec2(v.x, RESY - v.y);
}

glm::ray Camera::unProject(const glm::vec2 pos) const
{
    glm::vec3 near = glm::unProject(glm::vec3(pos.x, RESY-pos.y, 0.f),  Model() * View(), Projection(),
                                    Viewport());
    glm::vec3 far = glm::unProject(glm::vec3(pos.x, RESY-pos.y, 1.f),  Model() * View(), Projection(),
                                    Viewport());
    glm::ray ray(near, far - near);
    return ray;
}


void Camera::Update(const GameTimer &gt)
{
    bool mvp_recalc = false;
    if(m_projection_matrix_dirty)
    {
        ReCreateProjectionMatrix();
        mvp_recalc = true;
    }
    if(m_view_matrix_dirty)
    {
        ReCreateViewMatrix(gt);
        mvp_recalc = true;
    }

    if(mvp_recalc)
    {
        m_MVP = m_projection * m_view * m_model;
        m_VP = m_projection*m_view;
    }
}

void NormalizePlane(float frustum[6][4], int side) {
    float magnitude = (float)sqrt((frustum[side][0] * frustum[side][0]) +
                                  (frustum[side][1] * frustum[side][1]) +
                                  (frustum[side][2] * frustum[side][2]));
    frustum[side][0] /= magnitude;
    frustum[side][1] /= magnitude;
    frustum[side][2] /= magnitude;
    frustum[side][3] /= magnitude;
}

void Camera::CalculateFrustum() {
    const glm::mat4 &MV = m_view * m_model;
    const glm::mat4 &P = m_projection;

    m_clipMatrix[0]  = (MV[0][0] * P[0][0]) + (MV[0][1] * P[1][0]) + (MV[0][2] * P[2][0]) + (MV[0][3] * P[3][0]);
    m_clipMatrix[1]  = (MV[0][0] * P[0][1]) + (MV[0][1] * P[1][1]) + (MV[0][2] * P[2][1]) + (MV[0][3] * P[3][1]);
    m_clipMatrix[2]  = (MV[0][0] * P[0][2]) + (MV[0][1] * P[1][2]) + (MV[0][2] * P[2][2]) + (MV[0][3] * P[3][2]);
    m_clipMatrix[3]  = (MV[0][0] * P[0][3]) + (MV[0][1] * P[1][3]) + (MV[0][2] * P[2][3]) + (MV[0][3] * P[3][3]);

    m_clipMatrix[4]  = (MV[1][0] * P[0][0]) + (MV[1][1] * P[1][0]) + (MV[1][2] * P[2][0]) + (MV[1][3] * P[3][0]);
    m_clipMatrix[5]  = (MV[1][0] * P[0][1]) + (MV[1][1] * P[1][1]) + (MV[1][2] * P[2][1]) + (MV[1][3] * P[3][1]);
    m_clipMatrix[6]  = (MV[1][0] * P[0][2]) + (MV[1][1] * P[1][2]) + (MV[1][2] * P[2][2]) + (MV[1][3] * P[3][2]);
    m_clipMatrix[7]  = (MV[1][0] * P[0][3]) + (MV[1][1] * P[1][3]) + (MV[1][2] * P[2][3]) + (MV[1][3] * P[3][3]);

    m_clipMatrix[8]  = (MV[2][0] * P[0][0]) + (MV[2][1] * P[1][0]) + (MV[2][2] * P[2][0]) + (MV[2][3] * P[3][0]);
    m_clipMatrix[9]  = (MV[2][0] * P[0][1]) + (MV[2][1] * P[1][1]) + (MV[2][2] * P[2][1]) + (MV[2][3] * P[3][1]);
    m_clipMatrix[10] = (MV[2][0] * P[0][2]) + (MV[2][1] * P[1][2]) + (MV[2][2] * P[2][2]) + (MV[2][3] * P[3][2]);
    m_clipMatrix[11] = (MV[2][0] * P[0][3]) + (MV[2][1] * P[1][3]) + (MV[2][2] * P[2][3]) + (MV[2][3] * P[3][3]);

    m_clipMatrix[12] = (MV[3][0] * P[0][0]) + (MV[3][1] * P[1][0]) + (MV[3][2] * P[2][0]) + (MV[3][3] * P[3][0]);
    m_clipMatrix[13] = (MV[3][0] * P[0][1]) + (MV[3][1] * P[1][1]) + (MV[3][2] * P[2][1]) + (MV[3][3] * P[3][1]);
    m_clipMatrix[14] = (MV[3][0] * P[0][2]) + (MV[3][1] * P[1][2]) + (MV[3][2] * P[2][2]) + (MV[3][3] * P[3][2]);
    m_clipMatrix[15] = (MV[3][0] * P[0][3]) + (MV[3][1] * P[1][3]) + (MV[3][2] * P[2][3]) + (MV[3][3] * P[3][3]);

    m_frustum[RIGHT_PLANE][0] = m_clipMatrix[3]  - m_clipMatrix[0];
    m_frustum[RIGHT_PLANE][1] = m_clipMatrix[7]  - m_clipMatrix[4];
    m_frustum[RIGHT_PLANE][2] = m_clipMatrix[11] - m_clipMatrix[8];
    m_frustum[RIGHT_PLANE][3] = m_clipMatrix[15] - m_clipMatrix[12];
    NormalizePlane(m_frustum, RIGHT_PLANE);

    m_frustum[LEFT_PLANE][0] = m_clipMatrix[3]  + m_clipMatrix[0];
    m_frustum[LEFT_PLANE][1] = m_clipMatrix[7]  + m_clipMatrix[4];
    m_frustum[LEFT_PLANE][2] = m_clipMatrix[11] + m_clipMatrix[8];
    m_frustum[LEFT_PLANE][3] = m_clipMatrix[15] + m_clipMatrix[12];
    NormalizePlane(m_frustum, LEFT_PLANE);

    m_frustum[BOTTOM_PLANE][0] = m_clipMatrix[3]  + m_clipMatrix[1];
    m_frustum[BOTTOM_PLANE][1] = m_clipMatrix[7]  + m_clipMatrix[5];
    m_frustum[BOTTOM_PLANE][2] = m_clipMatrix[11] + m_clipMatrix[9];
    m_frustum[BOTTOM_PLANE][3] = m_clipMatrix[15] + m_clipMatrix[13];
    NormalizePlane(m_frustum, BOTTOM_PLANE);

    m_frustum[TOP_PLANE][0] = m_clipMatrix[3]  - m_clipMatrix[1];
    m_frustum[TOP_PLANE][1] = m_clipMatrix[7]  - m_clipMatrix[5];
    m_frustum[TOP_PLANE][2] = m_clipMatrix[11] - m_clipMatrix[9];
    m_frustum[TOP_PLANE][3] = m_clipMatrix[15] - m_clipMatrix[13];
    NormalizePlane(m_frustum, TOP_PLANE);

    m_frustum[BACK_PLANE][0] = m_clipMatrix[3]  - m_clipMatrix[2];
    m_frustum[BACK_PLANE][1] = m_clipMatrix[7]  - m_clipMatrix[6];
    m_frustum[BACK_PLANE][2] = m_clipMatrix[11] - m_clipMatrix[10];
    m_frustum[BACK_PLANE][3] = m_clipMatrix[15] - m_clipMatrix[14];
    NormalizePlane(m_frustum, BACK_PLANE);

    m_frustum[FRONT_PLANE][0] = m_clipMatrix[3]  + m_clipMatrix[2];
    m_frustum[FRONT_PLANE][1] = m_clipMatrix[7]  + m_clipMatrix[6];
    m_frustum[FRONT_PLANE][2] = m_clipMatrix[11] + m_clipMatrix[10];
    m_frustum[FRONT_PLANE][3] = m_clipMatrix[15] + m_clipMatrix[14];
    NormalizePlane(m_frustum, FRONT_PLANE);
}

bool Camera::BoxWithinFrustum(const glm::vec3 &min, const glm::vec3 &max) {
    for(int i = 0; i < 6; i++) {
        if((m_frustum[i][0] * min.x + m_frustum[i][1] * min.y + m_frustum[i][2] * min.z + m_frustum[i][3] <= 0.0F) &&
           (m_frustum[i][0] * max.x + m_frustum[i][1] * min.y + m_frustum[i][2] * min.z + m_frustum[i][3] <= 0.0F) &&
           (m_frustum[i][0] * min.x + m_frustum[i][1] * max.y + m_frustum[i][2] * min.z + m_frustum[i][3] <= 0.0F) &&
           (m_frustum[i][0] * max.x + m_frustum[i][1] * max.y + m_frustum[i][2] * min.z + m_frustum[i][3] <= 0.0F) &&
           (m_frustum[i][0] * min.x + m_frustum[i][1] * min.y + m_frustum[i][2] * max.z + m_frustum[i][3] <= 0.0F) &&
           (m_frustum[i][0] * max.x + m_frustum[i][1] * min.y + m_frustum[i][2] * max.z + m_frustum[i][3] <= 0.0F) &&
           (m_frustum[i][0] * min.x + m_frustum[i][1] * max.y + m_frustum[i][2] * max.z + m_frustum[i][3] <= 0.0F) &&
           (m_frustum[i][0] * max.x + m_frustum[i][1] * max.y + m_frustum[i][2] * max.z + m_frustum[i][3] <= 0.0F))
        {
              return false;
        }
    }
    return true;
}
