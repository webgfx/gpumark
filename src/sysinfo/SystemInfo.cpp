#include "SystemInfo.h"
#include <Windows.h>
#include <dxgi1_6.h>
#include <d3d12.h>
#include <wrl/client.h>
#include <sstream>
#include <iomanip>
#include <intrin.h>

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d12.lib")

using namespace Microsoft::WRL;

namespace SysInfo {

#if defined(_M_X64) || defined(_M_IX86)
// CPUID helper - only available on x86/x64
static void GetCPUID(int function, int subfunction, int* regs) {
    __cpuidex(regs, function, subfunction);
}
#endif

CPUInfo GetCPUInfo() {
    CPUInfo info = {};

#if defined(_M_X64) || defined(_M_IX86)
    // x86/x64 CPU detection using CPUID
    int regs[4] = {0};

    // Get vendor string
    GetCPUID(0, 0, regs);
    char vendor[13] = {0};
    memcpy(vendor, &regs[1], 4);
    memcpy(vendor + 4, &regs[3], 4);
    memcpy(vendor + 8, &regs[2], 4);
    info.vendor = vendor;

    // Get brand string
    char brand[49] = {0};
    for (int i = 0; i < 3; i++) {
        GetCPUID(0x80000002 + i, 0, regs);
        memcpy(brand + i * 16, regs, 16);
    }
    info.brand = brand;

    // Get feature flags
    GetCPUID(1, 0, regs);
    info.supportsSSE = (regs[3] & (1 << 25)) != 0;
    info.supportsSSE2 = (regs[3] & (1 << 26)) != 0;
    info.supportsSSE3 = (regs[2] & (1 << 0)) != 0;
    info.supportsSSE41 = (regs[2] & (1 << 19)) != 0;
    info.supportsSSE42 = (regs[2] & (1 << 20)) != 0;
    info.supportsAVX = (regs[2] & (1 << 28)) != 0;

    // Get extended features
    GetCPUID(7, 0, regs);
    info.supportsAVX2 = (regs[1] & (1 << 5)) != 0;
    info.supportsAVX512 = (regs[1] & (1 << 16)) != 0;

    // Get cache sizes
    GetCPUID(0x80000006, 0, regs);
    info.cacheSizeL1 = ((regs[2] >> 24) & 0xFF);
    info.cacheSizeL2 = ((regs[2] >> 16) & 0xFFFF);

    GetCPUID(0x80000008, 0, regs);
    info.cacheSizeL3 = ((regs[3] >> 18) & 0x3FFF) * 512;
#elif defined(_M_ARM64) || defined(_M_ARM)
    // ARM CPU detection - provide generic info
    info.vendor = "ARM";
    info.brand = "ARM Processor";
    // SSE/AVX not applicable on ARM
    info.supportsSSE = false;
    info.supportsSSE2 = false;
    info.supportsSSE3 = false;
    info.supportsSSE41 = false;
    info.supportsSSE42 = false;
    info.supportsAVX = false;
    info.supportsAVX2 = false;
    info.supportsAVX512 = false;
    // Cache sizes would need ARM-specific methods
    info.cacheSizeL1 = 0;
    info.cacheSizeL2 = 0;
    info.cacheSizeL3 = 0;
#else
    info.vendor = "Unknown";
    info.brand = "Unknown Processor";
#endif

    // Get core count (works on all platforms)
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    info.logicalProcessors = sysInfo.dwNumberOfProcessors;

    // Physical cores
    DWORD length = 0;
    GetLogicalProcessorInformation(nullptr, &length);
    if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
        std::vector<SYSTEM_LOGICAL_PROCESSOR_INFORMATION> buffer(
            length / sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION));
        if (GetLogicalProcessorInformation(buffer.data(), &length)) {
            info.cores = 0;
            for (const auto& proc : buffer) {
                if (proc.Relationship == RelationProcessorCore) {
                    info.cores++;
                }
            }
        }
    }

    return info;
}

std::vector<GPUInfo> GetGPUInfo() {
    std::vector<GPUInfo> gpus;

    ComPtr<IDXGIFactory6> factory;
    if (FAILED(CreateDXGIFactory2(0, IID_PPV_ARGS(&factory)))) {
        return gpus;
    }

    UINT adapterIndex = 0;
    ComPtr<IDXGIAdapter1> adapter;

    while (factory->EnumAdapterByGpuPreference(
        adapterIndex++,
        DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
        IID_PPV_ARGS(&adapter)) != DXGI_ERROR_NOT_FOUND) {

        DXGI_ADAPTER_DESC1 desc;
        if (SUCCEEDED(adapter->GetDesc1(&desc))) {
            GPUInfo info = {};

            // Convert wide string to string
            char name[128];
            WideCharToMultiByte(CP_UTF8, 0, desc.Description, -1, name, sizeof(name), nullptr, nullptr);
            info.name = name;

            info.vendorId = desc.VendorId;
            info.deviceId = desc.DeviceId;
            info.dedicatedMemory = desc.DedicatedVideoMemory;
            info.sharedMemory = desc.SharedSystemMemory;

            // Determine vendor
            switch (desc.VendorId) {
                case 0x10DE: info.vendor = "NVIDIA"; break;
                case 0x1002: info.vendor = "AMD"; break;
                case 0x8086: info.vendor = "Intel"; break;
                case 0x1414: info.vendor = "Microsoft"; break;
                default: info.vendor = "Unknown"; break;
            }

            // Try to create D3D12 device to get feature level and capabilities
            ComPtr<ID3D12Device> device;
            D3D_FEATURE_LEVEL featureLevels[] = {
                D3D_FEATURE_LEVEL_12_2,
                D3D_FEATURE_LEVEL_12_1,
                D3D_FEATURE_LEVEL_12_0,
                D3D_FEATURE_LEVEL_11_1,
                D3D_FEATURE_LEVEL_11_0,
            };

            for (auto level : featureLevels) {
                if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), level, IID_PPV_ARGS(&device)))) {
                    info.featureLevel = static_cast<uint32_t>(level);

                    // Check for ray tracing support
                    D3D12_FEATURE_DATA_D3D12_OPTIONS5 options5 = {};
                    if (SUCCEEDED(device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS5, &options5, sizeof(options5)))) {
                        info.supportsRayTracing = options5.RaytracingTier >= D3D12_RAYTRACING_TIER_1_0;
                    }

                    // Check for variable rate shading
                    D3D12_FEATURE_DATA_D3D12_OPTIONS6 options6 = {};
                    if (SUCCEEDED(device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS6, &options6, sizeof(options6)))) {
                        info.supportsVariableRateShading = options6.VariableShadingRateTier >= D3D12_VARIABLE_SHADING_RATE_TIER_1;
                    }

                    // Check for mesh shaders
                    D3D12_FEATURE_DATA_D3D12_OPTIONS7 options7 = {};
                    if (SUCCEEDED(device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS7, &options7, sizeof(options7)))) {
                        info.supportsMeshShaders = options7.MeshShaderTier >= D3D12_MESH_SHADER_TIER_1;
                    }

                    break;
                }
            }

            // Get driver version
            LARGE_INTEGER version;
            if (SUCCEEDED(adapter->CheckInterfaceSupport(__uuidof(IDXGIDevice), &version))) {
                std::ostringstream oss;
                oss << HIWORD(version.HighPart) << "."
                    << LOWORD(version.HighPart) << "."
                    << HIWORD(version.LowPart) << "."
                    << LOWORD(version.LowPart);
                info.driverVersion = oss.str();
            }

            gpus.push_back(info);
        }

        adapter.Reset();
    }

    return gpus;
}

MemoryInfo GetMemoryInfo() {
    MemoryInfo info = {};

    MEMORYSTATUSEX memStatus;
    memStatus.dwLength = sizeof(memStatus);

    if (GlobalMemoryStatusEx(&memStatus)) {
        info.totalPhysical = memStatus.ullTotalPhys;
        info.availablePhysical = memStatus.ullAvailPhys;
        info.totalVirtual = memStatus.ullTotalVirtual;
        info.availableVirtual = memStatus.ullAvailVirtual;
        info.totalPageFile = memStatus.ullTotalPageFile;
        info.availablePageFile = memStatus.ullAvailPageFile;
        info.memoryLoad = memStatus.dwMemoryLoad;
    }

    return info;
}

OSInfo GetOSInfo() {
    OSInfo info = {};

    // Get Windows version using RtlGetVersion
    typedef LONG(WINAPI* RtlGetVersionPtr)(PRTL_OSVERSIONINFOW);
    HMODULE ntdll = GetModuleHandleW(L"ntdll.dll");
    if (ntdll) {
        RtlGetVersionPtr RtlGetVersion = (RtlGetVersionPtr)GetProcAddress(ntdll, "RtlGetVersion");
        if (RtlGetVersion) {
            RTL_OSVERSIONINFOW osvi = { sizeof(osvi) };
            if (RtlGetVersion(&osvi) == 0) {
                std::ostringstream oss;
                oss << osvi.dwMajorVersion << "." << osvi.dwMinorVersion;
                info.version = oss.str();

                std::ostringstream buildOss;
                buildOss << osvi.dwBuildNumber;
                info.build = buildOss.str();

                // Determine Windows version name
                if (osvi.dwMajorVersion == 10 && osvi.dwBuildNumber >= 22000) {
                    info.name = "Windows 11";
                } else if (osvi.dwMajorVersion == 10) {
                    info.name = "Windows 10";
                } else if (osvi.dwMajorVersion == 6) {
                    if (osvi.dwMinorVersion == 3) info.name = "Windows 8.1";
                    else if (osvi.dwMinorVersion == 2) info.name = "Windows 8";
                    else if (osvi.dwMinorVersion == 1) info.name = "Windows 7";
                }
            }
        }
    }

    // Get architecture
    SYSTEM_INFO sysInfo;
    GetNativeSystemInfo(&sysInfo);
    switch (sysInfo.wProcessorArchitecture) {
        case PROCESSOR_ARCHITECTURE_AMD64:
            info.architecture = "x64";
            break;
        case PROCESSOR_ARCHITECTURE_ARM64:
            info.architecture = "ARM64";
            break;
        case PROCESSOR_ARCHITECTURE_INTEL:
            info.architecture = "x86";
            break;
        case PROCESSOR_ARCHITECTURE_ARM:
            info.architecture = "ARM";
            break;
        default:
            info.architecture = "Unknown";
            break;
    }

    // Check if running under WOW64
    BOOL isWow64 = FALSE;
    IsWow64Process(GetCurrentProcess(), &isWow64);
    info.isWow64 = isWow64 != FALSE;

    return info;
}

SystemInfo GetSystemInfo() {
    SystemInfo info;
    info.cpu = GetCPUInfo();
    info.gpus = GetGPUInfo();
    info.memory = GetMemoryInfo();
    info.os = GetOSInfo();
    return info;
}

std::string FormatBytes(uint64_t bytes) {
    const char* units[] = {"B", "KB", "MB", "GB", "TB"};
    int unitIndex = 0;
    double size = static_cast<double>(bytes);

    while (size >= 1024.0 && unitIndex < 4) {
        size /= 1024.0;
        unitIndex++;
    }

    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << size << " " << units[unitIndex];
    return oss.str();
}

std::string GetSystemInfoString() {
    SystemInfo info = GetSystemInfo();
    std::ostringstream oss;

    oss << "=== System Information ===" << std::endl;

    // CPU Info
    oss << std::endl << "CPU:" << std::endl;
    oss << "  Brand: " << info.cpu.brand << std::endl;
    oss << "  Vendor: " << info.cpu.vendor << std::endl;
    oss << "  Cores: " << info.cpu.cores << " physical, "
        << info.cpu.logicalProcessors << " logical" << std::endl;
    oss << "  Cache: L1=" << info.cpu.cacheSizeL1 << "KB, "
        << "L2=" << info.cpu.cacheSizeL2 << "KB, "
        << "L3=" << info.cpu.cacheSizeL3 << "KB" << std::endl;
    oss << "  Features: ";
    if (info.cpu.supportsSSE) oss << "SSE ";
    if (info.cpu.supportsSSE2) oss << "SSE2 ";
    if (info.cpu.supportsSSE3) oss << "SSE3 ";
    if (info.cpu.supportsSSE41) oss << "SSE4.1 ";
    if (info.cpu.supportsSSE42) oss << "SSE4.2 ";
    if (info.cpu.supportsAVX) oss << "AVX ";
    if (info.cpu.supportsAVX2) oss << "AVX2 ";
    if (info.cpu.supportsAVX512) oss << "AVX512";
    oss << std::endl;

    // GPU Info
    oss << std::endl << "GPU(s):" << std::endl;
    for (size_t i = 0; i < info.gpus.size(); i++) {
        const auto& gpu = info.gpus[i];
        oss << "  [" << i << "] " << gpu.name << std::endl;
        oss << "      Vendor: " << gpu.vendor << " (0x" << std::hex << gpu.vendorId << std::dec << ")" << std::endl;
        oss << "      Device ID: 0x" << std::hex << gpu.deviceId << std::dec << std::endl;
        oss << "      Dedicated Memory: " << FormatBytes(gpu.dedicatedMemory) << std::endl;
        oss << "      Shared Memory: " << FormatBytes(gpu.sharedMemory) << std::endl;
        if (!gpu.driverVersion.empty()) {
            oss << "      Driver Version: " << gpu.driverVersion << std::endl;
        }
        oss << "      Feature Level: 0x" << std::hex << gpu.featureLevel << std::dec << std::endl;
        oss << "      Ray Tracing: " << (gpu.supportsRayTracing ? "Yes" : "No") << std::endl;
        oss << "      Variable Rate Shading: " << (gpu.supportsVariableRateShading ? "Yes" : "No") << std::endl;
        oss << "      Mesh Shaders: " << (gpu.supportsMeshShaders ? "Yes" : "No") << std::endl;
    }

    // Memory Info
    oss << std::endl << "Memory:" << std::endl;
    oss << "  Total Physical: " << FormatBytes(info.memory.totalPhysical) << std::endl;
    oss << "  Available Physical: " << FormatBytes(info.memory.availablePhysical) << std::endl;
    oss << "  Total Virtual: " << FormatBytes(info.memory.totalVirtual) << std::endl;
    oss << "  Available Virtual: " << FormatBytes(info.memory.availableVirtual) << std::endl;
    oss << "  Memory Load: " << info.memory.memoryLoad << "%" << std::endl;

    // OS Info
    oss << std::endl << "Operating System:" << std::endl;
    oss << "  Name: " << info.os.name << std::endl;
    oss << "  Version: " << info.os.version << " (Build " << info.os.build << ")" << std::endl;
    oss << "  Architecture: " << info.os.architecture << std::endl;
    if (info.os.isWow64) {
        oss << "  Running under WOW64: Yes" << std::endl;
    }

    return oss.str();
}

} // namespace SysInfo
