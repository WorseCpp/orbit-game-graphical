#include "common.hpp"

#include "VBO.hpp"

#include "IBO.hpp"

#include "Verts.hpp"

#include "Inputs.hpp"

// Vertex shader source
const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
uniform mat4 MVP;
void main() {
    gl_Position = MVP * vec4(aPos, 1.0);
}
)";

// Fragment shader source
const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
void main() {
    FragColor = vec4(1.0, 0.5, 0.2, 1.0);
}
)";


// VAO class
class VAO {
public:
    VAO() { glGenVertexArrays(1, &id); }
    ~VAO() { glDeleteVertexArrays(1, &id); }
    void bind() const { glBindVertexArray(id); }
    void unbind() const { glBindVertexArray(0); }
    GLuint getID() const { return id; }
private:
    GLuint id;
};

void err_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
    std::string sourceStr;
    switch (source) {
        case GL_DEBUG_SOURCE_API:             sourceStr = "API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   sourceStr = "Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: sourceStr = "Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:      sourceStr = "Third Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION:      sourceStr = "Application"; break;
        case GL_DEBUG_SOURCE_OTHER:            sourceStr = "Other"; break;
        default:                               sourceStr = "Unknown"; break;
    }

    std::string typeStr;
    switch (type) {
        case GL_DEBUG_TYPE_ERROR:               typeStr = "Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: typeStr = "Deprecated Behavior"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  typeStr = "Undefined Behavior"; break;
        case GL_DEBUG_TYPE_PORTABILITY:         typeStr = "Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         typeStr = "Performance"; break;
        case GL_DEBUG_TYPE_MARKER:              typeStr = "Marker"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          typeStr = "Push Group"; break;
        case GL_DEBUG_TYPE_POP_GROUP:           typeStr = "Pop Group"; break;
        case GL_DEBUG_TYPE_OTHER:               typeStr = "Other"; break;
        default:                                typeStr = "Unknown"; break;
    }

    std::string severityStr;
    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:         severityStr = "High"; break;
        case GL_DEBUG_SEVERITY_MEDIUM:       severityStr = "Medium"; break;
        case GL_DEBUG_SEVERITY_LOW:          severityStr = "Low"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: severityStr = "Notification"; break;
        default:                             severityStr = "Unknown"; break;
    }

    std::cerr << "OpenGL Debug Message [" << id << "]: " << message << "\n"
          << "Source: " << sourceStr << "\n"
          << "Type: " << typeStr << "\n"
          << "Severity: " << severityStr << std::endl;
    std::cerr << "OpenGL Debug Message: " << message << std::endl;
}

int main() {
    // Init GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Triangle", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Init GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW\n";
        return -1;
    }

    // Vertex data
    SFloat3 vertices[] = {
        SFloat3(0.0f,  0.5f, 0.0f),
        SFloat3(-0.5f, -0.5f, 0.0f),
        SFloat3(0.5f, -0.5f, 0.0f)
    };

    std::shared_ptr<Camera> camera = std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), 90.0f, 0.0f);

    InputHandler inputHandler(camera);

    // VAO, VBO
    VAO vao;
    vao.bind();

    DynVBO<SFloat3> dyn_vbo = DynVBO<SFloat3>(4);

    dyn_vbo.bind();
    dyn_vbo.loadData(vertices, 3);
    vao.unbind();

    // Index data for the triangle
    unsigned int indices[] = {0, 1, 2};

    // DynIBO creation and data loading
    DynIBO dyn_ibo(3);
    dyn_ibo.bind();
    dyn_ibo.loadData(indices, 3);
    dyn_ibo.unbind();

    // Shader compilation
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
   
    glEnable(GL_DEBUG_OUTPUT);
   
    glDebugMessageCallback(err_callback, nullptr);


    GLint success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    // Set the input handler as the user pointer for the window
    glfwSetWindowUserPointer(window, &inputHandler);
    // Set key callback to pass keyboard input to the input handler
    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        InputHandler* handler = static_cast<InputHandler*>(glfwGetWindowUserPointer(window));
        if (handler) {
            handler->processKeyboard(key, action);
        }
    });



    // Added mouse callback to pass mouse movement to the input handler
    glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos) {
        InputHandler* handler = static_cast<InputHandler*>(glfwGetWindowUserPointer(window));
        if (handler) {
            handler->processMouseMovement(xpos, ypos);
        }
    });

    // Render loop
    while (!glfwWindowShouldClose(window)) {

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera->GetViewMatrix();
        glm::mat4 projection = camera->GetProjectionMatrix(800.0f, 600.0f);
   

        glm::mat4 MVP = projection * view * model;
        GLint mvpLocation = glGetUniformLocation(shaderProgram, "MVP");
        glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, &MVP[0][0]);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        
        vao.bind();
        dyn_vbo.bind();
        dyn_ibo.bind();
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();

        inputHandler.doKeyboardUpdate();
    }

    // Cleanup
    // No manual cleanup required as VAO and VBO destructors handle deletion.
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}