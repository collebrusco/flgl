//
//  Camera.cpp
//  opengl-tools
//
//  Created by Frank Collebrusco on 12/27/22.
//
//

#include "Camera.h"
#include "../inc/flgl.h"
#include <flgl/logger.h>

using namespace glm;
LOG_MODULE(glcam);

Camera::Camera() {
    should_update = true;
    needs_inverses.val = 0x11;
}

vec2 Camera::world_mouse(vec2 mp) const {
    mp /= vec2((float)window.frame.x, (float)window.frame.y);
    vec4 p = vec4((mp.x * 2.) - 1.,  -((mp.y * 2) - 1), 0., 1.);
    vec4 w = this->iview() * (this->iproj() * (p));
    return w.xy();
}

Ray Camera::mouse_ray(vec2 mp) const {
    float nx = (2.0f * (mp.x / static_cast<float>(window.frame.x))) - 1.0f;
    float ny = -((2.0f * (mp.y / static_cast<float>(window.frame.y))) - 1.0f);

    vec4 camspace = this->iproj() * vec4(nx, ny, -1.0f, 1.0f);
    camspace /= camspace.w;

    mat4 inv_view = this->iview();
    vec4 wspace = inv_view * camspace;

    vec3 origin = this->readPos();
    vec3 direction = normalize(vec3(wspace) - origin);

    return Ray(origin, direction);
}

void Camera::update(){
    this->sub_update();
    if (this->update_condition()){
        this->updateProj();
        this->updateView();
        should_update = false;
        prev_frame = window.frame;
    }
}
glm::mat4 const& Camera::updateView(){
    setShouldUpdate();
    glm::vec3 target = pos + look;
    _view = lookAt(pos, target, up);
    return _view;
}
glm::mat4 const& Camera::view() const {return _view;}
glm::mat4 const& Camera::proj() const {return _proj;}
glm::mat4 const& Camera::iview() const {
    if (needs_inverses.mat.view) {
        _iview = glm::inverse(_view);
        needs_inverses.mat.view = false;
    }
    return _iview;
}
glm::mat4 const& Camera::iproj() const {
    if (needs_inverses.mat.proj) {
        _iproj = glm::inverse(_proj);
        needs_inverses.mat.proj = false;
    }
    return _iproj;
}

void Camera::setShouldUpdate() { 
    should_update = true;
    needs_inverses.val = 0x11;
}

bool Camera::shouldUpdate() const {
    return should_update;
}

bool Camera::update_condition() const {
    return shouldUpdate() || prev_frame != window.frame;
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

glm::vec3 Camera::getRight() const {
    return glm::cross(this->readLook(), this->readUp());
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
glm::mat4 const& OrthoCamera::updateProj()  {
    setShouldUpdate();
    glm::vec2 orthoDims = glm::vec2(viewWidth, viewWidth / window.aspect);
    glm::vec4 lrbt = (lrbtMat * orthoDims);
    _proj = glm::ortho(lrbt.x, lrbt.y, lrbt.z, lrbt.w, near, far);
    return _proj;
}

void OrthoCamera::setViewWidth(float vw) {
    setShouldUpdate();
    viewWidth = vw;
}


//********************************ORTHO********************************
PerspectiveCamera::PerspectiveCamera(){
    phi = (3.14159265*0.5); theta = (0);
    pos = glm::vec3(0.f, 0.f, 1.f);
    look = glm::vec3(0.f, 0.f, -1.f);
    up = glm::vec3(0.f, 1.f, 0.f);
    near = 0.0001f;
    far = 1000.0f;
    fov = glm::radians(90.f);
}

PerspectiveCamera::PerspectiveCamera(glm::vec3 p, glm::vec3 l, glm::vec3 u, float n, float f, float fv){
    pos = p; look = l; up = u; near = n; far = f; fov = glm::radians(fv); phi = (3.14159265*0.5); theta = (0);
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

glm::mat4 const& PerspectiveCamera::updateProj(){
    setShouldUpdate();
    _proj = glm::perspective(fov, window.aspect, near, far);
    return _proj;
}

void MousePerspectiveCamera::enable_look(float mult) {
    this->mult = mult;
    window.grab_mouse();
}

void MousePerspectiveCamera::disable_look() {
    mult = 0.;
    window.set_mouse_grab(false);
}

void MousePerspectiveCamera::point(glm::vec3 l) {
    this->look = l;
    l.y = 0;
    l = glm::normalize(l);
    this->up = cross(look, l);
}

MousePerspectiveCamera::MousePerspectiveCamera() : PerspectiveCamera(), sens(0.002)
{
}

MousePerspectiveCamera::MousePerspectiveCamera(glm::vec3 p, glm::vec3 l, glm::vec3 u, float n, float f, float fv) 
: PerspectiveCamera(p, l, u, n, f, fv), sens(0.01)
{
}

void MousePerspectiveCamera::sub_update()
{
    if (mult != 0. && (window.mouse.delta.x != 0 || window.mouse.delta.y != 0)) {
        setShouldUpdate();
        phi += window.mouse.delta.y * sens.y;
        theta -= window.mouse.delta.x * sens.x;
        glm::vec3 anchor = glm::vec3(-glm::cos(theta), 0.0, glm::sin(theta));
        look = glm::vec3(-1 * glm::sin(phi) * glm::sin(theta),
                    glm::cos(phi),
                    -1 * glm::sin(phi) * glm::cos(theta));
        up = cross(look, anchor);
    }
    this->Camera::update();
}

MouseMovePerspectiveCamera::MouseMovePerspectiveCamera(float move, float fly, float boost)
 : move(move), fly(fly), boost(boost) {
}

void MouseMovePerspectiveCamera::set_move(float m) {
    move = m;
}

float MouseMovePerspectiveCamera::get_move() const {
    return move;
}

void MouseMovePerspectiveCamera::set_fly(float m) {
    fly = m;
}

float MouseMovePerspectiveCamera::get_fly() const {
    return fly;
}

void MouseMovePerspectiveCamera::set_boost(float m) {
    boost = m;
}

float MouseMovePerspectiveCamera::get_boost() const {
    return boost;
}

void MouseMovePerspectiveCamera::sub_update() {
}

void MouseMovePerspectiveCamera::update(float dt) {
    this->MousePerspectiveCamera::sub_update();

    if (window.keyboard[GLFW_KEY_TAB].down) {
        dt *= boost;
    }
    if (window.keyboard[GLFW_KEY_W].down) {
        this->getPos() += dt * move * this->readLook();
    }
    if (window.keyboard[GLFW_KEY_A].down) {
        this->getPos() -= dt * move * this->getRight();
    }
    if (window.keyboard[GLFW_KEY_S].down) {
        this->getPos() -= dt * move * this->readLook();
    }
    if (window.keyboard[GLFW_KEY_D].down) {
        this->getPos() += dt * move * this->getRight();
    }
    if (window.keyboard[GLFW_KEY_SPACE].down) {
        this->getPos() += dt * fly * this->readUp();
    }
    if (window.keyboard[GLFW_KEY_LEFT_SHIFT].down) {
        this->getPos() -= dt * fly * this->readUp();
    }
    this->Camera::update();
}
