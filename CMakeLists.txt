cmake_minimum_required(VERSION 3.5)

project(VERLETSIMULATOR VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(Qt6 REQUIRED COMPONENTS Widgets OpenGL OpenGLWidgets Core)
find_package(glm REQUIRED)
find_package(GTest REQUIRED)

add_library(Physics STATIC)
target_sources(Physics
    PRIVATE
    verletintegrator.cpp
    physicsmanager.cpp
    ball.cpp

    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS ${PROJECT_SOURCE_DIR}
        FILES
            verletintegrator.hpp
            physicsmanager.hpp
            ball.hpp)
target_link_libraries(Physics
    PUBLIC
    glm::glm
    PRIVATE
    Qt6::Core

)

add_executable(Physics_unitTest
    physicsmanagerUnitTest.cpp)
target_sources(Physics_unitTest
PRIVATE
verletUnitTest.cpp
)
target_link_libraries(Physics_unitTest
Physics
GTest::GTest
GTest::Main
)
target_include_directories(Physics_unitTest
PRIVATE
        "${GTEST_INCLUDE_DIRS}"
)



add_executable(VerletSimulator
    shaderprogram.hpp shaderprogram.cpp
    resources.qrc
    texture.hpp texture.cpp


)
target_sources(VerletSimulator
PRIVATE
main.cpp
mainwindow.cpp
mainwindow.hpp
mainwindow.ui
sceneview.hpp
sceneview.cpp
)
target_link_libraries(VerletSimulator
    PRIVATE
    Qt6::Widgets
    Qt6::OpenGL
    Qt::OpenGLWidgets
    glm::glm
    Physics
)



