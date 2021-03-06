#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

enum class CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

struct CameraVectors
{
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 Front;

    glm::vec3 WorldUp;
};

class Camera
{
public:
    // euler Angles
    float Yaw;
    float Pitch;

    // camera options
    float MovementSpeed;
    float MouseSensitivity;

    // constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 GetViewMatrix() { return glm::lookAt(m_Position, m_Position + m_Vectors.Front, m_Vectors.Up); }

    void ProcessKeyboard(CameraMovement direction, float deltaTime);

    void ProcessMouseMovement(float xoffset, float yoffset);

private:
    float lastX = 800 / 2.0f;
    float lastY = 600 / 2.0f;

    bool firstMouse = true;

    glm::vec3 m_Position;

    CameraVectors m_Vectors;

    void UpdateCameraVectors();
};