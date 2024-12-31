#ifndef BALL_HPP
#define BALL_HPP

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include "verletintegrator.hpp"

class Ball
{
public:
    Ball(const glm::vec2& initialPosition, const glm::vec2& initialVelocity, float radius, const glm::vec3& color, float deltaTime);

    void update(float deltaTime);

    glm::vec2 getPosition() const;
    float getRadius() const;
    glm::vec3 getColor() const;
    glm::vec2 getVelocity(float deltaTime) const;

    const VerletIntegrator& getIntegrator() const { return integrator; }
    VerletIntegrator& getIntegrator() { return integrator; }

private:
    float radius;
    glm::vec3 color;
    VerletIntegrator integrator;
};

#endif // BALL_HPP
