#ifndef VBO_HPP
#define VBO_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "common.hpp"

#include <type_traits>

#include <concepts>

template<typename T>
concept HasAttribPointer = requires(T t) {
    { t.setAttribPointer() } -> std::same_as<void>;
};

template<HasAttribPointer T>
class VBO {
public:
    VBO(){
        glGenBuffers(1, &id);
        glBindBuffer(GL_ARRAY_BUFFER, id);
        T dummy;
        dummy.setAttribPointer(); // Call setAttribPointer to configure the vertex attributes
    }

    ~VBO(){ glDeleteBuffers(1, &id); }

    void staticLoadData(T* data, GLsizeiptr arr_size){
        bind();
        glBufferData(GL_ARRAY_BUFFER, arr_size * sizeof(T), data, GL_STATIC_DRAW);
    }

    void bind() const {
        glBindBuffer(GL_ARRAY_BUFFER, id);
    }

    void unbind() const {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    GLuint getID() const {
        return id;
    }

private:
    GLuint id;
};

template<HasAttribPointer T>
class DynVBO {
public:
    DynVBO(int arr_size)
    : m_arr_size(arr_size)
    {
        glGenBuffers(1, &id);
        glBindBuffer(GL_ARRAY_BUFFER, id);
        glBufferData(GL_ARRAY_BUFFER, arr_size * sizeof(T), nullptr, GL_DYNAMIC_DRAW);
        T dummy;
        dummy.setAttribPointer(); // Call setAttribPointer to configure the vertex attributes
    }

    ~DynVBO(){ glDeleteBuffers(1, &id); }

    void loadData(T* data, GLsizeiptr arr_size, GLsizeiptr idx = 0){
        bind();

        if (idx < 0 || idx >= m_arr_size) {
            std::cerr << "Index out of bounds: " << idx << " for array size: " << m_arr_size << std::endl;
            return;
        }

        if (idx + arr_size < 0 || idx + arr_size >= m_arr_size) {
            std::cerr << "Data exceeds buffer size: " << idx + arr_size << " for array size: " << m_arr_size << std::endl;
            return;
        }

        glBufferSubData(GL_ARRAY_BUFFER, idx * sizeof(T), arr_size * sizeof(T), data);
    }

    void bind() const {
        glBindBuffer(GL_ARRAY_BUFFER, id);
    }

    void unbind() const {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    GLuint getID() const {
        return id;
    }

private:
    GLuint id;
    GLuint m_arr_size;
};

#include "VBO.hpp"
#endif
