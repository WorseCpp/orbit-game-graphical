// Camera.h
#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Default camera values
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;

const int FORWARD = 1;
const int BACKWARD = -1;
const int LEFT = -2;
const int RIGHT = 2;

class Camera {
public:
    // Camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // Euler Angles
    float Yaw;
    float Pitch;
    // Camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    // Constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f),
           glm::vec3 up       = glm::vec3(0.0f, 1.0f, 0.0f),
           float yaw          = YAW,
           float pitch        = PITCH)
      : Front(glm::vec3(0.0f, 0.0f, -1.0f)),
        MovementSpeed(SPEED),
        MouseSensitivity(SENSITIVITY),
        Zoom(ZOOM) {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    // Processes input received from any keyboard-like input system. Accepts movement direction and deltaTime.
    void processMove(const glm::vec3& direction, float deltaTime) {
        float velocity = MovementSpeed * deltaTime;
        Position += direction * velocity;
    }

    // Processes input received from a mouse input system.
    void processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true) {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw   += xoffset;
        Pitch += yoffset;

        if (constrainPitch) {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        updateCameraVectors();
    }

    // Processes input received from a mouse scroll-wheel event.
    void ProcessMouseScroll(float yoffset) {
        Zoom -= yoffset;
        if (Zoom < 1.0f)
            Zoom = 1.0f;
        if (Zoom > 45.0f)
            Zoom = 45.0f;
    }

    glm::mat4 GetProjectionMatrix(float width, float height) const {
        return glm::perspective(glm::radians(Zoom), width / height, 0.1f, 1000.0f);
    }
    
    glm::mat4 GetViewMatrix() const {
        return glm::lookAt(Position, Position + Front, Up);
    }


private:
    // Calculates the front vector from the Camera's (updated) Euler Angles.
    void updateCameraVectors() {
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        Right = glm::normalize(glm::cross(Front, WorldUp));
        Up    = glm::normalize(glm::cross(Right, Front));
    }
};

#endif // CAMERA_H