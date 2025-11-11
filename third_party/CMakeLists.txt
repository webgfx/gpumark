# Third-party dependencies

# GLAD - OpenGL Loader
add_library(glad STATIC
    glad/include/KHR/khrplatform.h
    glad/include/glad/glad.h
    glad/src/glad.c
)

target_include_directories(glad PUBLIC
    glad/include
)

# STB - Image loading library (header-only)
add_library(stb INTERFACE)

target_include_directories(stb INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/stb
)

# ImGui - Immediate mode GUI library
add_library(imgui STATIC
    imgui/imconfig.h
    imgui/imgui.cpp
    imgui/imgui.h
    imgui/imgui_demo.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_internal.h
    imgui/imgui_widgets.cpp
    imgui/imstb_rectpack.h
    imgui/imstb_textedit.h
    imgui/imstb_truetype.h
)

target_include_directories(imgui PUBLIC
    imgui
    glad/include
)

# Add GLFW implementation only if GLFW exists
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/glfw)
    target_sources(imgui PRIVATE
        imgui/examples/imgui_impl_glfw.cpp
        imgui/examples/imgui_impl_glfw.h
    )
endif()

# GLFW - Window and input library
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/glfw)
    # Use GLFW's own CMakeLists.txt but configure it
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
    
    add_subdirectory(glfw)
    
    target_include_directories(imgui PUBLIC
        glfw/include
    )
endif()
