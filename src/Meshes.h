//
//  Meshes.h
//  graphics-library-interface
//
//  Created by Frank Collebrusco on 3/19/23.
//

#ifndef Meshes_h
#define Meshes_h

#include <vector>
#include "Vertex.h"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

// ********************************TILE********************************
static const std::vector<Vertex> tile_vertices = {
    Vertex(glm::vec3(-0.5f, 0.5f, 0.f), glm::vec2(0.0f, 1.0f)),
    Vertex(glm::vec3(0.5f, 0.5f, 0.f), glm::vec2(1.0f, 1.0f)),
    Vertex(glm::vec3(-0.5f, -0.5f, 0.f), glm::vec2(0.0f, 0.0f)),
    Vertex(glm::vec3(0.5f, -0.5f, 0.f), glm::vec2(1.0f, 0.0f)),

};

static const std::vector<uint32_t> tile_elements = {
            0, 3, 2,
            0, 1, 3
        };
const ConstMesh TileMesh(tile_vertices, tile_elements, TRIANGLES);

static const std::vector<uint32_t> tile_outline_elements = {
    0, 1,
    1, 3,
    3, 2,
    2, 0
};
const ConstMesh TileOutlineMesh(tile_vertices, tile_outline_elements, LINES);

// ********************************CUBE********************************
static const std::vector<Vertex> cube_verticies = {
    Vertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec2(0.0f, 1.0f)),
    Vertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 1.0f)),
    Vertex(glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec2(0.0f, 0.0f)),
    Vertex(glm::vec3(0.5f, -0.5f, 0.5f), glm::vec2(1.0f, 0.0f)),

    Vertex(glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec2(0.0f, 1.0f)),
    Vertex(glm::vec3(0.5f, 0.5f, -0.5f), glm::vec2(1.0f, 1.0f)),
    Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f)),
    Vertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(1.0f, 0.0f))};

static const std::vector<uint32_t> cube_elements = {
            0, 3, 2,    // front face
            0, 1, 3,
            
            5, 6, 7,    // rear face
            4, 5, 6,
    
            0, 4, 2,    // left face
            4, 2, 6,
    
            1, 5, 7,    // right face
            3, 1, 7,
    
            0, 1, 5,    // top face
            0, 4, 5,
    
            2, 6, 7,    // bottom face
            2, 7, 3
        };
const ConstMesh CubeMesh(cube_verticies, cube_elements, TRIANGLES);


#endif /* Meshes_h */
