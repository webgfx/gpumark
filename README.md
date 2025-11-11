# GPUMark
GPUMark is a performance test suite for GPU benchmarking.

<table>
  <tr align=center>
    <td><strong>Feature</td>
    <td><strong>Benchmark</td>
    <td><strong>Description</td>
  </tr>
  <tr align=left>
    <td>MultiSample Anti-Aliasing (MSAA)</td>
    <td>aquarium</td>
    <td></td>
  </tr>
  <tr align=left>
    <td>Render Pass</td>
    <td>aquarium</td>
    <td></td>
  </tr>
  <tr align=left>
    <td>Variable Rate Shading (VRS)</td>
    <td>asteroid</td>
    <td>https://gitlab.devtools.intel.com/web/asteroids_vrs_vi</td>
  </tr>
  <tr align=left>
    <td>View Instancing (VI)</td>
    <td>asteroid</td>
    <td>https://gitlab.devtools.intel.com/web/asteroids_vrs_vi</td>
  </tr>
  <tr align=left>
    <td>Compute Shader</td>
    <td>d3d11_compute, d3d12_compute</td>
    <td></td>
  </tr>
  <tr align=left>
    <td>Overlay</td>
    <td>vp_overlay</td>
    <td></td>
  </tr>
  <tr align=left>
    <td>Async Compute</td>
    <td>nbody</td>
    <td>https://github.com/GPUOpen-LibrariesAndSDKs/nBodyD3D12/tree/master/Samples/D3D12nBodyGravity</td>
  </tr>
</table>

## Requirements

- **Windows 10/11** (ARM64 or x64)
- **CMake** 3.15 or later
- **Visual Studio 2022** with C++ desktop development workload
- **DirectX 12** compatible GPU

## Building

### Windows (PowerShell)

1. Clone the repository:
```powershell
git clone <repository-url>
cd gpumark
```

2. Create a build directory and configure:
```powershell
mkdir build
cd build
cmake ..
```

3. Build all benchmarks:
```powershell
cmake --build . --config Release
```

Or build a specific benchmark:
```powershell
cmake --build . --config Release --target aquarium
cmake --build . --config Release --target asteroid
cmake --build . --config Release --target d3d12_compute
cmake --build . --config Release --target nbody
cmake --build . --config Release --target sysinfo
cmake --build . --config Release --target vp_overlay
```

## Running Benchmarks

### Aquarium
A fish rendering benchmark testing MSAA and render pass features:
```powershell
cd build\src\aquarium\Release
.\aquarium.exe
```

**Command-line options:**
- `--backend d3d12` - Use Direct3D 12 backend (default)
- `--fish-count <number>` - Number of fish to render (default: 30000)
- `--msaa-count <1|2|4|8>` - MSAA sample count (default: 4)
- `--test-time <seconds>` - Auto-stop after specified seconds
- `--enable-full-screen-mode` - Run in fullscreen
- `--window-size=<width>,<height>` - Set window dimensions

Example:
```powershell
.\aquarium.exe --backend d3d12 --fish-count 10000 --msaa-count 4 --test-time 60
```

### Asteroid
Variable Rate Shading and View Instancing benchmark:
```powershell
cd build\src\asteroid\Release
.\asteroid.exe
```

### Compute Shader Tests
Direct3D 12 compute shader performance:
```powershell
cd build\src\compute\Release
.\d3d12_compute.exe
```

### N-Body Simulation
Async compute benchmark:
```powershell
cd build\src\nbody\Release
.\nbody.exe
```

### System Information
Display GPU and system information:
```powershell
cd build\src\sysinfo\Release
.\sysinfo.exe
```

## Interpreting Results

### Aquarium Output
```
[RESULT] RENDERPASS:False,MSAA:4,FPS:22
```
- **RENDERPASS**: Whether D3D12 render pass is enabled
- **MSAA**: Multi-sample anti-aliasing sample count
- **FPS**: Average frames per second

## Known Issues

- **ARM64 Support**: The project has been fixed to build and run on ARM64 Windows (e.g., Qualcomm Snapdragon X Elite/Plus)
- **Texture Loading**: Fixed mipmap generation crash for non-square textures
- **GPU Detection**: Qualcomm Adreno GPUs are now properly detected as integrated GPUs

## Platform Notes

### ARM64 Windows
The project fully supports ARM64 Windows with Qualcomm Adreno GPUs. The build system automatically detects the platform and uses appropriate CPU detection methods.

### Supported GPUs
- Intel integrated and discrete GPUs
- AMD GPUs
- NVIDIA GPUs
- Qualcomm Adreno X1 series (ARM64)

## License

See LICENSE file for details.
