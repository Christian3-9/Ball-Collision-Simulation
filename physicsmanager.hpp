#ifndef PHYSICSMANAGER_HPP
#define PHYSICSMANAGER_HPP

#include "ball.hpp"
#include <vector>

struct RenderData
{
    glm::vec2 position;
    float radius;
    glm::vec3 color;
};

class PhysicsManager
{
public:
    PhysicsManager(float deltaTime = 1.0f / 60.0f, float restitutionBalltoBallCollision = 0.8f, float restitutionBoundary = 0.6f);

    void update();

    void addBall(const Ball& ball);
    const std::vector<Ball>& getBalls() const;

    const std::vector<RenderData> getRenderData() const;

    float getDeltaTime() const;
    float getSubDeltaTime() const;
    glm::vec2 getMinBoundary() const;
    glm::vec2 getMaxBoundary() const;


    void enableAutomaticMode(bool enabled);
    bool isAutomaticMode() const;


private:
    std::vector<Ball> balls;
    float deltaTime{1.0f/60.0f};
    int subSteps{6};
    float restitutionBalltoBallCollision{0.8f};
    float restitutionBoundary{0.6f};
    float maxPenetrationFraction{0.1f};

    float minRandomVelocity{-10.0f};
    float rangeRandomVelocity{20.0f};
    float radiusRandomBall{0.50f};
    int maximumRandomBalls{200};
    int countRandomCreation{10};
    glm::vec2 minBoundary{-8.0f, -6.25f};
    glm::vec2 maxBoundary{8.0f, 6.25f};
    bool automaticMode{false};

    void checkBallCollisions(float subDeltaTime);
    void resolveCollision(Ball& ball1, Ball& ball2, const glm::vec2& collisionNormal, float deltaTime);
    void checkBoundaryCollisions(float subDeltaTime);
    void createRandomBall(float subDeltaTime);
};

#endif // PHYSICSMANAGER_HPP
