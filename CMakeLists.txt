cmake_minimum_required(VERSION 3.0)
project(bresenham)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_EXTENSIONS off)

add_executable(circle
    circle.cpp)

add_executable(line
    line.cpp)

if(WIN32)
    add_library(freeglut STATIC IMPORTED)
    include_directories(${CMAKE_SOURCE_DIR}/../freeglut/include)
    set_property(TARGET freeglut
        PROPERTY IMPORTED_LOCATION
        ${CMAKE_SOURCE_DIR}/../freeglut/lib/freeglut.lib)
    add_custom_command(TARGET circle POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/../freeglut/bin/freeglut.dll
        $<TARGET_FILE_DIR:circle>)

    set(GLUT_LIBRARIES freeglut)
else()
    find_package(GLUT REQUIRED)
    find_package(OpenGL REQUIRED)
endif()

target_link_libraries(circle
    ${GLUT_LIBRARIES}
    ${OPENGL_LIBRARIES})

target_link_libraries(line
    ${GLUT_LIBRARIES}
    ${OPENGL_LIBRARIES})
