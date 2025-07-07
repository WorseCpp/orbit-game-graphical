#ifndef SPRITE_HPP
#include "common.hpp"
#include "Verts.hpp"
#include "VBO.hpp"
#include "IBO.hpp"

class _BaseSprite
{
    static int id;
    int m_id;
    
    glm::vec3 m_pos;
    glm::vec3 euler_angles;

    glm::mat4 m_model_matrix;    
    int m_vert_loc;
    int m_index_loc;
};

template<HasAttribPointer T>
class EntitySprite : public _BaseSprite
{
    std::vector<T> m_vertices;
    std::vector<unsigned int> m_indices;
    std::shared_ptr<DynVBO<T>> m_vbo;
    std::shared_ptr<DynIBO> m_ibo;

    std::vector<int> m_vbo_blocks;
    std::vector<int> m_ibo_blocks;

public:
    EntitySprite(const glm::vec3& pos, const glm::vec3& euler_angles, std::shared_ptr<DynVBO<T>> vbo, std::shared_ptr<DynIBO> ibo)
        : m_vbo(vbo), m_ibo(ibo)
    {
        m_pos = pos;
        this->euler_angles = euler_angles;
        
        m_model_matrix = glm::mat4(1.0f);
        
        make_model_mat();
        m_id = _BaseSprite::id++;
    }

    void ship()
    {
        m_vbo->bind();
        m_ibo->bind();

        while (m_vbo_blocks.size() * m_vbo->allocator.blockSize() < m_vertices.size()) {
            m_vbo_blocks.push_back(m_vbo->allocator.allocate());
        }

        while (m_ibo_blocks.size() * m_ibo->allocator.blockSize() < m_indices.size()) {
            m_ibo_blocks.push_back(m_ibo->allocator.allocate());
        }

        for (int i = 0; i < m_indices.size(); i++) {
            int block = m_indices[i] / m_ibo->allocator.blockSize();
            m_indices[i] = m_indices[i] + block * m_vbo->allocator.blockSize();
        }

        for (int i = 0; i < m_vbo_blocks.size(); i++) {
            int block = m_vbo_blocks[i];
            m_vbo->loadData(m_vertices.data() + block * m_vbo->allocator.blockSize(), 
                std::min(m_vertices.size() - block * m_vbo->allocator.blockSize(), m_vbo->allocator.blockSize()), 
                block);
        }

        for (int i = 0; i < m_ibo_blocks.size(); i++) {
            int block = m_ibo_blocks[i];
            m_ibo->loadData(m_indices.data() + block * m_ibo->allocator.blockSize(),
                std::min(m_indices.size() - block * m_ibo->allocator.blockSize(), m_ibo->allocator.blockSize()), 
                block);
        }
    }

    void make_model_mat()
    {
        m_model_matrix = glm::mat4(1.0f);
        m_model_matrix = glm::rotate(m_model_matrix, euler_angles.x, glm::vec3(1.0f, 0.0f, 0.0f));
        m_model_matrix = glm::rotate(m_model_matrix, euler_angles.y, glm::vec3(0.0f, 1.0f, 0.0f));
        m_model_matrix = glm::rotate(m_model_matrix, euler_angles.z, glm::vec3(0.0f, 0.0f, 1.0f));
        m_model_matrix = glm::translate(m_model_matrix, m_pos);
    }

    void append_draw_info(std::vector<unsigned int>& block, std::vector<)


};
    

#endif

#define SPRITE_HPP



