cmake_minimum_required(VERSION 3.15)
project(GPUMark LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)

# Windows-specific settings
if(WIN32)
    add_definitions(-DUNICODE -D_UNICODE -DNOMINMAX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3")
endif()

# Common libraries for all benchmarks
set(COMMON_LIBS
    d3d11.lib
    dxgi.lib
    d3dcompiler.lib
    dxguid.lib
    d3d12.lib
    Kernel32.lib
    user32.lib
    Shcore.lib
)

# Common include directories
include_directories(${CMAKE_SOURCE_DIR}/src/include)

# Add third party dependencies
add_subdirectory(third_party)

# Add system info library
add_subdirectory(src/sysinfo)

# Add benchmark subdirectories
add_subdirectory(src/compute)
add_subdirectory(src/vp_overlay)
add_subdirectory(src/nbody)
add_subdirectory(src/asteroid)

# Aquarium requires external dependencies (GLFW, rapidjson, stb)
# Only build if dependencies are present
if(EXISTS ${CMAKE_SOURCE_DIR}/third_party/glfw AND 
   EXISTS ${CMAKE_SOURCE_DIR}/third_party/rapidjson AND
   EXISTS ${CMAKE_SOURCE_DIR}/third_party/stb)
    add_subdirectory(src/aquarium)
else()
    message(STATUS "Skipping aquarium benchmark (missing dependencies)")
endif()
