#ifndef SCENEVIEW_HPP
#define SCENEVIEW_HPP

#include "shaderprogram.hpp"
#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <glm/glm.hpp>
#include <QElapsedTimer>
#include "VerletIntegrator.hpp"
#include "texture.hpp"
#include "vector"
#include "physicsmanager.hpp"
#include <QTimer>

class SceneView : public QOpenGLWidget, protected QOpenGLExtraFunctions
{
    Q_OBJECT

public:
    SceneView(PhysicsManager* physicsManager, QWidget* parent = nullptr);
    ~SceneView();

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;
    void teardownGL();

private:
    QString read_shader_code_from_resource_file(QString filepath);
    void printContextInformation();
    void setup_vertex_data_and_buffers();

    unsigned int VBO{0};
    unsigned int VAO{0};
    unsigned int EBO{0};

    QImage textureImage;
    Texture texture;
    glm::mat4 projection;
    ShaderProgram shaderProgram;

    std::vector<VerletIntegrator> balls;

    PhysicsManager* physicsManager;

    QTimer *timer;
    QElapsedTimer elapsedTimer;

private slots:
    void updatePhysics();
};

#endif // SCENEVIEW_HPP
