//
//  flgl_math.cpp
//  flgl-tester
//
//  Created by Frank Collebrusco on 5/30/23.
//

#include "flgl_math.h"

glm::mat4 genModelMat2d(glm::vec2 pos, float rotation, glm::vec2 scale, glm::vec2 anchor){
    glm::mat4 m(1.0);
    m = glm::translate(m, glm::vec3(pos, 0.0));
    m = glm::rotate(m, glm::radians(rotation), glm::vec3(0.0, 0.0, 1.0));
    m = glm::scale(m, glm::vec3(scale, 1.0));
    m = glm::translate(m, glm::vec3(anchor, 0.0));
    return m;
}
glm::mat4 genModelMat2d(glm::vec2 pos, float rotation, glm::vec2 scale){
    glm::mat4 m(1.0);
    m = glm::translate(m, glm::vec3(pos, 0.0));
    m = glm::rotate(m, glm::radians(rotation), glm::vec3(0.0, 0.0, 1.0));
    m = glm::scale(m, glm::vec3(scale, 1.0));
    return m;
}
glm::mat4 genModelMat3d(glm::vec3 pos, glm::vec3 rotation, glm::vec3 scale, glm::vec3 anchor){
    glm::mat4 m;
    m = glm::mat4(1.0);
    m = glm::translate(m, pos);
    m = glm::rotate(m, glm::radians(rotation.x), glm::vec3(1.0, 0.0, 0.0));
    m = glm::rotate(m, glm::radians(rotation.y), glm::vec3(0.0, 1.0, 0.0));
    m = glm::rotate(m, glm::radians(rotation.z), glm::vec3(0.0, 0.0, 1.0));
    m = glm::scale(m, scale);
    m = glm::translate(m, anchor);
    return m;
}
glm::mat4 genModelMat3d(glm::vec3 pos, glm::vec3 rotation, glm::vec3 scale){
    glm::mat4 m;
    m = glm::mat4(1.0);
    m = glm::translate(m, pos);
    m = glm::rotate(m, glm::radians(rotation.x), glm::vec3(1.0, 0.0, 0.0));
    m = glm::rotate(m, glm::radians(rotation.y), glm::vec3(0.0, 1.0, 0.0));
    m = glm::rotate(m, glm::radians(rotation.z), glm::vec3(0.0, 0.0, 1.0));
    m = glm::scale(m, scale);
    return m;
}

glm::vec2 angleToVector(float degrees){
    /* TODO make this be angle from (0, 1) */
    return glm::vec2(glm::cos(glm::radians(degrees)), glm::sin(glm::radians(degrees)));
}

float vectorToAngle(glm::vec2 vec) {
    float a = glm::degrees(acos(glm::dot(glm::normalize(vec), glm::vec2(0.0f, 1.0f))));
    return vec.x > 0.f ? 360.f - a : a;
}
