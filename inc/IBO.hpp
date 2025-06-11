#ifndef IBO_HPP
#define IBO_HPP

#include "common.hpp"

class IBO {
public:
    // Constructs an index buffer with given indices and their count.
    IBO(const unsigned int* indices, unsigned int count)
        : m_Count(count)
    {
        glGenBuffers(1, &m_ID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    // Deletes the buffer object.
    ~IBO() {
        glDeleteBuffers(1, &m_ID);
    }

    // Binds the index buffer.
    void bind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
    }

    // Unbinds the index buffer.
    void unbind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    // Returns the number of indices.
    unsigned int getCount() const { return m_Count; }

private:
    unsigned int m_ID;
    unsigned int m_Count;
};

class DynIBO {
public:
    // Constructs a dynamic index buffer with allocated space for 'count' indices.
    DynIBO(unsigned int count)
        : m_Count(count + 1)
    {
        glGenBuffers(1, &m_ID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), nullptr, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    // Deletes the dynamic index buffer.
    ~DynIBO() {
        glDeleteBuffers(1, &m_ID);
    }

    // Binds the dynamic index buffer.
    void bind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
    }

    // Unbinds the dynamic index buffer.
    void unbind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    // Updates the dynamic index buffer with new data starting at an optional offset.
    void loadData(const unsigned int* indices, unsigned int count, unsigned int offset = 0) {
        bind();
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset * sizeof(unsigned int), count * sizeof(unsigned int), indices);
        unbind();
    }

    void loadData(const std::vector<unsigned int>& data, GLsizeiptr idx = 0){
        if (data.empty()) {
            std::cerr << "Error: Attempting to load empty data into VBO." << std::endl;
            return;
        }
        if (data.size() >= m_Count) {
            std::cerr << "Error: Data size exceeds VBO capacity." << std::endl;
            return;
        }
        
        
        loadData(data.data(), data.size(), idx);
    }

    // Returns the maximum number of indices the buffer can hold.
    unsigned int getCount() const { return m_Count; }

private:
    unsigned int m_ID;
    unsigned int m_Count;
};

#endif // IBO_HPP