#pragma once

#include <string>
#include <vector>
#include <cstdint>

namespace SysInfo {

struct CPUInfo {
    std::string vendor;
    std::string brand;
    int cores;
    int logicalProcessors;
    int cacheSizeL1;  // KB
    int cacheSizeL2;  // KB
    int cacheSizeL3;  // KB
    bool supportsSSE;
    bool supportsSSE2;
    bool supportsSSE3;
    bool supportsSSE41;
    bool supportsSSE42;
    bool supportsAVX;
    bool supportsAVX2;
    bool supportsAVX512;
};

struct GPUInfo {
    std::string name;
    std::string vendor;
    uint32_t vendorId;
    uint32_t deviceId;
    uint64_t dedicatedMemory;  // Bytes
    uint64_t sharedMemory;     // Bytes
    std::string driverVersion;
    uint32_t featureLevel;     // D3D Feature Level
    bool supportsRayTracing;
    bool supportsVariableRateShading;
    bool supportsMeshShaders;
};

struct MemoryInfo {
    uint64_t totalPhysical;      // Bytes
    uint64_t availablePhysical;  // Bytes
    uint64_t totalVirtual;       // Bytes
    uint64_t availableVirtual;   // Bytes
    uint64_t totalPageFile;      // Bytes
    uint64_t availablePageFile;  // Bytes
    int memoryLoad;              // Percentage (0-100)
};

struct OSInfo {
    std::string name;
    std::string version;
    std::string build;
    std::string architecture;
    bool isWow64;
};

struct SystemInfo {
    CPUInfo cpu;
    std::vector<GPUInfo> gpus;
    MemoryInfo memory;
    OSInfo os;
};

// Get complete system information
SystemInfo GetSystemInfo();

// Individual getters
CPUInfo GetCPUInfo();
std::vector<GPUInfo> GetGPUInfo();
MemoryInfo GetMemoryInfo();
OSInfo GetOSInfo();

// Helper functions
std::string FormatBytes(uint64_t bytes);
std::string GetSystemInfoString();

} // namespace SysInfo
