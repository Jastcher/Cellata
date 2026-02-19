cmake_minimum_required(VERSION 3.13)
project(CellularEngine VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# -------------------------------------------------------------------
# 1. ENGINE LIBRARY (Static)
# -------------------------------------------------------------------
# Collect all source files inside the Engine directory
file(GLOB_RECURSE ENGINE_SRC "Engine/*.cpp" "Engine/*.c")

add_library(Engine STATIC ${ENGINE_SRC})

target_include_directories(Engine PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}/Engine/src"             # This lets Sandbox do #include "SomeEngineHeader.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Engine/vendor/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/Engine/vendor/include/imgui"
)
# Use ${CMAKE_CURRENT_SOURCE_DIR} to create an absolute path
target_link_directories(Engine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Engine/vendor/lib")

# Link the library (Note: Use 'glfw3', not 'libglfw3.a')
target_link_libraries(Engine PUBLIC glfw3 GL)
# -------------------------------------------------------------------
# 2. SANDBOX EXECUTABLE (Entry Point)
# -------------------------------------------------------------------
# Collect source files in Sandbox (e.g., main.cpp)
file(GLOB_RECURSE SANDBOX_SRC "Sandbox/*.cpp")

add_executable(Sandbox ${SANDBOX_SRC})

# Link the Sandbox to the Engine
# Because of the 'PUBLIC' include above, Sandbox now "sees" Engine headers automatically.
target_link_libraries(Sandbox PRIVATE Engine)
