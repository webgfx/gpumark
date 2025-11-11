# Aquarium MSAA and Render Pass Benchmark
add_executable(aquarium
    Aquarium.cpp
    Aquarium.h
    Behavior.cpp
    Behavior.h
    Buffer.h
    BufferManager.cpp
    BufferManager.h
    Context.cpp
    Context.h
    ContextFactory.cpp
    ContextFactory.h
    FishModel.cpp
    FishModel.h
    Main.cpp
    Matrix.h
    Model.cpp
    Model.h
    Program.cpp
    Program.h
    ResourceHelper.cpp
    ResourceHelper.h
    SeaweedModel.h
    Texture.cpp
    Texture.h
    AQUARIUM_ASSERT.h
    FPSTimer.cpp
    FPSTimer.h
    d3d12/BufferD3D12.cpp
    d3d12/BufferD3D12.h
    d3d12/ContextD3D12.cpp
    d3d12/ContextD3D12.h
    d3d12/FishModelD3D12.cpp
    d3d12/FishModelD3D12.h
    d3d12/FishModelInstancedDrawD3D12.cpp
    d3d12/FishModelInstancedDrawD3D12.h
    d3d12/GenericModelD3D12.cpp
    d3d12/GenericModelD3D12.h
    d3d12/InnerModelD3D12.cpp
    d3d12/InnerModelD3D12.h
    d3d12/OutsideModelD3D12.cpp
    d3d12/OutsideModelD3D12.h
    d3d12/ProgramD3D12.cpp
    d3d12/ProgramD3D12.h
    d3d12/SeaweedModelD3D12.cpp
    d3d12/SeaweedModelD3D12.h
    d3d12/TextureD3D12.cpp
    d3d12/TextureD3D12.h
    d3d12/imgui_impl_dx12.cpp
    d3d12/imgui_impl_dx12.h
)

target_compile_definitions(aquarium PRIVATE
    ENABLE_D3D12_BACKEND
)

target_link_libraries(aquarium PRIVATE
    ${COMMON_LIBS}
    glad
    imgui
    stb
)

# Include directories for third-party dependencies
target_include_directories(aquarium PRIVATE
    ${CMAKE_SOURCE_DIR}/third_party/glad/include
    ${CMAKE_SOURCE_DIR}/third_party/imgui
    ${CMAKE_SOURCE_DIR}/third_party/imgui/examples
    ${CMAKE_SOURCE_DIR}/third_party/stb
)

# Check if GLFW is available (optional dependency)
if(TARGET glfw)
    target_link_libraries(aquarium PRIVATE glfw)
    target_include_directories(aquarium PRIVATE ${CMAKE_SOURCE_DIR}/third_party/glfw/include)
endif()

# Check if rapidjson is available (optional dependency)
if(EXISTS ${CMAKE_SOURCE_DIR}/third_party/rapidjson/include)
    target_include_directories(aquarium PRIVATE ${CMAKE_SOURCE_DIR}/third_party/rapidjson/include)
endif()

# Copy assets and shaders to build directory
add_custom_command(TARGET aquarium POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/assets
    $<TARGET_FILE_DIR:aquarium>/assets
    COMMENT "Copying aquarium assets to build directory"
)

add_custom_command(TARGET aquarium POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/shaders
    $<TARGET_FILE_DIR:aquarium>/shaders
    COMMENT "Copying aquarium shaders to build directory"
)
