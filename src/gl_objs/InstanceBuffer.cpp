#include "InstanceBuffer.h"
using namespace glm;

template<>
void InstanceBuffer<uint8_t>::attach_to_vao(VertexArray& vao) const {
    vao.attrib(vao.max_index()+1, 1, GL_UNSIGNED_BYTE, 0, 0, 1);
}
template<>
void InstanceBuffer<uint16_t>::attach_to_vao(VertexArray& vao) const {
    vao.attrib(vao.max_index()+1, 1, GL_UNSIGNED_SHORT, 0, 0, 1);
}
template<>
void InstanceBuffer<uint32_t>::attach_to_vao(VertexArray& vao) const {
    vao.attrib(vao.max_index()+1, 1, GL_UNSIGNED_INT, 0, 0, 1);
}
template<>
void InstanceBuffer<int8_t>::attach_to_vao(VertexArray& vao) const {
    vao.attrib(vao.max_index()+1, 1, GL_BYTE, 0, 0, 1);
}
template<>
void InstanceBuffer<int16_t>::attach_to_vao(VertexArray& vao) const {
    vao.attrib(vao.max_index()+1, 1, GL_SHORT, 0, 0, 1);
}
template<>
void InstanceBuffer<int32_t>::attach_to_vao(VertexArray& vao) const {
    vao.attrib(vao.max_index()+1, 1, GL_INT, 0, 0, 1);
}

template<>
void InstanceBuffer<float>::attach_to_vao(VertexArray& vao) const {
    vao.attrib(vao.max_index()+1, 1, GL_FLOAT, 0, 0, 1);
}
template<>
void InstanceBuffer<double>::attach_to_vao(VertexArray& vao) const {
    vao.attrib(vao.max_index()+1, 1, GL_DOUBLE, 0, 0, 1);
}

template<>
void InstanceBuffer<vec2>::attach_to_vao(VertexArray& vao) const {
    vao.attrib(vao.max_index()+1, 2, GL_FLOAT, 0, 0, 1);
}
template<>
void InstanceBuffer<vec3>::attach_to_vao(VertexArray& vao) const {
    vao.attrib(vao.max_index()+1, 3, GL_FLOAT, 0, 0, 1);
}
template<>
void InstanceBuffer<vec4>::attach_to_vao(VertexArray& vao) const {
    vao.attrib(vao.max_index()+1, 4, GL_FLOAT, 0, 0, 1);
}

template<>
void InstanceBuffer<ivec2>::attach_to_vao(VertexArray& vao) const {
    vao.attrib(vao.max_index()+1, 2, GL_INT, 0, 0, 1);
}
template<>
void InstanceBuffer<ivec3>::attach_to_vao(VertexArray& vao) const {
    vao.attrib(vao.max_index()+1, 3, GL_INT, 0, 0, 1);
}
template<>
void InstanceBuffer<ivec4>::attach_to_vao(VertexArray& vao) const {
    vao.attrib(vao.max_index()+1, 4, GL_INT, 0, 0, 1);
}

template<>
void InstanceBuffer<mat3>::attach_to_vao(VertexArray& vao) const {
    for (int i = 0; i < 3; i++) {
        vao.attrib(vao.max_index() + 1, 3, GL_FLOAT, sizeof(glm::mat3), sizeof(float) * i * 3, 1);
    }
}
template<>
void InstanceBuffer<mat4>::attach_to_vao(VertexArray& vao) const {
    for (int i = 0; i < 4; i++) {
        vao.attrib(vao.max_index() + 1, 4, GL_FLOAT, sizeof(glm::mat4), sizeof(float) * i * 4, 1);
    }
}


