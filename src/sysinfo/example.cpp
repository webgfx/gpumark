#include "SystemInfo.h"
#include <iostream>

int main() {
    // Simple usage - print all system info
    std::cout << SysInfo::GetSystemInfoString() << std::endl;
    
    // Or get individual components
    /*
    auto cpu = SysInfo::GetCPUInfo();
    std::cout << "CPU: " << cpu.brand << std::endl;
    std::cout << "Cores: " << cpu.cores << std::endl;
    
    auto gpus = SysInfo::GetGPUInfo();
    for (const auto& gpu : gpus) {
        std::cout << "GPU: " << gpu.name << std::endl;
        std::cout << "VRAM: " << SysInfo::FormatBytes(gpu.dedicatedMemory) << std::endl;
    }
    
    auto mem = SysInfo::GetMemoryInfo();
    std::cout << "RAM: " << SysInfo::FormatBytes(mem.totalPhysical) << std::endl;
    
    auto os = SysInfo::GetOSInfo();
    std::cout << "OS: " << os.name << " " << os.version << std::endl;
    */
    
    return 0;
}
