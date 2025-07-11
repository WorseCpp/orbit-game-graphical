

#include "common.hpp"

#include "VBO.hpp"

#include "IBO.hpp"

#include "Verts.hpp"

#include "Inputs.hpp"

#include "Shader.hpp"

#include "Debug.hpp"

#include "Texture.hpp"

#include "Geom.hpp"

#include "ProcGen.hpp"

#include "Sprite.hpp"

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

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    //glCullFace(GL_BACK);
    glDebugMessageCallback(err_callback, nullptr);


    //282 463




    
    std::shared_ptr<Camera> camera = std::make_shared<Camera>(glm::vec3(32.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), -180.0f, 0.0f);

    InputHandler inputHandler(camera);

    // VAO, VBO
    VAO vao;
    vao.bind();

    DynVBO<P_N_C> dyn_vbo = DynVBO<P_N_C>(3e6, 3e5);

    // DynIBO creation and data loading
    DynIBO dyn_ibo(3e7 * 5, 3e6);

    //std::cout << "gonna make a planet" << std::endl;
    Planet planet = Planet(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), std::make_shared<DynVBO<P_N_C>>(dyn_vbo), std::make_shared<DynIBO>(dyn_ibo), 
            mt_gen());
    //std::cout << "made a planet" << std::endl;
    planet.ship();

    Planet planet2 = Planet(glm::vec3(100.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), std::make_shared<DynVBO<P_N_C>>(dyn_vbo), std::make_shared<DynIBO>(dyn_ibo), 
            mt_gen());
    planet2.ship();


    Texture earth_texture("./8081_earthmap10k.jpg");
    earth_texture.bind();

    Shader simple_shad = Shader("./shad/PNC_simple");
    
    simple_shad.bind();

    //Texture earth_texture("./8081_earthmap10k.jpg");
    

   

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

    // Bind the texture to the uniform
    //simple_tex_shad.bind();
    //earth_texture.bind();
    //simple_tex_shad.setInt("ourTexture", 0);

    // Render loop
    float theta = 0.0;
    while (!glfwWindowShouldClose(window)) {

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera->GetViewMatrix();
        glm::mat4 projection = camera->GetProjectionMatrix(800.0f, 600.0f);
   

        glm::mat4 MVP = projection * view * model;
        theta += .01 * 10;
        simple_shad.setMat4f("MVP", &MVP[0][0]);
        simple_shad.setFloat("theta", theta);
        

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        vao.bind();
        dyn_vbo.bind();
        dyn_ibo.bind();

        // Example: Draw using glMultiDrawElements for multiple blocks
        // Suppose you have arrays of counts and offsets for each block
        // Replace these with your actual data
        std::vector<GLsizei> counts ;// = {planet.numIndicies()}; // Number of indices per block
        std::vector<void*> offsets  ;//=  {nullptr}; // Offset for each block (nullptr for start)
        
        planet.addDrawCallData(counts, offsets);
        planet2.addDrawCallData(counts, offsets);

        // Draw multiple blocks (here only one block as example)
        glMultiDrawElements(GL_TRIANGLES, counts.data(), GL_UNSIGNED_INT, offsets.data(), counts.size());
        glfwSwapBuffers(window);
        glfwPollEvents();

        inputHandler.doKeyboardUpdate();
    }

    // Cleanup
    // No manual cleanup required as VAO and VBO destructors handle deletion.
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}