//
//  Camera.h
//  opengl-tools
//
//  Created by Frank Collebrusco on 12/27/22.
//
/*
 Camera object used to generate view and perspective matricies for vertex shaders.
 Position, Look, and Up vectors locate camera in space (view matrix)
 View Width and FOV affect the projection matricies of Orthographic and Perspective cameras respectively
 These object follow get/set/read convention, wherein get returns a non-const reference to a value. use read for const access
 This way you can use something like:
    cam.getPos().x += 3;
 And
    cam.readPos();
 provides a const reference that doesn't signal for an update.
 */

#ifndef Camera_h
#define Camera_h
#include <flgl/glm.h>

class Camera {
private:
    bool should_update;
    union {
        struct {
            bool view : 4;
            bool proj : 4;
        } mat;
        uint8_t val;
    } needs_inverses;
protected:
    glm::ivec2 prev_frame;
    glm::vec3 pos, look, up;
    glm::mat4 _view;
    glm::mat4 _proj;
    glm::mat4 _iview;
    glm::mat4 _iproj;
    float near, far;
public:
    Camera();
    virtual ~Camera() = default;
    
    void setShouldUpdate();
    bool shouldUpdate() const;
    virtual bool update_condition() const;
    glm::vec3& getPos();
    glm::vec3& getLook();
    glm::vec3& getUp();
    float& getNear();
    float& getFar();
    glm::vec3 const& readPos() const;
    glm::vec3 const& readLook() const;
    glm::vec3 const& readUp() const;
    float readNear() const;
    float readFar() const;
    
    virtual void update(); // if you override this, call this->Camera::update() from it
    glm::mat4 const& updateView();
    virtual glm::mat4 const& updateProj() = 0;
    glm::mat4 const& view() const;
    glm::mat4 const& proj() const;
    glm::mat4 const& iview();
    glm::mat4 const& iproj();
};

class OrthoCamera : public Camera {
protected:
    constexpr const static glm::mat2x4 lrbtMat = { glm::vec4(-0.5f, 0.5f, 0, 0),
                                                   glm::vec4(0, 0, -0.5f, 0.5f) };
    float viewWidth;
public:
    OrthoCamera();
    OrthoCamera(const OrthoCamera&) = default;
    OrthoCamera(glm::vec3 p, glm::vec3 l, glm::vec3 u, float n, float f, float w);
    void setViewWidth(float vw);
    float& getViewWidth();
    float readViewWidth() const;
    virtual glm::mat4 const& updateProj() override;
};

class PerspectiveCamera : public Camera {
    float fov;
    bool mouseControlled;
public:
    float phi, theta;
    PerspectiveCamera();
    PerspectiveCamera(const PerspectiveCamera&) = default;
    PerspectiveCamera(glm::vec3 p, glm::vec3 l, glm::vec3 u, float n, float f, float fv);
    void setFOV(float fv);
    float& getFOV();
    float readFOV() const;
    virtual glm::mat4 const& updateProj() override final;
    void setMouseControl(bool);
    void update() override;
};

#endif /* Camera_h */
