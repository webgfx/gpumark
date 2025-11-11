# Asteroid VRS and View Instancing Benchmark
add_executable(asteroid
    asteroids_d3d12.cpp
    asteroids_d3d12.h
    camera.cpp
    camera.h
    common_defines.h
    dds.h
    DDSTextureLoader.cpp
    DDSTextureLoader.h
    descriptor.h
    dxil_asteriod_ps_hlsl.h
    dxil_asteriod_vs_hlsl.h
    dxil_font_ps_hlsl.h
    dxil_skybox_ps_hlsl.h
    dxil_skybox_vs_hlsl.h
    dxil_sprite_ps_hlsl.h
    dxil_sprite_vs_hlsl.h
    font.h
    gui.h
    mesh.cpp
    mesh.h
    noise.h
    ScreenGrab12.cpp
    ScreenGrab12.h
    settings.h
    simplexnoise1234.c
    simplexnoise1234.h
    simulation.cpp
    simulation.h
    sprite.h
    subset_d3d12.h
    texture.cpp
    texture.h
    upload_heap.h
    util.h
    WinWrapper.cpp
)

target_link_libraries(asteroid PRIVATE
    ${COMMON_LIBS}
    Ninput.lib
    winmm.lib
)

# Enable C++ exceptions for asteroid
target_compile_options(asteroid PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/EHsc>
)

# Copy asset files to build directory
add_custom_command(TARGET asteroid POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:asteroid>/asteroid
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/directx12.dds
        $<TARGET_FILE_DIR:asteroid>/asteroid/directx12.dds
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/starbox_1024.dds
        $<TARGET_FILE_DIR:asteroid>/asteroid/starbox_1024.dds
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/intel_clear_bd_50_usascii.inl
        $<TARGET_FILE_DIR:asteroid>/intel_clear_bd_50_usascii.inl
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/stb_font_consolas_bold_50_usascii.inl
        $<TARGET_FILE_DIR:asteroid>/stb_font_consolas_bold_50_usascii.inl
    COMMENT "Copying asteroid asset files"
)
