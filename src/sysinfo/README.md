# System Information Library

A lightweight C++ library for retrieving detailed system information on Windows, including CPU, GPU, memory, and OS details.

## Features

- **CPU Information**: Vendor, brand, core count, cache sizes, instruction set support (SSE, AVX, etc.)
- **GPU Information**: Name, vendor, memory sizes, driver version, D3D12 feature support (ray tracing, VRS, mesh shaders)
- **Memory Information**: Physical/virtual memory stats, page file usage, memory load percentage
- **OS Information**: Windows version, build number, architecture

## Usage

### Simple Usage

```cpp
#include "SystemInfo.h"
#include <iostream>

int main() {
    // Print all system information
    std::cout << SysInfo::GetSystemInfoString() << std::endl;
    return 0;
}
```

### Detailed Usage

```cpp
#include "SystemInfo.h"
#include <iostream>

int main() {
    // Get CPU information
    auto cpu = SysInfo::GetCPUInfo();
    std::cout << "CPU: " << cpu.brand << std::endl;
    std::cout << "Cores: " << cpu.cores << " physical, " 
              << cpu.logicalProcessors << " logical" << std::endl;
    std::cout << "AVX2 Support: " << (cpu.supportsAVX2 ? "Yes" : "No") << std::endl;
    
    // Get GPU information
    auto gpus = SysInfo::GetGPUInfo();
    for (const auto& gpu : gpus) {
        std::cout << "GPU: " << gpu.name << std::endl;
        std::cout << "VRAM: " << SysInfo::FormatBytes(gpu.dedicatedMemory) << std::endl;
        std::cout << "Ray Tracing: " << (gpu.supportsRayTracing ? "Yes" : "No") << std::endl;
    }
    
    // Get memory information
    auto mem = SysInfo::GetMemoryInfo();
    std::cout << "Total RAM: " << SysInfo::FormatBytes(mem.totalPhysical) << std::endl;
    std::cout << "Available RAM: " << SysInfo::FormatBytes(mem.availablePhysical) << std::endl;
    std::cout << "Memory Load: " << mem.memoryLoad << "%" << std::endl;
    
    // Get OS information
    auto os = SysInfo::GetOSInfo();
    std::cout << "OS: " << os.name << " " << os.version 
              << " (Build " << os.build << ")" << std::endl;
    std::cout << "Architecture: " << os.architecture << std::endl;
    
    // Get everything at once
    auto info = SysInfo::GetSystemInfo();
    // Access via info.cpu, info.gpus, info.memory, info.os
    
    return 0;
}
```

## API Reference

### Data Structures

#### `CPUInfo`
- `vendor` - CPU vendor (e.g., "GenuineIntel", "AuthenticAMD")
- `brand` - CPU brand string
- `cores` - Number of physical cores
- `logicalProcessors` - Number of logical processors (with hyperthreading)
- `cacheSizeL1/L2/L3` - Cache sizes in KB
- `supportsSSE/SSE2/SSE3/SSE41/SSE42/AVX/AVX2/AVX512` - Instruction set flags

#### `GPUInfo`
- `name` - GPU name/description
- `vendor` - Vendor name (NVIDIA, AMD, Intel, Microsoft)
- `vendorId` - PCI vendor ID
- `deviceId` - PCI device ID
- `dedicatedMemory` - Dedicated video memory in bytes
- `sharedMemory` - Shared system memory in bytes
- `driverVersion` - Driver version string
- `featureLevel` - D3D12 feature level
- `supportsRayTracing` - DXR support
- `supportsVariableRateShading` - VRS support
- `supportsMeshShaders` - Mesh shader support

#### `MemoryInfo`
- `totalPhysical/availablePhysical` - Physical RAM in bytes
- `totalVirtual/availableVirtual` - Virtual memory in bytes
- `totalPageFile/availablePageFile` - Page file in bytes
- `memoryLoad` - Memory usage percentage (0-100)

#### `OSInfo`
- `name` - Windows version name (e.g., "Windows 11")
- `version` - Version number (e.g., "10.0")
- `build` - Build number
- `architecture` - CPU architecture (x64, x86, ARM64)
- `isWow64` - Running under WOW64 emulation

### Functions

- `GetSystemInfo()` - Get all system information
- `GetCPUInfo()` - Get CPU information only
- `GetGPUInfo()` - Get GPU information only (returns vector)
- `GetMemoryInfo()` - Get memory information only
- `GetOSInfo()` - Get OS information only
- `FormatBytes(uint64_t)` - Format byte count to human-readable string
- `GetSystemInfoString()` - Get formatted string with all system info

## Building

The library is automatically built as part of the GPUMark project:

```bash
cmake -B build -S . -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release
```

To build just the system info example:

```bash
cmake --build build --config Release --target sysinfo_example
```

## Requirements

- Windows 10 or later
- Visual Studio 2019 or later
- DirectX 12
- CMake 3.15+

## Integration

To use this library in your benchmarks, add it as a dependency in CMakeLists.txt:

```cmake
target_link_libraries(your_benchmark PRIVATE
    sysinfo
)
```

Then include the header:

```cpp
#include "SystemInfo.h"
```

## License

Part of the GPUMark project.
