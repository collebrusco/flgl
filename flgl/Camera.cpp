//
//  Camera.cpp
//  opengl-tools
//
//  Created by Frank Collebrusco on 12/27/22.
//
//

#include "Camera.h"
#include "../flgl.h"

Camera::Camera(){
    should_update = true;
}

void Camera::update(){
    if (shouldUpdate() || prev_frame != Graphics::getWindow().frame){
        this->updateProj();
        this->updateView();
        should_update = false;
    }
}
glm::mat4 Camera::updateView(){
    setShouldUpdate();
    glm::vec3 target = pos + look;
    _view = lookAt(pos, target, up);
    return _view;
}
glm::mat4 const& Camera::view() const {return _view;}
glm::mat4 const& Camera::proj() const {return _proj;}

void Camera::setShouldUpdate() { 
    should_update = true;
}

bool Camera::shouldUpdate() const {
    return should_update;
}

glm::vec3 &Camera::getPos() { 
    setShouldUpdate();
    return pos;
}

glm::vec3 &Camera::getLook() { 
    setShouldUpdate();
    return look;
}

glm::vec3 &Camera::getUp() { 
    setShouldUpdate();
    return up;
}

float &Camera::getNear() { 
    setShouldUpdate();
    return near;
}

float &Camera::getFar() { 
    setShouldUpdate();
    return far;
}

const glm::vec3 &Camera::readPos() const {
    return pos;
}

const glm::vec3 &Camera::readLook() const {
    return look;
}

const glm::vec3 &Camera::readUp() const {
    return up;
}

float Camera::readNear() const {
    return near;
}

float Camera::readFar() const {
    return far;
}


//********************************ORTHO********************************
OrthoCamera::OrthoCamera(){
    pos = glm::vec3(0.f, 0.f, 1.f);
    look = glm::vec3(0.f, 0.f, -1.f);
    up = glm::vec3(0.f, 1.f, 0.f);
    near = 0.0001f;
    far = 10000.0f;
    viewWidth = 10;
}
OrthoCamera::OrthoCamera(glm::vec3 p, glm::vec3 l, glm::vec3 u, float n, float f, float w){
    pos = p; look = l; up = u; near = n; far = f; viewWidth = w;
}
float OrthoCamera::readViewWidth() const {
    return viewWidth;
}
float& OrthoCamera::getViewWidth(){
    setShouldUpdate();
    return viewWidth;
}
glm::mat4 OrthoCamera::updateProj()  {
    setShouldUpdate();
    glm::vec2 orthoDims = glm::vec2(viewWidth, viewWidth / Graphics::getWindow().aspect);

    glm::vec4 lrbt = (lrbtMat * orthoDims);

    orthoDims.y = orthoDims.x / Graphics::getWindow().aspect;
    lrbt = (lrbtMat * orthoDims);
    _proj = glm::ortho(lrbt.x, lrbt.y, lrbt.z, lrbt.w, near, far);
    return _proj;
}

void OrthoCamera::setViewWidth(float vw) {
    setShouldUpdate();
    viewWidth = vw;
}


//********************************ORTHO********************************
PerspectiveCamera::PerspectiveCamera(){
    phi = 3.14/2; theta = 0; mouseControlled = false;
    pos = glm::vec3(0.f, 0.f, 1.f);
    look = glm::vec3(0.f, 0.f, -1.f);
    up = glm::vec3(0.f, 1.f, 0.f);
    near = 0.0001f;
    far = 10000.0f;
    fov = glm::radians(90.f);
}

PerspectiveCamera::PerspectiveCamera(glm::vec3 p, glm::vec3 l, glm::vec3 u, float n, float f, float fv){
    pos = p; look = l; up = u; near = n; far = f; fov = glm::radians(fv); phi = 3.14/2; theta = 0; mouseControlled = false;
}

void PerspectiveCamera::setFOV(float fv){
    setShouldUpdate();
    fov = glm::radians(fv);
}

float& PerspectiveCamera::getFOV(){
    setShouldUpdate();
    return fov;
}

float PerspectiveCamera::readFOV() const {
    return fov;
}

glm::mat4 PerspectiveCamera::updateProj(){
    setShouldUpdate();
    _proj = glm::perspective(fov, Graphics::getWindow().aspect, near, far);
    return _proj;
}

void PerspectiveCamera::update(){
    if (mouseControlled){
        phi += Graphics::getWindow().mouse.delta.y * 0.01; //TODO: mouse sensitivity / move control elsewhere
        theta -= Graphics::getWindow().mouse.delta.x * 0.01;
        glm::vec3 anchor = glm::vec3(-cos(theta), 0.0, sin(theta));
        look = glm::vec3(-1 * sin(phi) * sin(theta),
                    cos(phi),
                    -1 * sin(phi) * cos(theta));
        up = cross(look, anchor);
    }
    this->Camera::update();
}

void PerspectiveCamera::setMouseControl(bool f){
    mouseControlled = f;
}
