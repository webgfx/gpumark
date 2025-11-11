# D3D11 Compute Shader Benchmark
add_executable(d3d11_compute
    d3d11_compute.cpp
)

target_link_libraries(d3d11_compute PRIVATE
    ${COMMON_LIBS}
)

# D3D12 Compute Shader Benchmark
add_executable(d3d12_compute
    d3d12_compute.cpp
    d3d12_compute.h
)

target_link_libraries(d3d12_compute PRIVATE
    ${COMMON_LIBS}
)

# Enable C++ exceptions for d3d12_compute
target_compile_options(d3d12_compute PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/EHsc>
)
