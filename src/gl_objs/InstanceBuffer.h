/** 
 * InstancedBuffer.h 
 * flgl
 * created 04/18/25 by frank collebrusco
 */
#ifndef INSTANCED_BUFFER_H
#define INSTANCED_BUFFER_H
#include "Buffer.h"
#include "VertexArray.h"
#include <flgl/glm.h>

typedef uint8_t  InstancedAttribute_u8,	   iAttr_u8;
typedef uint16_t InstancedAttribute_u16,   iAttr_u16;
typedef uint32_t InstancedAttribute_u32,   iAttr_u32;

typedef int8_t  InstancedAttribute_i8,	   iAttr_i8;
typedef int16_t InstancedAttribute_i16,    iAttr_i16;
typedef int32_t InstancedAttribute_i32,    iAttr_i32;

typedef float  InstancedAttribute_f32,	   iAttr_f32;
typedef double InstancedAttribute_f64,	   iAttr_f64;

typedef glm::vec2 InstancedAttribute_v2,   iAttr_v2;
typedef glm::vec3 InstancedAttribute_v3,   iAttr_v3;
typedef glm::vec4 InstancedAttribute_v4,   iAttr_v4;

typedef glm::ivec2 InstancedAttribute_iv2, iAttr_iv2;
typedef glm::ivec3 InstancedAttribute_iv3, iAttr_iv3;
typedef glm::ivec4 InstancedAttribute_iv4, iAttr_iv4;

typedef glm::mat3 InstancedAttribute_m3,   iAttr_m3;
typedef glm::mat4 InstancedAttribute_m4,   iAttr_m4;

typedef struct IntancedAttirbute_2v4 {
	glm::vec4 a, b;
} 										   iAttr_2v4;
typedef struct IntancedAttirbute_3v4 {
	glm::vec4 a, b, c;
} 										   iAttr_3v4;

template<typename Attrib>
class InstanceBuffer : public Buffer {
public:

	InstanceBuffer() : Buffer(GL_ARRAY_BUFFER) {}

	size_t isize() const {
		return sizeof(Attrib);
	}

	void attach_to_vao(VertexArray&, size_t idx) const;
};



#endif /* INSTANCED_BUFFER_H */
