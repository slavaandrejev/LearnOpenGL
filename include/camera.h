#pragma once

#include <cmath>

#include <cstdint>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
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

        yaw   = lastYaw + offset.x;
        pitch = lastPitch - offset.y;
        if (89.0f < pitch) {
            pitch = 89.0f;
        }
        if (-89.0f > pitch) {
            pitch = -89.0f;
        }
        frontDir = glm::normalize(glm::vec3{
            std::cos(glm::radians(yaw)) * std::cos(glm::radians(pitch))
          , std::sin(glm::radians(pitch))
          , std::sin(glm::radians(yaw)) * std::cos(glm::radians(pitch))
        });
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

        bool IsMoving() const {
            return 0 != forward || 0 != back || 0 != left || 0 != right;
        }
    };
    MoveDirs moveDirs;

    void UpdateCameraMoveDir() {
        speedDir =
            float(moveDirs.forward) * frontDir
          - float(moveDirs.back) * frontDir
          + float(moveDirs.right) * glm::normalize(glm::cross(frontDir, upDir))
          - float(moveDirs.left) * glm::normalize(glm::cross(frontDir, upDir));
    }

    float zoom = 45.0f;
    glm::vec3 pos      = glm::vec3(0.0f, 0.0f,  3.0f);
    glm::vec3 frontDir = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 upDir    = glm::vec3(0.0f, 1.0f,  0.0f);

    float speed = 0.05f;
    glm::vec3 speedDir = glm::vec3(0.0f, 0.0f, 0.0f);

    static constexpr float mouseSensitivity = 0.1f;
    glm::dvec2 startMousepos;
    float yaw       = -90.0f;
    float pitch     = 0.0f;
    float lastYaw   = -90.0f;
    float lastPitch = 0.0f;
};
