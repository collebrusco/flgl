//
//  Vertex.h
//  graphics-library-interface
//
//  Created by Frank Collebrusco on 3/18/23.
//
//

#ifndef Vertex_h
#define Vertex_h

#include <vector>
#define GLM_FORCE_SWIZZLE
#include <glm/vec2.hpp>
#include <glm/vec3.hpp> // glm::vec3

/*
 TODO: this currently only allows for one vertex type (3 floats for position, 2 for UV coords).
 TODO: Add configurability for custom vertex types. This requires configuring gl vertex attribute pointers
 TODO: Which is probably functionality that will be moved into the mesh objects (for uploading)
 TODO: Meshdetails can likely stay the same, user just needs to use correct shader for their vertex type
 */

typedef uint32_t vao_id_t;

enum DrawType_t {
    TRIANGLES, LINES
};

struct Vertex {
    Vertex();
    Vertex(glm::vec3);
    Vertex(glm::vec3, glm::vec2);
    glm::vec3 pos;
    glm::vec2 UV;
};

// Use this to create meshes
struct ConstMesh {
    const std::vector<Vertex>& verticies;
    const std::vector<uint32_t>& elements;
    DrawType_t type;
    ConstMesh(const std::vector<Vertex>& vs, const std::vector<uint32_t>& ems, DrawType_t t);
};

struct Mesh {
    std::vector<Vertex> verticies;
    std::vector<uint32_t> elements;
    DrawType_t type;
    Mesh() = default;
    Mesh(DrawType_t t);
};

struct MeshDetails {
    MeshDetails();
    MeshDetails(vao_id_t v, uint32_t e);
    MeshDetails(vao_id_t v, uint32_t e, DrawType_t dt);
    vao_id_t vao;
    uint32_t numElements;
    DrawType_t drawType;
};

#endif /* Vertex_h */
