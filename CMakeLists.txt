cmake_minimum_required(VERSION 3.0)
project(bresenham_circle)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_EXTENSIONS off)

set(EXECUTABLE ${PROJECT_NAME})

find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(${EXECUTABLE}
    main.cpp)

target_link_libraries(${EXECUTABLE}
    ${GLUT_LIBRARIES}
    ${OPENGL_LIBRARIES})

add_custom_target(run
    COMMAND ${EXECUTABLE}
    DEPENDS ${EXECUTABLE}
    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR})
