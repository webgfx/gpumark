# VP Overlay Benchmark
add_executable(vp_overlay
    vp_overlay.h
    vp_overlay.cpp
)

target_link_libraries(vp_overlay PRIVATE
    ${COMMON_LIBS}
)
