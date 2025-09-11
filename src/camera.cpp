#include "camera.h"

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

void Camera::updatePos(float deltaTime)
{
    if (moveForward_ && !moveBack_)
    {
        pos_ += ((sprint_ ? sprintSpeed_ : movSpeed_)*deltaTime) * front_;
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
