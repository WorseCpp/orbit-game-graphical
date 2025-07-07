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
    std::shared_ptr<VBO<T>> m_vbo;
    std::shared_ptr<IBO> m_ibo;

public:
    EntitySprite(const glm::vec3& pos, const glm::vec3& euler_angles, std::shared_ptr<VBO<T>> vbo, std::shared_ptr<IBO> ibo)
        : m_vbo(vbo), m_ibo(ibo)
    {
        m_pos = pos;
        this->euler_angles = euler_angles;
        
        m_model_matrix = glm::mat4(1.0f);
        
        make_model_mat();
        m_id = _BaseSprite::id++;
    }

    void make_model_mat()
    {
        m_model_matrix = glm::mat4(1.0f);
        m_model_matrix = glm::rotate(m_model_matrix, euler_angles.x, glm::vec3(1.0f, 0.0f, 0.0f));
        m_model_matrix = glm::rotate(m_model_matrix, euler_angles.y, glm::vec3(0.0f, 1.0f, 0.0f));
        m_model_matrix = glm::rotate(m_model_matrix, euler_angles.z, glm::vec3(0.0f, 0.0f, 1.0f));
        m_model_matrix = glm::translate(m_model_matrix, m_pos);
    }


};
    

#endif

#define SPRITE_HPP



