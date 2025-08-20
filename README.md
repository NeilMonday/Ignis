# Ignis (submodules edition)

This starter uses **git submodules** for all open-source dependencies:

- GLFW → `external/glfw` (3.4)
- Dear ImGui (docking) → `external/imgui`
- GLM → `external/glm`
- spdlog → `external/spdlog`
- Vulkan Memory Allocator (VMA) → `external/VulkanMemoryAllocator`

## Clone & init
```bash
git clone git@github.com:<you>/Ignis.git
cd Ignis
git submodule update --init --recursive
```

## Build (Windows, VS 2022)
```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64 -DUSE_VALIDATION=ON -DTRIPLE_BUFFERING=ON -DENABLE_HDR=ON
cmake --build build --config Release
.uild\Release\Ignis.exe
```

## Build (Linux)
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DUSE_VALIDATION=ON -DTRIPLE_BUFFERING=ON -DENABLE_HDR=ON
cmake --build build -j
./build/Ignis
```

If CMake errors about missing submodules, run the `git submodule` command above.
