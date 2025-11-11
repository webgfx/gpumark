# N-Body Async Compute Benchmark
add_executable(nbody WIN32
    Main.cpp
    Win32Application.h
    Win32Application.cpp
    DXSample.cpp
    DXSample.h
    DXSampleHelper.h
    D3D12nBodyGravity.cpp
    D3D12nBodyGravity.h
    StepTimer.h
    SimpleCamera.cpp
    SimpleCamera.h
    d3dx12.h
)

target_link_libraries(nbody PRIVATE
    ${COMMON_LIBS}
)

# Add include directories - nbody directory first for d3dx12.h, then common includes
target_include_directories(nbody PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src/include
)

# Enable C++ exceptions for nbody
target_compile_options(nbody PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/EHsc>
)

# Copy shader files to build directory
add_custom_command(TARGET nbody POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_CURRENT_SOURCE_DIR}/nBodyGravityCS.hlsl
    $<TARGET_FILE_DIR:nbody>/nBodyGravityCS.hlsl
    COMMENT "Copying nBodyGravityCS.hlsl"
)

add_custom_command(TARGET nbody POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_CURRENT_SOURCE_DIR}/ParticleDraw.hlsl
    $<TARGET_FILE_DIR:nbody>/ParticleDraw.hlsl
    COMMENT "Copying ParticleDraw.hlsl"
)
