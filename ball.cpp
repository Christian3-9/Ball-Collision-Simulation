#include "ball.hpp"

Ball::Ball(const glm::vec2& initialPosition, const glm::vec2& initialVelocity,
           float radius, const glm::vec3& color, float deltaTime)
    : radius(radius), color(color), integrator(initialPosition,
                 initialVelocity, deltaTime) {}

glm::vec2 Ball::getPosition() const
{
    return integrator.getCurrentPosition();
}

float Ball::getRadius() const
{
    return radius;
}

glm::vec3 Ball::getColor() const
{
    return color;
}

void Ball::update(float deltaTime)
{
    integrator.updatePosition(deltaTime);
}

glm::vec2 Ball::getVelocity(float deltaTime) const
{
    return (integrator.getCurrentPosition() - integrator.getPreviousPosition()) / deltaTime;
}
