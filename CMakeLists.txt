cmake_minimum_required(VERSION 3.17)
project(NearestPoints)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(delaunay_triangulation)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS})

add_executable(NearestPoints main.cpp)
target_link_libraries(NearestPoints geometry ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )

add_executable(tests tests.cpp test_runner.h)
target_link_libraries(tests PUBLIC geometry)

add_executable(test_generator test_generator.cpp)
target_link_libraries(test_generator PUBLIC geometry)
