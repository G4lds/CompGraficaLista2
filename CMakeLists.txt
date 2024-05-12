cmake_minimum_required(VERSION 3.5)
project(Lista1 VERSION 0.1.0 LANGUAGES C CXX)

cmake_policy(SET CMP0072 NEW)

find_package(OpenGL REQUIRED)

add_executable(projeto_opengl src/main.cpp src/glad.h src/glad.c)

target_link_libraries(projeto_opengl glfw OpenGL::GL)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

