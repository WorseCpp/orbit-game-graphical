#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "common.hpp"

class Shader {
public:
    // the program ID
    unsigned int ID;

    // Constructor that builds the shader program from vertex and fragment shader source files
    Shader(const std::string path) {
        
        std::string vertexPath = path + "/vertex_shader.glsl";
        std::string fragmentPath = path + "/frag_shader.glsl";
        
        // 1. Retrieve the vertex/fragment source code from file

        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        
        // Ensure ifstream objects can throw exceptions:
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try {
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();		
            vShaderFile.close();
            fShaderFile.close();
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        } catch (std::ifstream::failure& e) {
            std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ\n";
        }
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();
        
        // 2. Compile shaders
        unsigned int vertex, fragment;
        // Vertex Shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, nullptr);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");
        
        // Fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, nullptr);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");
        
        // 3. Link shaders into a shader program
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");
        
        // 4. Delete the shader objects once linked
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }
    
    // Activate the shader program
    void bind() { 
        glUseProgram(ID); 
    }
    
    // Utility functions for setting uniform variables in the shader
    void setBool(const std::string& name, bool value) const {         
        glUniform1i(glGetUniformLocation(ID, name.c_str()), static_cast<int>(value)); 
    }
    
    void setInt(const std::string& name, int value) const { 
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
    }
    
    void setFloat(const std::string& name, float value) const { 
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
    }

    void setMat4f(const std::string &name, const float* matrix) const {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, matrix);
    }

    ~Shader() {
        // Delete the shader program
        glDeleteProgram(ID);
    }

private:
    // Utility function for checking shader compilation/linking errors.
    void checkCompileErrors(unsigned int shader, const std::string& type) {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM") {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
                std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " 
                          << type << "\n" << infoLog 
                          << "\n -- --------------------------------------------------- -- " 
                          << std::endl;
            }
        } else {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
                std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " 
                          << type << "\n" << infoLog 
                          << "\n -- --------------------------------------------------- -- " 
                          << std::endl;
            }
        }
    }
};

#endif // SHADER_HPP