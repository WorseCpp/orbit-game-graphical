#include "Inputs.hpp"
#include "Camera.hpp"  // Ensure that this header provides the definition for CameraPtr
#include <iostream>
#include <GLFW/glfw3.h>

InputHandler::InputHandler(std::shared_ptr<Camera> cam) : m_camera(cam) {
    // Constructor implementation (if needed)
    m_keyStates = std::unordered_map<int, bool>();
    if (!m_camera) {
        std::cerr << "Warning: Camera pointer is null in InputHandler." << std::endl;
    }
}

InputHandler::~InputHandler() {
    // Destructor implementation (if needed)
}

void InputHandler::processKeyboard(int key, int action) {
    // Example implementation: log the keyboard event
    // std::cout << "Keyboard event: key = " << key << ", action = " << action << std::endl;


    if (action == GLFW_PRESS) {
        m_keyStates[key] = true;
    } else if (action == GLFW_RELEASE) {
        m_keyStates[key] = false;
    }    
}

glm::vec3 remove_comp(const glm::vec3& vec, const glm::vec3& component) {
    glm::vec3 result = vec;
    
    result -= component * glm::dot(vec, component) / glm::dot(component, component);

    return result;
}

void InputHandler::doKeyboardUpdate()
{
    float moveSpeed = 0.1f;
    if (m_keyStates[GLFW_KEY_W]) {
        m_camera->processMove(remove_comp(m_camera->Front, m_camera->Up), moveSpeed); // Move forward
    }
    if (m_keyStates[GLFW_KEY_S]) {
        m_camera->processMove(remove_comp(-m_camera->Front, m_camera->Up), moveSpeed); // Move backward
    }
    if (m_keyStates[GLFW_KEY_A]) {
        m_camera->processMove(remove_comp(-m_camera->Right, m_camera->Up), moveSpeed); // Move left
    }
    if (m_keyStates[GLFW_KEY_D]) {
        m_camera->processMove(remove_comp(m_camera->Right, m_camera->Up), moveSpeed); // Move right
    }
    if (m_keyStates[GLFW_KEY_SPACE]) {
        m_camera->processMove(m_camera->Up, moveSpeed); // Move up
    }
    if (m_keyStates[GLFW_KEY_LEFT_SHIFT]) {
        m_camera->processMove(-m_camera->Up, moveSpeed); // Move down
    }

    if (m_keyStates[GLFW_KEY_ESCAPE]) {

        GLFWwindow* window = glfwGetCurrentContext();
        if (mouseCaptured) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            std::cout << "Mouse released" << std::endl;
        } else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            int width, height;
            glfwGetWindowSize(window, &width, &height);
            glfwSetCursorPos(window, width / 2.0, height / 2.0);
            std::cout << "Mouse captured" << std::endl;
        }
        mouseCaptured = !mouseCaptured;
        m_keyStates[GLFW_KEY_ESCAPE] = false; // Prevent continuous toggling
    }
}

void InputHandler::processMouseMovement(float xoffset, float yoffset) {
    if (!mouseCaptured)
    {
        // If the mouse is not captured, we might want to ignore the movement
        // or handle it differently, such as updating a UI element or similar.
        return;
    }
    else
    {
        // Example implementation: log the mouse movement
        // std::cout << "Mouse move: xoffset = " << xoffset << ", yoffset = " << yoffset << std::endl;

        // If a camera is set, you could forward this for camera rotation/adjustment
    
        GLFWwindow* window = glfwGetCurrentContext();
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        float centerX = width / 2.0f;
        float centerY = height / 2.0f;

        xoffset = xoffset - centerX;
        yoffset = centerY - yoffset;

        float sens = 0.5f;
        xoffset *= sens;
        yoffset *= sens;

        m_camera->processMouseMovement(xoffset, yoffset);

        glfwSetCursorPos(window, centerX, centerY);
    }
}

void InputHandler::processMouseScroll(float yoffset) {
    // Example implementation: log the scroll event
    std::cout << "Mouse scroll: yoffset = " << yoffset << std::endl;

    // If a camera is set, you could adjust the camera zoom
    // e.g., m_camera->handleMouseScroll(yoffset);
}

void InputHandler::setCamera(std::shared_ptr<Camera> camera) {
    m_camera = camera;
}

std::shared_ptr<Camera> InputHandler::getCamera() const {
    return m_camera;
}
