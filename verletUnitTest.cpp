#include <gtest/gtest.h>
#include "verletintegrator.hpp"

TEST(VerletIntegrator, GivenNoInitialVelocity_WhenUpdated_ExpectPositionAffectedByGravityOnly)
{
    glm::vec2 initialPosition(0.0f, 0.0f);
    glm::vec2 initialVelocity(0.0f, 0.0f);
    float deltaTime = 0.01f;

    VerletIntegrator integrator(initialPosition, initialVelocity, deltaTime);

    integrator.updatePosition(deltaTime);

    glm::vec2 expectedPosition = initialPosition + glm::vec2(0.0f, -9.81f) * (deltaTime * deltaTime);
    EXPECT_EQ(integrator.getCurrentPosition().x, expectedPosition.x);
    EXPECT_EQ(integrator.getCurrentPosition().y, expectedPosition.y);
}

TEST(VerletIntegrator, GivenInitialVelocity_WhenUpdated_ExpectPositionAffectedByBothVelocityAndGravity)
{
    glm::vec2 initialPosition(0.0f, 0.0f);
    glm::vec2 initialVelocity(1.0f, 1.0f);
    float deltaTime = 0.01f;

    VerletIntegrator integrator(initialPosition, initialVelocity, deltaTime);

    integrator.updatePosition(deltaTime);

    glm::vec2 expectedPosition = initialPosition + initialVelocity * deltaTime +
                                 glm::vec2(0.0f, -9.81f) * (deltaTime * deltaTime);
    EXPECT_EQ(integrator.getCurrentPosition().x, expectedPosition.x);
    EXPECT_EQ(integrator.getCurrentPosition().y, expectedPosition.y);
}

TEST(VerletIntegrator, GivenUpdatedPosition_WhenReset_ExpectNewPositionSetCorrectly)
{
    glm::vec2 initialPosition(0.0f, 0.0f);
    glm::vec2 initialVelocity(1.0f, 1.0f);
    float deltaTime = 0.01f;

    VerletIntegrator integrator(initialPosition, initialVelocity, deltaTime);

    integrator.updatePosition(deltaTime);

    glm::vec2 newPosition(5.0f, 5.0f);
    integrator.setPosition(newPosition);

    EXPECT_EQ(integrator.getCurrentPosition(), newPosition);
}

TEST(VerletIntegrator, GivenMultipleUpdates_WhenPositionUpdated_ExpectYPositionDecreasing)
{
    glm::vec2 initialPosition(0.0f, 0.0f);
    glm::vec2 initialVelocity(1.0f, 1.0f);
    float deltaTime = 0.01f;

    VerletIntegrator integrator(initialPosition, initialVelocity, deltaTime);

    for (int i = 0; i < 100; ++i)
    {
        integrator.updatePosition(deltaTime);
    }

    glm::vec2 finalPosition = integrator.getCurrentPosition();
    EXPECT_TRUE(finalPosition.y < 0.0f);
}
