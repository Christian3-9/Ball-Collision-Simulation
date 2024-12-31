#include "physicsmanager.hpp"
#include "QDebug"

PhysicsManager::PhysicsManager(float deltaTime, float restitutionBalltoBallCollision, float restitutionBoundary)
    : deltaTime(deltaTime), restitutionBalltoBallCollision(restitutionBalltoBallCollision), restitutionBoundary(restitutionBoundary)
{}

void PhysicsManager::addBall(const Ball& ball)
{
    balls.push_back(ball);
}

void PhysicsManager::update()
{
    float subDeltaTime = deltaTime / subSteps;
    static int updateCounter = 0;

    if (automaticMode && updateCounter++ % countRandomCreation == 0 && balls.size() < maximumRandomBalls)
    {
        createRandomBall(subDeltaTime);
        createRandomBall(subDeltaTime);
    }

    for (int step = 0; step < subSteps; ++step)
    {
        checkBoundaryCollisions(subDeltaTime);
        checkBallCollisions(subDeltaTime);
        for (auto& ball : balls)
        {
            ball.update(subDeltaTime);
        }
    }
}

const std::vector<RenderData> PhysicsManager::getRenderData() const
{
    std::vector<RenderData> renderData;
    for (const auto& ball : balls)
    {
        renderData.push_back(
        {
            ball.getPosition(),
            ball.getRadius(),
            ball.getColor()
        });
    }
    return renderData;
}

void PhysicsManager::checkBallCollisions(float subDeltaTime)
{


    for (size_t i = 0; i < balls.size(); ++i)
    {
        for (size_t j = i + 1; j < balls.size(); ++j)
        {
            auto& ball1 = balls[i];
            auto& ball2 = balls[j];
            glm::vec2 delta = ball2.getPosition() - ball1.getPosition();
            float distance = glm::length(delta);
            float combinedRadius = ball1.getRadius() + ball2.getRadius();

            if (distance < combinedRadius)
            {
                glm::vec2 collisionNormal = delta / distance;
                float maxPenetrationDepth = maxPenetrationFraction * combinedRadius;
                float penetrationDepth = glm::clamp(combinedRadius - distance, 0.0f, maxPenetrationDepth);
                float correctionFactor1 = ball2.getRadius() / combinedRadius;
                float correctionFactor2 = ball1.getRadius() / combinedRadius;
                glm::vec2 correction = collisionNormal * penetrationDepth;
                ball1.getIntegrator().setPosition(ball1.getPosition() - correction * correctionFactor1);
                ball2.getIntegrator().setPosition(ball2.getPosition() + correction * correctionFactor2);
                resolveCollision(ball1, ball2, collisionNormal, subDeltaTime);
            }
        }
    }
}

void PhysicsManager::resolveCollision(Ball& ball1, Ball& ball2, const glm::vec2& collisionNormal, float subDeltaTime)
{
    glm::vec2 velocity1 = ball1.getVelocity(subDeltaTime);
    glm::vec2 velocity2 = ball2.getVelocity(subDeltaTime);
    glm::vec2 relativeVelocity = velocity2 - velocity1;
    float velocityAlongNormal = glm::dot(relativeVelocity, collisionNormal);

    if (velocityAlongNormal > 0.0f) return;

    float combinedRadius = ball1.getRadius() + ball2.getRadius();
    float overlap = combinedRadius - glm::length(ball2.getPosition() - ball1.getPosition());
    float restitution = restitutionBalltoBallCollision;
    if (overlap > 0.0f)
    {
        restitution *= glm::clamp(1.0f - overlap / combinedRadius, 0.0f, 1.0f);
    }

    float impulseLimited = glm::clamp(-(1.0f + restitution) * velocityAlongNormal / 2.0f, -10.0f, 10.0f);
    glm::vec2 impulseVector = impulseLimited * collisionNormal;
    float impulseFactor1 = ball1.getRadius() / combinedRadius;
    float impulseFactor2 = ball2.getRadius() / combinedRadius;
    ball1.getIntegrator().setPosition(ball1.getPosition() + impulseVector * impulseFactor1 * subDeltaTime);
    ball2.getIntegrator().setPosition(ball2.getPosition() - impulseVector * impulseFactor2 * subDeltaTime);
}

void PhysicsManager::checkBoundaryCollisions(float subDeltaTime)
{
    for (auto& ball : balls)
    {
        glm::vec2 position = ball.getIntegrator().getCurrentPosition();
        glm::vec2 velocity = (position - ball.getIntegrator().getPreviousPosition()) / subDeltaTime;

        if (position.x < minBoundary.x + ball.getRadius() || position.x > maxBoundary.x - ball.getRadius())
        {
            position.x = glm::clamp(position.x, minBoundary.x + ball.getRadius(), maxBoundary.x - ball.getRadius());
            velocity.x *= -restitutionBoundary;
        }

        if (position.y < minBoundary.y + ball.getRadius() || position.y > maxBoundary.y - ball.getRadius())
        {
            position.y = glm::clamp(position.y, minBoundary.y + ball.getRadius(), maxBoundary.y - ball.getRadius());
            velocity.y *= -restitutionBoundary;
        }

        glm::vec2 correctedPreviousPosition = position - velocity * subDeltaTime;
        ball.getIntegrator().setPosition(position);
        ball.getIntegrator().setPreviousPosition(correctedPreviousPosition);
    }
}

void PhysicsManager::createRandomBall(float subDeltaTime)
{
    float xPositionRandom = minBoundary.x + static_cast<float>(std::rand()) / RAND_MAX * (maxBoundary.x - minBoundary.x);
    float yPositionRandom = minBoundary.y + static_cast<float>(std::rand()) / RAND_MAX * (maxBoundary.y - minBoundary.y);
    glm::vec2 position = glm::vec2(xPositionRandom, yPositionRandom);

    float velocityXRandom = minRandomVelocity + static_cast<float>(std::rand()) / RAND_MAX * rangeRandomVelocity;
    float velocityYRandom = minRandomVelocity + static_cast<float>(std::rand()) / RAND_MAX * rangeRandomVelocity;
    glm::vec2 velocity = glm::vec2(velocityXRandom, velocityYRandom);
    if (glm::length(velocity) > rangeRandomVelocity*0.5f)
    {
        velocity = glm::normalize(velocity) * rangeRandomVelocity*0.5f;
    }

    float redValue = static_cast<float>(std::rand()) / RAND_MAX;
    float greenValue = static_cast<float>(std::rand()) / RAND_MAX;
    float blueValue = static_cast<float>(std::rand()) / RAND_MAX;
    glm::vec3 color = glm::vec3(redValue, greenValue, blueValue);

    balls.emplace_back(position, velocity, radiusRandomBall, color, subDeltaTime);
}

const std::vector<Ball>& PhysicsManager::getBalls() const
{
    return balls;
}

float PhysicsManager::getDeltaTime() const
{
    return deltaTime;
}

float PhysicsManager::getSubDeltaTime() const
{
    return deltaTime/subSteps;
}

glm::vec2 PhysicsManager::getMinBoundary() const
{
    return minBoundary;
}

glm::vec2 PhysicsManager::getMaxBoundary() const
{
    return maxBoundary;
}

void PhysicsManager::enableAutomaticMode(bool enabled)
{
    automaticMode = enabled;
}

bool PhysicsManager::isAutomaticMode() const
{
    return automaticMode;
}
