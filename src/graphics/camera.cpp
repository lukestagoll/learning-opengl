#include "camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
glm::mat4 Camera::getViewMatrix() const
{
    return glm::lookAt(pos_, pos_ + front_, up_);
}

glm::mat4 Camera::getProjection(float aspectRatio, float nearPlane, float farPlane) const
{
    glm::mat4 projection = glm::mat4(1.0f);
    return glm::perspective(glm::radians(zoom_), aspectRatio, nearPlane, farPlane);
}

void Camera::setSprint(bool sprint)
{
    sprint_ = sprint;
}

void Camera::setForward(bool forward)
{
    moveForward_ = forward;
}

void Camera::setBack(bool back)
{
    moveBack_ = back;
}

void Camera::setLeft(bool left)
{
    moveLeft_ = left;
}

void Camera::setRight(bool right)
{
    moveRight_ = right;
}

void Camera::setYaw(float xrel)
{
    yaw_ += xrel * sensitivity_;
}

void Camera::setPitch(float yrel)
{
    pitch_ -= yrel * sensitivity_;
    if (pitch_ > 89.0f)
    {
        pitch_ = 89.0f;
    }
    if (pitch_ < -89.0f)
    {
        pitch_ = -89.0f;
    }
}

void Camera::updatePos(float deltaTime)
{
    if (moveForward_ && !moveBack_)
    {
        pos_ += ((sprint_ ? sprintSpeed_ : movSpeed_) * deltaTime) * front_;
    }
    else if (moveBack_ && !moveForward_)
    {
        pos_ -= (movSpeed_ * deltaTime) * front_;
    }

    if (moveLeft_ && !moveRight_)
    {
        pos_ -= glm::normalize(glm::cross(front_, up_)) * (movSpeed_ * deltaTime);
    }
    else if (moveRight_ && !moveLeft_)
    {
        pos_ += glm::normalize(glm::cross(front_, up_)) * (movSpeed_ * deltaTime);
    }
}

void Camera::updateDir()
{
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    direction.y = sin(glm::radians(pitch_));
    direction.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    front_ = glm::normalize(direction);
}