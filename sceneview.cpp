#include "sceneview.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <QFile>
#include <QDebug>
#include <QElapsedTimer>
#include <QTimer>

SceneView::SceneView(PhysicsManager* physicsManager, QWidget* parent)
    : QOpenGLWidget(parent), physicsManager(physicsManager)
{
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &SceneView::updatePhysics);
    timer->start(16); // ~60 FPS
}

SceneView::~SceneView()
{
    teardownGL();
}

void SceneView::setup_vertex_data_and_buffers()
{
    float vertices[] =
    {
        1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f
    };

    unsigned int indices[] =
    {
        0, 1, 2,
        2, 1, 3
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void SceneView::initializeGL()
{
    makeCurrent();
    initializeOpenGLFunctions();

    QString vertexShaderPath = read_shader_code_from_resource_file(":/shaders/circles.vert");
    QString fragmentShaderPath = read_shader_code_from_resource_file(":/shaders/circles.frag");
    shaderProgram.set_shader_code(vertexShaderPath.toStdString(), fragmentShaderPath.toStdString());

    QImage textureImage(":/textures/circle.png");
    QImage formattedImage = textureImage.convertToFormat(QImage::Format_RGBA8888);
    texture.create(formattedImage.bits(), formattedImage.width(), formattedImage.height());

    shaderProgram.create();
    shaderProgram.use();
    shaderProgram.setUniform("ourTexture", 0);

    setup_vertex_data_and_buffers();
}

void SceneView::paintGL()
{
    glClearColor(0.0f, 0.3f, 0.7f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shaderProgram.use();
    texture.use();
    glBindVertexArray(VAO);

    auto renderDataList = physicsManager->getRenderData();

    for (const auto& data : renderDataList)
    {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(data.position, 0.0f));

        model = glm::scale(model, glm::vec3(data.radius, data.radius, 1.0f));
        shaderProgram.setUniform("model", model);
        shaderProgram.setUniform("ballColor", data.color);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

    glBindVertexArray(0);
}

void SceneView::updatePhysics()
{
    physicsManager->update();
    update();
}

void SceneView::resizeGL(int width, int height)
{
    float aspectRatio = 1.0f;
    const float minX = physicsManager->getMinBoundary().x;
    const float maxX = physicsManager->getMaxBoundary().x;
    const float minY = physicsManager->getMinBoundary().y;
    const float maxY = physicsManager->getMaxBoundary().y;

    if (aspectRatio > 1.0f)
    {
        projection = glm::ortho(minX * aspectRatio, maxX * aspectRatio, minY, maxY, -1.0f, 1.0f);
    }
    else
    {
        projection = glm::ortho(minX, maxX, minY / aspectRatio, maxY / aspectRatio, -1.0f, 1.0f);
    }

    shaderProgram.use();
    shaderProgram.setUniform("projection", projection);
}

void SceneView::teardownGL()
{
    makeCurrent();
    shaderProgram.destroy();
    texture.destroy();
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

QString SceneView::read_shader_code_from_resource_file(QString filepath)
{
    QString data;
    QFile file(filepath);

    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug()<<"file not opened";
    }
    else
    {
        data = file.readAll();
    }

    return data;
}

void SceneView::printContextInformation()
{
    QString glType{(context()->isOpenGLES()) ? "OpenGL ES" : "OpenGL"};
    QString glVersion{reinterpret_cast<const char*>(glGetString(GL_VERSION))};
    QString glProfile;


#define CASE(c) case QSurfaceFormat::c: glProfile = #c; break

    switch (format().profile())
    {
        CASE(NoProfile);
        CASE(CoreProfile);
        CASE(CompatibilityProfile);
    }

#undef CASE
    // qPrintable() will print our QString w/o quotes around it.
    qDebug() << qPrintable(glType) << qPrintable(glVersion) << "(" << qPrintable(glProfile) << ")";
}
