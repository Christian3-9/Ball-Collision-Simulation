#include "mainwindow.hpp"
#include "./ui_mainwindow.h"
#include "sceneview.hpp"
#include <QGridLayout>
#include <QDebug>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , physicsManager(new PhysicsManager())
{
    ui->setupUi(this);
    scene = new SceneView(physicsManager);
    QGridLayout* layout = new QGridLayout(ui->frame);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(scene);
    ui->frame->setLayout(layout);
    ui->createBallButton->setEnabled(false);
    ui->xPositionEdit->setValidator(new QDoubleValidator(physicsManager->getMinBoundary().x, physicsManager->getMaxBoundary().x, 2, this));
    ui->yPositionEdit->setValidator(new QDoubleValidator(physicsManager->getMinBoundary().y, physicsManager->getMaxBoundary().y, 2, this));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete physicsManager;
}

void MainWindow::on_modeComboBox_currentIndexChanged(int index) {
    if (index == 0)
    {
        ui->createBallButton->setEnabled(false);
        physicsManager->enableAutomaticMode(false);
    }
    else if (index == 1)
    {
        ui->createBallButton->setEnabled(false);
        physicsManager->enableAutomaticMode(true);
    }
    else if (index == 2)
    {
        ui->createBallButton->setEnabled(true);
        physicsManager->enableAutomaticMode(false);
    }
}

void MainWindow::on_createBallButton_clicked()
{
    if (physicsManager->isAutomaticMode())
    {
        return;
    }

    float xBallPosition = ui->xPositionEdit->text().isEmpty() ? 0.0f : ui->xPositionEdit->text().toFloat();
    float yBallPosition = ui->yPositionEdit->text().isEmpty() ? 0.0f : ui->yPositionEdit->text().toFloat();
    glm::vec2 ballPosition = glm::vec2(xBallPosition, yBallPosition);
    glm::vec2 ballVelocity = glm::vec2(currentVelocityX, currentVelocityY);
    glm::vec3 ballColor = glm::vec3(currentRed, currentGreen, currentBlue);

    createBall(ballPosition, ballVelocity, currentRadius, ballColor);
}

void MainWindow::createBall(const glm::vec2& position, const glm::vec2& velocity, float radius, const glm::vec3& color)
{
    float subDeltaTime = physicsManager->getSubDeltaTime();
    Ball newBall(position, velocity, radius, color, subDeltaTime);
    physicsManager->addBall(newBall);
}

void MainWindow::on_radiusSlider_valueChanged(int value)
{
    currentRadius = minRadius + (value / rangeVelocitiesAndRadius) * (maxRadius - minRadius);
    ui->radiusValueLabel->setText(QString::number(currentRadius, 'f', 2));
}

void MainWindow::on_xVelocitySlider_valueChanged(int value)
{
    currentVelocityX = minVelocity + (value / rangeVelocitiesAndRadius) * (maxVelocity - minVelocity);
    ui->xVelocityValueLabel->setText(QString::number(currentVelocityX, 'f', 2));
}

void MainWindow::on_yVelocitySlider_valueChanged(int value)
{
    currentVelocityY = minVelocity + (value / rangeVelocitiesAndRadius) * (maxVelocity - minVelocity);
    ui->yVelocityValueLabel->setText(QString::number(currentVelocityY, 'f', 2));
}

void MainWindow::on_redSlider_valueChanged(int value)
{
    currentRed = value / rangeColor;
    ui->redValueLabel->setText(QString::number(value));
}

void MainWindow::on_greenSlider_valueChanged(int value)
{
    currentGreen = value / rangeColor;
    ui->greenValueLabel->setText(QString::number(value));
}

void MainWindow::on_blueSlider_valueChanged(int value)
{
    currentBlue = value / rangeColor;
    ui->blueValueLabel->setText(QString::number(value));
}
