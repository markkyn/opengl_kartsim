cmake_minimum_required(VERSION 3.22.1)

project("opengl_kartsim")

set(CMAKE_CXX_STANDARD 11)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

include_directories(../include)
include_directories(${OPENGL_INCLUDE_DIR})
include_directories(${GLUT_INCLUDE_DIRS})

add_executable(
    main 
    ../src/main.cpp
    ../src/math/matrix.cpp
    ../src/math/vectors.cpp
    ../src/gameobjects/camera.cpp
    ../src/gameobjects/gameobjects.cpp
    ../src/graphics/objloader.cpp
)


target_link_libraries(main ${OPENGL_LIBRARIES})
target_link_libraries(main ${GLUT_LIBRARIES})
