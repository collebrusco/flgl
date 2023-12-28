#ifndef VERTICIES_H
#define VERTICIES_H

#include "gfx.h"
#include "gl_objs/VertexBuffer.h"

struct __attribute__((packed)) Vt3p2u {
	glm::vec3 pos;
	glm::vec2 uv;
}; typedef Vt3p2u Vt_classic;


#endif
