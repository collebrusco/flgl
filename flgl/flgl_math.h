//
//  flgl_math.h
//  flgl-tester
//
//  Created by Frank Collebrusco on 5/30/23.
//
/*
 FLGL_Math provides commonly re-used matrix calculations.
 */

#ifndef flgl_math_h
#define flgl_math_h

#include <glm/ext/matrix_transform.hpp>

// Model matrix calculators
// These generate model matricies from 2D or 3D position, rotation, scale, and optionally anchor points.
glm::mat4 genModelMat2d(glm::vec2 pos, float rotation, glm::vec2 scale, glm::vec2 anchor);
glm::mat4 genModelMat2d(glm::vec2 pos, float rotation, glm::vec2 scale);
glm::mat4 genModelMat3d(glm::vec3 pos, glm::vec3 rotation, glm::vec3 scale, glm::vec3 anchor);
glm::mat4 genModelMat3d(glm::vec3 pos, glm::vec3 rotation, glm::vec3 scale);

#endif /* flgl_math_h */
