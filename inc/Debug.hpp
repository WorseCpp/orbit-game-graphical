
#ifndef DEBUG_HPP
#define DEBUG_HPP

#include "common.hpp"

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

    if (severity == GL_DEBUG_SEVERITY_HIGH) {
        std::cerr << "\033[1;31m";  // Set text color to red for errors
    }
    std::cerr << "OpenGL Debug Message [" << id << "]: " << message << "\n"
              << "Source: " << sourceStr << "\n"
              << "Type: " << typeStr << "\n"
              << "Severity: " << severityStr << std::endl;
    std::cerr << "OpenGL Debug Message: " << message << std::endl;
    if (severity == GL_DEBUG_SEVERITY_HIGH) {
        std::cerr << "\033[0m";  // Reset text color to default
    }
}

#endif