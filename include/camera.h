#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
  public:
    Camera(glm::vec3 pos, glm::vec3 front, glm::vec3 up) : pos_(pos), front_(front), up_(up) {};

    // Matrices
    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjection(float aspect, float nearPlane = 0.1f, float farPlane = 100.0f) const;

    void setSprint(bool sprint);
    void setForward(bool forward);
    void setBack(bool back);
    void setLeft(bool left);
    void setRight(bool right);

    void updatePos(float deltaTime);

  private:
    // Config
    float movSpeed_ = 3.0f;
    float sprintSpeed_ = 6.0f;
    float zoom_ = 45.0f; // degrees (fov)

    // State
    glm::vec3 pos_;
    glm::vec3 front_;
    glm::vec3 up_;

    bool sprint_ = false;
    bool moveForward_ = false;
    bool moveBack_ = false;
    bool moveLeft_ = false;
    bool moveRight_ = false;

    void updateVectors();
};
