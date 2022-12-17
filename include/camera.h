#pragma once

#include <cmath>

#include <cstdint>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

class Camera {
public:
    Camera(
        const glm::vec3 &position = {0.0f, 0.0f, 3.0f}
      , const glm::vec3 &up = {0.0f, 1.0f, 0.0f}
      , const glm::vec3 &front = {0.0f, 0.0f, -1.0f}
      )
      : pos(position)
      , frontDir(front)
      , upDir(up)
    {
        frontDir = glm::normalize(frontDir);
        upDir = glm::normalize(upDir);

        // e0, e1, and up form an ONB of the initial camera position, with e0
        // looking forward, but perpendicular to up. Yaw and pitch angles are
        // relative to e0, e1 plane.
        e1 = glm::normalize(glm::cross(upDir, frontDir));
        e0 = glm::normalize(glm::cross(e1, upDir));
    }

    void StartForward() {
        moveDirs.forward = 1;
        UpdateCameraMoveDir();
    }

    void StartBack() {
        moveDirs.back = 1;
        UpdateCameraMoveDir();
    }

    void StartLeft() {
        moveDirs.left = 1;
        UpdateCameraMoveDir();
    }

    void StartRight() {
        moveDirs.right = 1;
        UpdateCameraMoveDir();
    }

    void StartUp() {
        moveDirs.up = 1;
        UpdateCameraMoveDir();
    }

    void StartDown() {
        moveDirs.down = 1;
        UpdateCameraMoveDir();
    }

    void StopForward() {
        moveDirs.forward = 0;
        UpdateCameraMoveDir();
    }

    void StopBack() {
        moveDirs.back = 0;
        UpdateCameraMoveDir();
    }

    void StopLeft() {
        moveDirs.left = 0;
        UpdateCameraMoveDir();
    }

    void StopRight() {
        moveDirs.right = 0;
        UpdateCameraMoveDir();
    }

    void StopUp() {
        moveDirs.up = 0;
        UpdateCameraMoveDir();
    }

    void StopDown() {
        moveDirs.down = 0;
        UpdateCameraMoveDir();
    }

    void TimeTick(float deltaTime) {
        if (moveDirs.IsMoving()) {
            pos += deltaTime * speed * speedDir;
        }
    }

    bool IsMoving() const {
        return moveDirs.IsMoving();
    }

    void OnPointerEnter(double x, double y) {
        startMousepos = {x, y};
    }

    void OnPointerMotion(double x, double y) {
        auto offset = double(mouseSensitivity) * (glm::dvec2{x, y} - startMousepos);

        yaw   = lastYaw - offset.x;
        pitch = lastPitch - offset.y;
        if (89.0f < pitch) {
            pitch = 89.0f;
        }
        if (-89.0f > pitch) {
            pitch = -89.0f;
        }
        frontDir = glm::rotate(e0, -glm::radians(pitch), e1);
        frontDir = glm::rotate(frontDir, glm::radians(yaw), upDir);
    }

    void OnPointerLeave() {
        lastYaw = yaw;
        lastPitch = pitch;
    }

    void OnScroll(double yoffset) {
        zoom -= yoffset;

        if (1.0f > zoom) {
            zoom = 1.0f;
        }

        if (45.0f < zoom) {
            zoom = 45.0f;
        }
    }

    auto GetViewMatrix() const {
        return glm::lookAt(pos, pos + frontDir, upDir);
    }

    auto GetZoom() const {
        return zoom;
    }

private:
    struct MoveDirs {
        uint8_t forward:1 = 0;
        uint8_t back:1    = 0;
        uint8_t left:1    = 0;
        uint8_t right:1   = 0;
        uint8_t up:1      = 0;
        uint8_t down:1    = 0;

        bool IsMoving() const {
            return
                0 != forward
             || 0 != back
             || 0 != left
             || 0 != right
             || 0 != up
             || 0 != down;
        }
    };
    MoveDirs moveDirs;

    void UpdateCameraMoveDir() {
        speedDir =
            float(moveDirs.forward) * frontDir
          - float(moveDirs.back) * frontDir
          + float(moveDirs.right) * glm::normalize(glm::cross(frontDir, upDir))
          - float(moveDirs.left) * glm::normalize(glm::cross(frontDir, upDir))
          + float(moveDirs.up) * upDir
          - float(moveDirs.down) * upDir;
}

    float zoom = 45.0f;
    glm::vec3 pos;
    glm::vec3 frontDir;
    glm::vec3 upDir;

    glm::vec3 e0;
    glm::vec3 e1;

    float speed = 0.05f;
    glm::vec3 speedDir = glm::vec3(0.0f, 0.0f, 0.0f);

    static constexpr float mouseSensitivity = 0.1f;
    glm::dvec2 startMousepos;
    float yaw       = 0.0f;
    float pitch     = 0.0f;
    float lastYaw   = 0.0f;
    float lastPitch = 0.0f;
};
