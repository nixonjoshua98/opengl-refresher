#include "Camera.h"


Camera::Camera(glm::vec3 position, glm::vec3 up)
{
    m_Position = m_PreviousPosition = position;

    m_Vectors.WorldUp = up;

    m_Vectors.Front = glm::vec3(0.0f, 0.0f, -1.0f);

    MovementSpeed = 2.5f;

    MouseSensitivity = 0.1f;

    Yaw = -90.0f;
    Pitch = 0.0f;

    UpdateCameraVectors();
}

void Camera::ProcessKeyboard(CameraMovement direction, float deltaTime)
{
    float velocity = MovementSpeed * deltaTime;

    switch (direction)
    {
        case CameraMovement::FORWARD:
            m_Position += m_Vectors.Front * velocity;
            break;

        case CameraMovement::BACKWARD:
            m_Position -= m_Vectors.Front * velocity;
            break;

        case CameraMovement::LEFT:
            m_Position -= m_Vectors.Right * velocity;
            break;

        case CameraMovement::RIGHT:
            m_Position += m_Vectors.Right * velocity;
            break;
    }
}

void Camera::ProcessMouseMovement(float xOffset, float yOffset)
{
    xOffset *= MouseSensitivity;
    yOffset *= MouseSensitivity;

    Yaw += xOffset;
    Pitch += yOffset;

    Pitch = std::max(-89.0f, std::min(Pitch, 89.0f));

    UpdateCameraVectors();
}

void Camera::UpdateCameraVectors()
{
    glm::vec3 front;

    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));

    m_Vectors.Front = glm::normalize(front);

    m_Vectors.Right = glm::normalize(glm::cross(m_Vectors.Front, m_Vectors.WorldUp));

    m_Vectors.Up = glm::normalize(glm::cross(m_Vectors.Right, m_Vectors.Front));
}
