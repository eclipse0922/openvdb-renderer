cmake_minimum_required(VERSION 3.12)

set(CMAKE_TOOLCHAIN_FILE
    "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake"
    CACHE STRING "Vcpkg toolchain file")

# set the project name
project(openvdb-renderer VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Export compile commands
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


###############################################################
# PACKAGES
###############################################################


# glfw
find_package(glfw3 REQUIRED)
include_directories(${GLFW_INCLUDE_DIRS})
link_libraries(${GLFW_LIBRARY_DIRS})

# opengl
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

# glew
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})

# glm
find_package(glm REQUIRED)
include_directories(${GLM_INCLUDE_DIRS})

# OpenVDB
find_package(OpenVDB REQUIRED)

# imgui
find_package(imgui CONFIG REQUIRED)


###############################################################
# SOURCES
###############################################################

set(SOURCES
  src/main.cpp

  src/scene/Camera.cpp
  src/scene/Light.cpp

  src/app/Application.cpp
  src/app/ApplicationWindow.cpp

  src/geometry/FullScreenVAO.cpp
  src/geometry/VAO.cpp
  src/geometry/IGeometry.cpp
  src/geometry/CubeGeometry.cpp
  src/geometry/MeshUtil.hpp

  src/log/Log.cpp

  src/shader/FragmentShader.cpp
  src/shader/IShader.cpp
  src/shader/Program.cpp
  src/shader/VertexShader.cpp
  src/shader/Material.cpp

  src/vdb/VDBFile.cpp
)

###############################################################
# EXECUTABLE
###############################################################

include_directories(${PROJECT_SOURCE_DIR} src)

# add the executable
add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})

# Warning level
if(MSVC)
    target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE /bigobj /W4 /permissive-)
else()
    target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
endif()
target_link_libraries(
  ${CMAKE_PROJECT_NAME}
  ${OPENGL_LIBRARIES}
  ${GLEW_LIBRARIES}
  glfw
  imgui::imgui
  OpenVDB::openvdb
)
