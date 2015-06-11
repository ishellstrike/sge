#include "camera.h"
#include <glm/gtc/matrix_transform.inl>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/quaternion.hpp>
#include <string>
#include "helper.h"
#include <glm/gtc/quaternion.hpp>
#include "prefecences.h"

Camera::Camera(glm::vec3 __lookAt) :
    fieldOfView(glm::quarter_pi<float>()),
    lookAt(__lookAt),
    nearPlane(0.1f),
    farPlane(10000)
{

}

Camera::Camera() :
       fieldOfView(glm::quarter_pi<float>()),
       lookAt(glm::vec3(0,0,0)),
       nearPlane(0.1f),
       farPlane(10000)
{

}

Camera::Camera(float __fieldOfView, glm::vec3 __lookAt, glm::vec3 __up, float __nearPlane, float __farPlane) :
    fieldOfView(__fieldOfView),
    lookAt(__lookAt),
    nearPlane(__nearPlane),
    farPlane(__farPlane)
{
}

void Camera::ReCreateViewMatrix(const GameTimer &gt) {
    auto yawq = glm::angleAxis(yaw, glm::vec3(0,1,0));
    auto pitchq = glm::angleAxis(pitch, glm::vec3(1,0,0));
    auto rollq = glm::angleAxis(roll, glm::vec3(0,0,1));

    rotation_quaternion = rollq * pitchq * yawq * rotation_quaternion;
    rotation_quaternion = normalize(rotation_quaternion);

    position += camera_position_delta;
    camera_look_at = position + camera_direction;


    yaw *= 0;
    pitch *= 0;
    roll *= 0;
    camera_position_delta = camera_position_delta * 0.f;

    view = translate(mat4_cast(rotation_quaternion), -position);//glm::lookAt(position, look_at, up);
    camera_up = glm::vec3(view[1][0],view[1][1],view[1][2]);
    camera_direction = glm::vec3(view[0][0],view[0][1],view[0][2]);
    MVP = projection * view * model;

    if(camera_position_delta.x + camera_position_delta.y + camera_position_delta.z + pitch + yaw < 0.01)
        viewMatrixDirty = false;
}

void Camera::Move(CameraDirection dir) {
    switch (dir) {
    case UP:
       camera_position_delta += glm::vec3(0, -camera_scale, 0) * rotation_quaternion;
        break;
    case DOWN:
        camera_position_delta += glm::vec3(0, camera_scale, 0) * rotation_quaternion;
        break;
    case LEFT:
       camera_position_delta += glm::vec3(-camera_scale, 0, 0) * rotation_quaternion;
        break;
    case RIGHT:
       camera_position_delta += glm::vec3(camera_scale, 0, 0) * rotation_quaternion;
        break;
    case FORWARD:
        camera_position_delta += glm::vec3(0, 0, -camera_scale) * rotation_quaternion;
        break;
    case BACK:
       camera_position_delta += glm::vec3(0, 0, camera_scale) * rotation_quaternion;
        break;
    }
    viewMatrixDirty = true;
}

void Camera::ReCreateProjectionMatrix()
{
    projection = glm::perspectiveFov(fieldOfView, Prefecences::Instance()->resolution.x, Prefecences::Instance()->resolution.y, nearPlane, farPlane);
    projectionMatrixDirty = false;
}

void Camera::SetPosition(const glm::vec3 &_p)
{
    position = _p;
    viewMatrixDirty = true;
}

void Camera::SetLookAt(const glm::vec3 &_p)
{
    lookAt = _p;
    viewMatrixDirty = true;
}

std::string Camera::getFullDebugDescription()
{
    return std::to_string(m_frustum);
}

void Camera::SetViewport(const glm::vec4 &_p)
{
    viewport = _p;
    projectionMatrixDirty = true;
}

void Camera::Reset()
{
    yaw = 0;
    pitch = 0.001f;
    zoom = 30;
    viewMatrixDirty = true;
}
const glm::mat4 &Camera::getProjection() const
{
    return projection;
}
const glm::mat4 &Camera::getView() const
{
    return view;
}
const glm::mat4 &Camera::getModel() const
{
    return model;
}
const glm::mat4 &Camera::getMVP() const
{
    return MVP;
}
const glm::mat4 &Camera::getVP() const
{
    return VP;
}

const glm::vec3 &Camera::getLookAt() const
{
    return lookAt;
}
void Camera::setLookAt(const glm::vec3 &value)
{
    lookAt = value;
    viewMatrixDirty = true;
}

const glm::vec3 &Camera::getPosition() const
{
    return position;
}
void Camera::setPosition(const glm::vec3 &value)
{
    position = value;
    viewMatrixDirty = true;
}

float Camera::getPitch() const
{
    return pitch;
}
void Camera::setPitch(float degrees)
{
    if(degrees == 0) return;
    degrees = degrees/1000.f;

    if (degrees < -max_pitch_rate) {
        degrees = -max_pitch_rate;
    } else if (degrees > max_pitch_rate) {
        degrees = max_pitch_rate;
    }
    pitch += degrees;

    if (pitch > glm::two_pi<float>()) {
        pitch -= glm::two_pi<float>();
    } else if (pitch < -glm::two_pi<float>()) {
        pitch += glm::two_pi<float>();
    }
}

float Camera::getYaw() const
{
    return yaw;
}
void Camera::setYaw(float degrees)
{
    if(degrees == 0) return;
    degrees = degrees/1000.f;
    if (degrees < -max_yaw_rate) {
        degrees = -max_yaw_rate;
    } else if (degrees > max_yaw_rate) {
        degrees = max_yaw_rate;
    }

    if (pitch > glm::half_pi<float>() && pitch < glm::three_over_two_pi<float>() ||
            (pitch < -glm::half_pi<float>() && pitch > -glm::three_over_two_pi<float>())) {
        yaw -= degrees;
    } else {
        yaw += degrees;
    }

    if (yaw > glm::two_pi<float>()) {
        yaw -= glm::two_pi<float>();
    } else if (yaw < -glm::two_pi<float>()) {
        yaw += glm::two_pi<float>();
    }
    viewMatrixDirty = true;
}

float Camera::getRoll() const
{
    return roll;
}

void Camera::setRoll(float value)
{
    roll = value;
    viewMatrixDirty = true;
}

float Camera::getZoom() const
{
    return zoom;
}

void Camera::setZoom(float value)
{
    zoom = value;
    viewMatrixDirty = true;
}

const glm::vec4 &Camera::getViewport() const
{
    return viewport;
}
void Camera::setViewport(const glm::vec4 &value)
{
    viewport = value;
    projectionMatrixDirty = true;
}

glm::vec2 Camera::Project(const glm::vec3 pos)
{
    glm::vec3 v = glm::project(pos, getModel() * getView(), getProjection(), getViewport());
    //v /= v.z;
    return glm::vec2(v.x, RESY - v.y);
}

glm::ray Camera::unProject(const glm::vec2 pos)
{
    glm::vec3 near = glm::unProject(glm::vec3(pos.x, RESY-pos.y, 0.f),  getModel() * getView(), getProjection(),
                                    getViewport());
    glm::vec3 far = glm::unProject(glm::vec3(pos.x, RESY-pos.y, 1.f),  getModel() * getView(), getProjection(),
                                    getViewport());
    glm::ray ray(near, far - near);
    return ray;
}


void Camera::Update(const GameTimer &gt)
{
    bool mvp_recalc = false;
    if(projectionMatrixDirty)
    {
        ReCreateProjectionMatrix();
        mvp_recalc = true;
    }
    if(viewMatrixDirty)
    {
        ReCreateViewMatrix(gt);
        mvp_recalc = true;
    }

    if(mvp_recalc)
    {
        MVP = projection * view * model;
        VP = projection*view;
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
    const glm::mat4 &MV = view * model;
    const glm::mat4 &P = projection;

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
