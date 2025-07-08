#ifndef SPRITE_HPP
#define SPRITE_HPP
#include "common.hpp"
#include "Verts.hpp"
#include "VBO.hpp"
#include "IBO.hpp"
#include <glm/gtc/matrix_transform.hpp>

#include "ProcGen.hpp"

class BaseSprite
{

    protected:
    int m_id;
    static inline int id = 0;
    
    glm::vec3 m_pos;
    glm::vec3 euler_angles;

    glm::mat4 m_model_matrix;    
    int m_vert_loc;
    int m_index_loc;
};

template<HasAttribPointer T>
class EntitySprite : public BaseSprite
{
protected:
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
        m_id = BaseSprite::id++;
    }

    void ship()
    {
        m_vbo->bind();
        m_ibo->bind();

        while (m_vbo_blocks.size() * m_vbo->allocator->blockSize() < m_vertices.size()) {
            m_vbo_blocks.push_back(m_vbo->allocator->allocate());
            // cout << "vbo block allocated: " << m_vbo_blocks.back() << "\n";
        }

        while (m_ibo_blocks.size() * m_ibo->allocator->blockSize() < m_indices.size()) {
            m_ibo_blocks.push_back(m_ibo->allocator->allocate());
            // cout << "ibo block allocated: " << m_ibo_blocks.back() << "\n";
        }

        // for (auto& block : m_vbo_blocks) {
        //     cout << "vbo block; " << block << "\n";
        // }
        // for (auto& block : m_ibo_blocks) {
        //     cout << "ibo block; " << block << "\n";
        // }

        for (int i = 0; i < m_indices.size(); i++) {
            int block = m_indices[i] / m_vbo->allocator->blockSize();
            m_indices[i] = (m_indices[i] % m_vbo->allocator->blockSize()) + m_vbo_blocks[block];
        }

        for (int i = 0; i < m_vbo_blocks.size(); i++) {
            int block = m_vbo_blocks[i];
            m_vbo->loadData(m_vertices.data() + i * m_vbo->allocator->blockSize(), 
                std::min(m_vertices.size() - i * m_vbo->allocator->blockSize(), m_vbo->allocator->blockSize()), 
                block);
        }

        for (int i = 0; i < m_ibo_blocks.size(); i++) {
            int block = m_ibo_blocks[i];
            m_ibo->loadData(m_indices.data() + i * m_ibo->allocator->blockSize(),
                std::min(m_indices.size() - i * m_ibo->allocator->blockSize(), m_ibo->allocator->blockSize()), 
                block);
        }
    }

    size_t numIndicies()
    {
        return static_cast<size_t>(m_indices.size());
    }

    void make_model_mat()
    {
        m_model_matrix = glm::mat4(1.0f);
        m_model_matrix = glm::rotate(m_model_matrix, euler_angles.x, glm::vec3(1.0f, 0.0f, 0.0f));
        m_model_matrix = glm::rotate(m_model_matrix, euler_angles.y, glm::vec3(0.0f, 1.0f, 0.0f));
        m_model_matrix = glm::rotate(m_model_matrix, euler_angles.z, glm::vec3(0.0f, 0.0f, 1.0f));
        m_model_matrix = glm::translate(m_model_matrix, m_pos);
    }

    void addDrawCallData(std::vector<GLsizei>& draw_counts, std::vector<void*>& draw_blocks)
    {
        for (int i = 0; i < m_ibo_blocks.size(); ++i) {
            draw_blocks.push_back((void*)(sizeof(unsigned int) * m_ibo_blocks[i]));
            draw_counts.push_back(static_cast<GLsizei>(std::min(m_ibo->allocator->blockSize(), m_indices.size() - i * m_ibo->allocator->blockSize())));
        }
    }

    ~EntitySprite()
    {
        for (auto& block : m_vbo_blocks) {
            m_vbo->allocator->deallocate(block);
        }
        for (auto& block : m_ibo_blocks) {
            m_ibo->allocator->deallocate(block);
        }  
    }

    // Templated mesh generator, to be specialized in derived classes
    virtual void mesh() = 0;
};

class Planet : public EntitySprite<P_N_C>
{
    unsigned long long m_seed;
    int m_nTheta;
    int m_nPhi;
    double m_rad;
public:
    Planet(const glm::vec3& pos, const glm::vec3& euler_angles, std::shared_ptr<DynVBO<P_N_C>> vbo, std::shared_ptr<DynIBO> ibo, unsigned long long seed, int nTheta = 1024, int nPhi = 1024, double rad = 32.)
        : EntitySprite<P_N_C>(pos, euler_angles, vbo, ibo), m_seed(seed), m_nTheta(nTheta), m_nPhi(nPhi), m_rad(rad)
    {

        mesh();
    }


    ~Planet()
    {
        for (auto& block : m_vbo_blocks) {
            m_vbo->allocator->deallocate(block);
        }
        for (auto& block : m_ibo_blocks) {
            m_ibo->allocator->deallocate(block);
        }
    }

    void mesh()
    {
        // Generate the mesh for the planet here
        // This could involve creating vertices and indices based on the planet's geometry
        // For example, you might create a sphere or a cube to represent the planet
        
        // The model_P_N_C takes a glm::Vec3 for position, normal, and color

        auto planet = PlanetArray(m_nTheta, m_nPhi, m_rad);
    
        planet.fractal(m_seed);
    
        std::tie(m_vertices, m_indices) = planet.mesh<P_N_C>();

        for (auto& v : m_vertices) {
            v.pos += m_pos; // Offset the vertices by the planet's position
        }

        std::cout << m_vertices.size() << " vertices, " << m_indices.size() << " indices\n";
    }
};
#endif



