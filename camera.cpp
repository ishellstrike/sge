#include "camera.h"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.inl>
#include <glm/gtx/quaternion.hpp>
#include <string>
#include "helper.inl"
#include <glm/gtc/quaternion.hpp>

Camera::Camera() {
    camera_mode = FREE;
    up = glm::vec3(0.0, 1.0, 0.0);
    field_of_view = 45;
    rotation_quaternion = glm::quat(1.0, 0.0, 0.0, 0.0);
    position_delta = glm::vec3(0.0, 0.0, 0.0);
    camera_scale = .5f;
    max_pitch_rate = 5;
    max_heading_rate = 5;
    move_camera = true;
    near_clip = 0.01f;
    far_clip = 1000;
    auto windowWidth = 1;
    auto windowHeight = 1;
    aspect = float(windowWidth) / float(windowHeight);
camera_heading = camera_pitch = 0;
}
Camera::~Camera() {
}

void Camera::Reset() {
    up = glm::vec3(0.0, 1.0, 0.0);
}

void Camera::Update() {
    auto yawq = glm::angleAxis(camera_pitch, glm::vec3(1,0,0));
    auto pitchq = glm::angleAxis(camera_heading, glm::vec3(0,1,0));

    projection = glm::perspective(field_of_view, aspect, near_clip, far_clip);

    rotation_quaternion = pitchq * yawq * rotation_quaternion;
    rotation_quaternion = glm::normalize(rotation_quaternion);

    position += position_delta;
    look_at = position + direction;


    camera_heading *= .5;
    camera_pitch *= .5;
    position_delta = position_delta * .8f;

    view = glm::translate(glm::mat4_cast(rotation_quaternion), -position);//glm::lookAt(position, look_at, up);
    up = glm::vec3(view[1][0],view[1][1],view[1][2]);
    direction = glm::vec3(view[0][0],view[0][1],view[0][2]);
    model = glm::mat4(1.0f);
    MVP = projection * view * model;
    //glLoadMatrixf(glm::value_ptr(MVP));
}

//Setting Functions
void Camera::SetMode(CameraType cam_mode) {
    camera_mode = cam_mode;
    up = glm::vec3(0.0, 1.0, 0.0);
    rotation_quaternion = glm::quat(1.0, 0.0, 0.0, 0.0);
}

void Camera::SetPosition(glm::vec3 pos) {
    position = pos;
}

void Camera::SetLookAt(glm::vec3 pos) {
    look_at = pos;
    rotation_quaternion = glm::quat_cast(glm::lookAt(position, pos, glm::vec3(0,0,1)));
}
void Camera::SetFOV(double fov) {
    field_of_view = fov;
}
void Camera::SetViewport(int loc_x, int loc_y, int width, int height) {
    viewport_x = loc_x;
    viewport_y = loc_y;
    window_width = width;
    window_height = height;
    //need to use doubles division here, it will not work otherwise and it is possible to get a zero aspect ratio with integer rounding
    aspect = double(width) / double(height);
    ;
}
void Camera::SetClipping(double near_clip_distance, double far_clip_distance) {
    near_clip = near_clip_distance;
    far_clip = far_clip_distance;
}

void Camera::Move(CameraDirection dir, GameTimer *gt) {
    if (camera_mode == FREE) {
        switch (dir) {
        case UP:
           position_delta += glm::vec3(0, -camera_scale, 0) * rotation_quaternion * (float) gt->elapsed;
            break;
        case DOWN:
            position_delta += glm::vec3(0, camera_scale, 0) * rotation_quaternion * (float) gt->elapsed;
            break;
        case LEFT:
           position_delta += glm::vec3(-camera_scale, 0, 0) * rotation_quaternion * (float) gt->elapsed;
            break;
        case RIGHT:
           position_delta += glm::vec3(camera_scale, 0, 0) * rotation_quaternion * (float) gt->elapsed;
            break;
        case FORWARD:
            position_delta += glm::vec3(0, 0, -camera_scale) * rotation_quaternion * (float) gt->elapsed;
            break;
        case BACK:
           position_delta += glm::vec3(0, 0, camera_scale) * rotation_quaternion * (float) gt->elapsed;
            break;
        }
    }
}
void Camera::ChangePitch(float degrees) {
    //Check bounds with the max pitch rate so that we aren't moving too fast
    if (degrees < -max_pitch_rate) {
        degrees = -max_pitch_rate;
    } else if (degrees > max_pitch_rate) {
        degrees = max_pitch_rate;
    }
    camera_pitch += degrees;

    //Check bounds for the camera pitch
    if (camera_pitch > 360.0f) {
        camera_pitch -= 360.0f;
    } else if (camera_pitch < -360.0f) {
        camera_pitch += 360.0f;
    }
}
void Camera::ChangeHeading(float degrees) {
    //Check bounds with the max heading rate so that we aren't moving too fast
    if (degrees < -max_heading_rate) {
        degrees = -max_heading_rate;
    } else if (degrees > max_heading_rate) {
        degrees = max_heading_rate;
    }
    //This controls how the heading is changed if the camera is pointed straight up or down
    //The heading delta direction changes
    if (camera_pitch > 90 && camera_pitch < 270 || (camera_pitch < -90 && camera_pitch > -270)) {
        camera_heading -= degrees;
    } else {
        camera_heading += degrees;
    }
    //Check bounds for the camera heading
    if (camera_heading > 360.0f) {
        camera_heading -= 360.0f;
    } else if (camera_heading < -360.0f) {
        camera_heading += 360.0f;
    }
}
void Camera::Move2D(int x, int y, GameTimer *gt) {
    //compute the mouse delta from the previous mouse position
    glm::vec3 mouse_delta = glm::vec3(x, y, 0) * 3.0F;
    //if the camera is moving, meaning that the mouse was clicked and dragged, change the pitch and heading
    if (move_camera) {
        ChangeHeading(0.08f * gt->elapsed * mouse_delta.x);
        ChangePitch(0.08f * gt->elapsed * mouse_delta.y);
    }
    //mouse_position = glm::vec3(x, y, 0);
}

void Camera::SetPos(int button, int state, int x, int y) {
    //if (button == 3 && state == GLUT_DOWN) {
    // position_delta += up * .05f;
    //} else if (button == 4 && state == GLUT_DOWN) {
    // position_delta -= up * .05f;
    //} else if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    // move_camera = true;
    //} else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
    // move_camera = false;
    //}
    //mouse_position = glm::vec3(x, y, 0);
}

Camera::CameraType Camera::GetMode() {
    return camera_mode;
}

void Camera::GetViewport(int &loc_x, int &loc_y, int &width, int &height) {
    loc_x = viewport_x;
    loc_y = viewport_y;
    width = window_width;
    height = window_height;
}

void Camera::GetMatricies(glm::mat4 &P, glm::mat4 &V, glm::mat4 &M) const {
    P = projection;
    V = view;
    M = model;
}

std::string Camera::getFullDebugDescription()
{
    return string_format("CAMERA\n%s\nheading = %s pitch = %s\nup = %s\ndir = %s\ncross = %s\n\n"
                         "%s",
        std::to_string(view).c_str(),
        std::to_string(camera_heading).c_str(),
        std::to_string(camera_pitch).c_str(),
        std::to_string(up).c_str(),
        std::to_string(direction).c_str(),
        std::to_string(glm::cross(direction, up)).c_str(),
        std::to_string(m_frustum).c_str());
}

void NormalizePlane(float frustum[6][4], int side) {
    float magnitude = ( float )sqrt( ( frustum[side][0] * frustum[side][0] ) + ( frustum[side][1] * frustum[side][1] )
                                        + ( frustum[side][2] * frustum[side][2] ) );
    frustum[side][0] /= magnitude;
    frustum[side][1] /= magnitude;
    frustum[side][2] /= magnitude;
    frustum[side][3] /= magnitude;
}

void Camera::CalculateFrustum( const glm::mat4 &projectionMatrix, const glm::mat4 &modelViewMatrix) {
    const glm::mat4 &MV = modelViewMatrix;
    const glm::mat4 &P = projectionMatrix;

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

glm::mat4 Camera::VP() const
{
    return projection*view;
}

glm::mat4 Camera::GetOrthoProjection()
{
    auto orthoProjection = glm::ortho(0.0f, (float)window_width, (float)window_height, 0.0f, 0.1f, 100.0f);
    return orthoProjection;
}

const glm::mat4 OrthoProjection(float left, float right,
                           float bottom, float top, float zNear, float zFar)
{
    const float tx = - (right + left) / (right - left),
        ty = - (top + bottom) / (top - bottom),
        tz = - (zFar + zNear) / (zFar - zNear);

    return glm::mat4(2 / (right - left), 0, 0, tx,
        0, 2 / (top - bottom), 0, ty,
        0, 0, -2 / (zFar - zNear), tz,
        0, 0, 0, 1);
}
