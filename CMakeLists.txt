cmake_minimum_required(VERSION 3.22.1)

project("opengl_kartsim")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")


find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(glm REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)

include_directories(.include)
include_directories(./include/stb)
include_directories(${OPENGL_INCLUDE_DIR})
include_directories(${GLUT_INCLUDE_DIRS})
include_directories(${GLEW_INCLUDE_DIRS})

add_executable(
    main 
    ../src/main.cpp
    ../src/math/matrix.cpp
    ../src/math/vectors.cpp
    ../src/gameobjects/camera.cpp
    ../src/gameobjects/terrain.cpp
    ../src/gameobjects/gameobjects.cpp
    ../src/graphics/objloader.cpp
    ../src/graphics/skybox.cpp
)


target_link_libraries(main ${OPENGL_LIBRARIES})
target_link_libraries(main ${GLUT_LIBRARIES})
target_link_libraries(main ${GLEW_LIBRARIES})
target_link_libraries(main glfw)
target_link_libraries(main glm::glm)