#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include "physicsmanager.hpp"

namespace Ui
{
class MainWindow;
}

class SceneView;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    void createBall(const glm::vec2& position, const glm::vec2& velocity, float radius, const glm::vec3& color);

private:
    Ui::MainWindow* ui;
    SceneView* scene;
    PhysicsManager* physicsManager;
    float minVelocity{-10.0f};
    float maxVelocity{10.0f};
    float currentVelocityX{-10.0f};
    float currentVelocityY{-10.0f};
    float minRadius{0.5f};
    float maxRadius{2.5f};
    float currentRadius{0.5f};
    float currentRed{0.0f};
    float currentGreen{0.0f};
    float currentBlue{0.0f};
    float rangeColor{255.0f};
    float rangeVelocitiesAndRadius{100.0f};

private slots:
    void on_modeComboBox_currentIndexChanged(int index);
    void on_createBallButton_clicked();
    void on_radiusSlider_valueChanged(int value);
    void on_xVelocitySlider_valueChanged(int value);
    void on_yVelocitySlider_valueChanged(int value);
    void on_redSlider_valueChanged(int value);
    void on_greenSlider_valueChanged(int value);
    void on_blueSlider_valueChanged(int value);
};

#endif // MAINWINDOW_HPP
