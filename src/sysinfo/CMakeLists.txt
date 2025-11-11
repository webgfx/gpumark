# System Information Library
add_library(sysinfo_lib STATIC
    SystemInfo.cpp
    SystemInfo.h
)

target_link_libraries(sysinfo_lib PRIVATE
    dxgi.lib
    d3d12.lib
)

target_include_directories(sysinfo_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# System info tool
add_executable(sysinfo
    example.cpp
)

target_link_libraries(sysinfo PRIVATE
    sysinfo_lib
)
