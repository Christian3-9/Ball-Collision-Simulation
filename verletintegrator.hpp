#ifndef VERLETINTEGRATOR_HPP
#define VERLETINTEGRATOR_HPP

#include <glm/vec2.hpp>
#include <glm/glm.hpp>

class VerletIntegrator
{
public:
    VerletIntegrator(const glm::vec2& initialPosition, const glm::vec2& initialVelocity, float deltaTime);

    void updatePosition(float deltaTime);

    void setPosition(const glm::vec2& position);
    void setPreviousPosition(const glm::vec2& position);
    void setAcceleration(const glm::vec2& newAcceleration);

    glm::vec2 getPreviousPosition() const;
    glm::vec2 getCurrentPosition() const;

private:
    glm::vec2 acceleration{0.0f, -9.81f};
    glm::vec2 currentPosition{0.0f,0.0f};
    glm::vec2 previousPosition{0.0f,0.0f};
    glm::vec2 velocity{0.0f,0.0f};

};

#endif // VERLETINTEGRATOR_HPP
