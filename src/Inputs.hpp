#ifndef INPUTS_HPP
#define INPUTS_HPP

#include <memory>

#include "Camera.hpp"
#include <unordered_map>

class InputHandler {
public:
    InputHandler(std::shared_ptr<Camera> camera);
    ~InputHandler();

    // Public member functions can be added here

    void processKeyboard(int key, int action);
    void processMouseMovement(float xoffset, float yoffset);
    void processMouseScroll(float yoffset);
    void setCamera(std::shared_ptr<Camera> camera);
    std::shared_ptr<Camera> getCamera() const;
    void doKeyboardUpdate();
private:
    // Private data members can be added here
    std::shared_ptr<Camera> m_camera;
    std::unordered_map<int, bool> m_keyStates; // To track key states
    glm::vec2 m_mouseposition = glm::vec2(0.0f, 0.0f); // To track mouse position
    bool mouseCaptured = false; // To track mouse capture state
};

#endif // INPUTS_HPP