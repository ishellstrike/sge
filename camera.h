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
    Camera(float fieldOfView, glm::vec3 lookAt, glm::vec3 up, float nearPlane, float farPlane);
    Camera(glm::vec3 lookAt);
    Camera();

    enum CameraDirection {
        UP, DOWN, LEFT, RIGHT, FORWARD, BACK
    };

    void CalculateFrustum();
    bool BoxWithinFrustum(const glm::vec3 &min, const glm::vec3 &max);
    void Update(const GameTimer &gt);
    void ReCreateViewMatrix(const GameTimer &gt);
    void ReCreateProjectionMatrix();
    void SetPosition(const glm::vec3 &_p);
    void SetLookAt(const glm::vec3 &_p);
    std::string getFullDebugDescription();
    void SetViewport(const glm::vec4 &_p);
    void Reset();

    const glm::mat4 &getProjection() const;
    const glm::mat4 &getView() const;
    const glm::mat4 &getModel() const;
    const glm::mat4 &getMVP() const;
    const glm::mat4 &getVP() const;

    const glm::vec3 &getLookAt() const;
    void setLookAt(const glm::vec3 &value);

    const glm::vec3 &getPosition() const;
    void setPosition(const glm::vec3 &value);

    float getPitch() const;
    void setPitch(float value);

    float getYaw() const;
    void setYaw(float value);

    float getRoll() const;
    void setRoll(float value);

    float getZoom() const;
    void setZoom(float value);

    const glm::vec4 &getViewport() const;
    void setViewport(const glm::vec4 &value);

    glm::vec3 Up, Forward, Backward, Left, Right;

    glm::vec2 Project(const glm::vec3 pos);
    glm::ray unProject(const glm::vec2 pos);

    float camera_scale = 1;

    void Move(CameraDirection dir);

    glm::vec3 camera_position_delta;
private:
    float pitch = 0;
    glm::vec4 viewport;

    float yaw = 0, roll = 0;

    float fieldOfView;

    float nearPlane;
    float farPlane;

    float MinZoom = 5;
    float MaxZoom = 100;
    float zoom = 30, max_pitch_rate = 5, max_yaw_rate = 5;
    glm::quat rotation_quaternion;

    glm::vec3 position;
    glm::vec3 lookAt;

    glm::vec3 camera_direction = glm::vec3(1, 0, 0), camera_position, camera_up = glm::vec3(0, 1, 0), camera_look_at = glm::vec3(1, 0, 0);


    bool projectionMatrixDirty = true, viewMatrixDirty = true;
    glm::mat4 projection = glm::mat4(1), view = glm::mat4(1), model = glm::mat4(1), MVP = glm::mat4(1), VP = glm::mat4(1);

    enum FrustrumPlane {
        RIGHT_PLANE, LEFT_PLANE, BOTTOM_PLANE, TOP_PLANE, BACK_PLANE, FRONT_PLANE
    };
    float m_clipMatrix[16];
    float m_frustum[6][4];

};
#endif // Camera_h_
