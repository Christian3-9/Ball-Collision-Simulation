#include <gtest/gtest.h>
#include "physicsmanager.hpp"
#include "ball.hpp"

TEST(PhysicsManager, GivenInitializedManager_WhenGettingParameters_ExpectCorrectValues)
{
    float deltaTime = 1.0f / 60.0f;
    int subSteps = 6;
    glm::vec2 minBoundary(-8.0f, -6.25f);
    glm::vec2 maxBoundary(8.0f, 6.25f);

    PhysicsManager manager(deltaTime, 0.8f, 0.9f);

    EXPECT_FLOAT_EQ(manager.getDeltaTime(), deltaTime);
    EXPECT_FLOAT_EQ(manager.getSubDeltaTime(), deltaTime / subSteps);
    EXPECT_EQ(manager.getMinBoundary(), minBoundary);
    EXPECT_EQ(manager.getMaxBoundary(), maxBoundary);
}

TEST(PhysicsManager, GivenNoBalls_WhenAddingOneBall_ExpectBallAddedCorrectly)
{
    float deltaTime = 1.0f / 60.0f;
    PhysicsManager manager(deltaTime, 0.8f, 0.9f);

    glm::vec2 ballPosition{0.0f,0.0f};
    glm::vec2 ballInitialVelocity{1.0f, 1.0f};
    float ballRadius{0.5f};
    glm::vec3 ballColor{1.0f,0.0f,0.0f};

    Ball ball(ballPosition, ballInitialVelocity, ballRadius, ballColor, deltaTime);
    manager.addBall(ball);

    EXPECT_EQ(manager.getBalls().size(), 1);
    EXPECT_EQ(manager.getBalls()[0].getPosition(), glm::vec2(0.0f, 0.0f));
}

TEST(PhysicsManager, GivenBallAtRightBoundary_WhenUpdated_ExpectBallStaysInsideDomain)
{
    float deltaTime = 1.0f / 60.0f;
    PhysicsManager manager(deltaTime, 0.8f, 0.9f);

    glm::vec2 maxBoundary = manager.getMaxBoundary();

    glm::vec2 ballPosition{maxBoundary.x + 0.5f, 0.0f};
    glm::vec2 ballInitialVelocity{1.0f, 0.0f};
    float ballRadius{0.5f};
    glm::vec3 ballColor{1.0f,0.0f,0.0f};

    Ball ballRight(ballPosition, ballInitialVelocity, ballRadius, ballColor, deltaTime);
    manager.addBall(ballRight);

    manager.update();

    glm::vec2 positionRight = manager.getBalls()[0].getPosition();
    EXPECT_LE(positionRight.x, maxBoundary.x);
}

TEST(PhysicsManager, GivenBallAtLeftBoundary_WhenUpdated_ExpectBallStaysInsideDomain)
{
    float deltaTime = 1.0f / 60.0f;
    PhysicsManager manager(deltaTime, 0.8f, 0.9f);

    glm::vec2 minBoundary = manager.getMinBoundary();

    glm::vec2 ballPosition{minBoundary.x - 0.5f, 0.0f};
    glm::vec2 ballInitialVelocity{-1.0f, 0.0f};
    float ballRadius{0.5f};
    glm::vec3 ballColor{1.0f,0.0f,0.0f};

    Ball ballLeft(ballPosition, ballInitialVelocity, ballRadius, ballColor, deltaTime);
    manager.addBall(ballLeft);

    manager.update();

    glm::vec2 positionLeft = manager.getBalls()[0].getPosition();
    EXPECT_GE(positionLeft.x, minBoundary.x);
}

TEST(PhysicsManager, GivenBallAtTopBoundary_WhenUpdated_ExpectBallStaysInsideDomain)
{
    float deltaTime = 1.0f / 60.0f;
    PhysicsManager manager(deltaTime, 0.8f, 0.9f);

    glm::vec2 maxBoundary = manager.getMaxBoundary();

    glm::vec2 ballPosition{0.0f, maxBoundary.y + 0.5f};
    glm::vec2 ballInitialVelocity{0.0f, 1.0f};
    float ballRadius{0.5f};
    glm::vec3 ballColor{1.0f,0.0f,0.0f};


    Ball ballTop(ballPosition, ballInitialVelocity, ballRadius, ballColor, deltaTime);
    manager.addBall(ballTop);

    manager.update();

    glm::vec2 positionTop = manager.getBalls()[0].getPosition();
    EXPECT_LE(positionTop.y, maxBoundary.y);
}

TEST(PhysicsManager, GivenBallAtBottomBoundary_WhenUpdated_ExpectBallStaysInsideDomain)
{
    float deltaTime = 1.0f / 60.0f;
    PhysicsManager manager(deltaTime, 0.8f, 0.9f);

    glm::vec2 minBoundary = manager.getMinBoundary();

    glm::vec2 ballPosition{0.0f, minBoundary.y - 0.5f};
    glm::vec2 ballInitialVelocity{0.0f, -1.0f};
    float ballRadius{0.5f};
    glm::vec3 ballColor{1.0f,0.0f,0.0f};

    Ball ballBottom(ballPosition, ballInitialVelocity, ballRadius, ballColor, deltaTime);
    manager.addBall(ballBottom);

    manager.update();

    glm::vec2 positionBottom = manager.getBalls()[0].getPosition();
    EXPECT_GE(positionBottom.y, minBoundary.y);
}


TEST(PhysicsManager, GivenTwoBalls_WhenColliding_ExpectCorrectVelocityChange)
{
    float deltaTime = 1.0f / 60.0f;
    PhysicsManager manager(deltaTime, 0.9f, 0.8f);

    glm::vec2 ball1Position{0.0f, 0.0f};
    glm::vec2 ball1InitialVelocity{1.0f, 0.0f};
    glm::vec2 ball2Position{0.8f, 0.0f};
    glm::vec2 ball2InitialVelocity{-1.0f, 0.0f};
    float ballRadius{0.5f};
    glm::vec3 ballColor{1.0f,0.0f,0.0f};

    Ball ball1(ball1Position, ball1InitialVelocity, ballRadius, ballColor, deltaTime);
    Ball ball2(ball2Position, ball2InitialVelocity, ballRadius, ballColor, deltaTime);

    manager.addBall(ball1);
    manager.addBall(ball2);

    manager.update();

    glm::vec2 velocity1 = manager.getBalls()[0].getVelocity(deltaTime);
    glm::vec2 velocity2 = manager.getBalls()[1].getVelocity(deltaTime);

    EXPECT_LT(velocity1.x, 0.0f); // Ball 1 should move left
    EXPECT_GT(velocity2.x, 0.0f); // Ball 2 should move right
}

TEST(PhysicsManager, GivenAutomaticModeEnabled_WhenUpdated_ExpectRandomBallsCreated)
{
    float deltaTime = 1.0f / 60.0f;
    PhysicsManager manager(deltaTime, 0.8f, 0.9f);

    manager.enableAutomaticMode(true);
    manager.update();

    EXPECT_GT(manager.getBalls().size(), 0);
}

TEST(PhysicsManager, GivenMultipleBalls_WhenRendering_ExpectCorrectRenderData)
{
    float deltaTime = 1.0f / 60.0f;
    PhysicsManager manager(deltaTime, 0.8f, 0.9f);

    glm::vec2 ball1Position{0.0f, 0.0f};
    glm::vec2 ball1InitialVelocity{1.0f, 0.0f};
    glm::vec2 ball2Position{0.8f, 0.0f};
    glm::vec2 ball2InitialVelocity{-1.0f, 0.0f};
    float ballRadius{0.5f};
    glm::vec3 ballColor{1.0f,0.0f,0.0f};

    Ball ball1(ball1Position, ball1InitialVelocity, ballRadius, ballColor, deltaTime);
    Ball ball2(ball2Position, ball2InitialVelocity, ballRadius, ballColor, deltaTime);

    manager.addBall(ball1);
    manager.addBall(ball2);

    auto renderData = manager.getRenderData();

    EXPECT_EQ(renderData.size(), 2);
    EXPECT_EQ(renderData[0].position, ball1.getPosition());
    EXPECT_EQ(renderData[1].position, ball2.getPosition());
}



