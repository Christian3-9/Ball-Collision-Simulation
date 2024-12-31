#include "verletintegrator.hpp"
#include <glm/common.hpp>

VerletIntegrator::VerletIntegrator(const glm::vec2& initialPosition, const glm::vec2& initialVelocity, float deltaTime)
    : currentPosition(initialPosition),
    previousPosition(initialPosition - initialVelocity * deltaTime)
{}

void VerletIntegrator::updatePosition(float deltaTime)
{
    glm::vec2 newPosition{0.0f, 0.0f};
    newPosition = currentPosition + (currentPosition - previousPosition) + acceleration * (deltaTime * deltaTime);
    previousPosition = currentPosition;
    currentPosition = newPosition;
}

void VerletIntegrator::setPosition(const glm::vec2& position)
{
    currentPosition = position;
}

glm::vec2 VerletIntegrator::getCurrentPosition() const
{
    return currentPosition;
}

glm::vec2 VerletIntegrator::getPreviousPosition() const
{
    return previousPosition;
}

void VerletIntegrator::setPreviousPosition(const glm::vec2& position)
{
    previousPosition = position;
}

void VerletIntegrator::setAcceleration(const glm::vec2& newAcceleration)
{
    acceleration = newAcceleration;
}
