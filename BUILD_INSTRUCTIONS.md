# Build Instructions - AethericPropulsion

Comprehensive guide for building the Unified Quantum Field Framework (UQFF) applications.

## Table of Contents

- [Prerequisites](#prerequisites)
- [Dependency Installation](#dependency-installation)
- [Build Configurations](#build-configurations)
- [Platform-Specific Instructions](#platform-specific-instructions)
- [Troubleshooting](#troubleshooting)
- [Advanced Options](#advanced-options)

---

## Prerequisites

### Required Tools

| Tool | Version | Purpose |
|------|---------|---------|
| **CMake** | 3.20+ | Build system generator |
| **Visual Studio 2022** | 17.0+ | C++ compiler (Windows) |
| **vcpkg** | Latest | Dependency manager |
| **Git** | 2.0+ | Version control |

### Optional Tools

| Tool | Version | Purpose |
|------|---------|---------|
| **Qt 6** | 6.10.0+ | GUI framework (scientific search) |
| **Wolfram Engine** | 14.3 | Symbolic mathematics |
| **Python** | 3.8+ | Build scripts, testing |

---

## Dependency Installation

### Step 1: Install vcpkg

```powershell
# Clone vcpkg
cd C:\
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg

# Bootstrap vcpkg
.\bootstrap-vcpkg.bat

# Add to PATH (optional)
$env:PATH += ";C:\vcpkg"
```

### Step 2: Install Core Dependencies

**Required** (always needed):

```powershell
cd C:\vcpkg
.\vcpkg install curl:x64-windows
.\vcpkg install sqlite3:x64-windows
.\vcpkg install nlohmann-json:x64-windows
```

**Optional** (for full features):

```powershell
# OpenCV (video input, gesture recognition)
.\vcpkg install opencv4[highgui,videoio,dnn,calib3d]:x64-windows

# AWS SDK (cloud sync)
.\vcpkg install aws-sdk-cpp[s3,cognito-idp]:x64-windows

# Note: Qt and VTK must be installed separately (see below)
```

### Step 3: Install Qt 6.10.0 (Optional)

Download and install from: <https://www.qt.io/download-open-source>

**Installation Path**: `C:\Qt\6.10.0\msvc2022_64`

**Components to Select**:

- Qt 6.10.0
  - MSVC 2022 64-bit
  - Qt WebEngine
  - Qt Network
  - Additional Libraries (Charts, Data Visualization)

### Step 4: Install Wolfram Engine (Optional)

Download from: <https://www.wolfram.com/engine/>

**Installation Path**: `C:\Program Files\Wolfram Research\Wolfram Engine\14.3`

**Free for Developers**: Wolfram Engine is free for non-production use.

---

## Build Configurations

### Configuration 1: Minimal Build (Calculator Only)

**What you get**:

- UQFF Calculator (341 PhysicsTerm classes)
- No GUI
- No external dependencies required

**Build time**: ~5-10 minutes

```powershell
mkdir build
cd build

cmake -G "Visual Studio 17 2022" -A x64 `
  -DCMAKE_BUILD_TYPE=Release `
  -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake `
  ..

cmake --build . --config Release --parallel
```

**Output**: `build\Release\uqff_calculator.exe`

---

### Configuration 2: Full Build (All Features)

**What you get**:

- UQFF Calculator
- Qt Scientific Search Interface
- Wolfram Engine integration
- OpenCV video input
- AWS cloud sync
- OpenMP parallel processing

**Build time**: ~15-30 minutes (first build)

```powershell
mkdir build
cd build

cmake -G "Visual Studio 17 2022" -A x64 `
  -DCMAKE_BUILD_TYPE=Release `
  -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake `
  -DUSE_QT=ON `
  -DUSE_OPENCV=ON `
  -DUSE_AWS=ON `
  -DUSE_WOLFRAM=ON `
  -DUSE_OPENMP=ON `
  ..

cmake --build . --config Release --parallel
```

**Outputs**:

- `build\Release\uqff_calculator.exe`
- `build\Release\scientific_search.exe`

---

### Configuration 3: Developer Build (With Debug Info)

```powershell
mkdir build-debug
cd build-debug

cmake -G "Visual Studio 17 2022" -A x64 `
  -DCMAKE_BUILD_TYPE=Debug `
  -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake `
  -DUSE_QT=ON `
  -DUSE_WOLFRAM=ON `
  ..

cmake --build . --config Debug --parallel
```

---

## Platform-Specific Instructions

### Windows (Recommended)

**Prerequisites**:

- Visual Studio 2022 with "Desktop development with C++" workload
- Windows 10/11 (64-bit)

**Build Steps**: See [Build Configurations](#build-configurations) above

**Run Executables**:

```powershell
# Calculator
.\build\Release\uqff_calculator.exe

# Scientific Search (requires Qt)
.\build\Release\scientific_search.exe
```

---

### Linux (Experimental)

**Prerequisites**:

```bash
# Ubuntu/Debian
sudo apt-get update
sudo apt-get install build-essential cmake git

# Core dependencies
sudo apt-get install libcurl4-openssl-dev libsqlite3-dev nlohmann-json3-dev

# Optional: Qt 6
sudo apt-get install qt6-base-dev qt6-webengine-dev

# Optional: OpenCV
sudo apt-get install libopencv-dev
```

**Build Steps**:

```bash
mkdir build && cd build

cmake -DCMAKE_BUILD_TYPE=Release \
  -DUSE_QT=ON \
  ..

cmake --build . --parallel

# Run
./uqff_calculator
```

**Note**: Wolfram Engine WSTP paths are Windows-specific. Linux support requires modification of `CMakeLists.txt` line 64-80.

---

### macOS (Untested)

**Prerequisites**:

```bash
# Install Homebrew
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Install dependencies
brew install cmake curl sqlite3 nlohmann-json opencv
brew install qt@6
```

**Build Steps**: Similar to Linux (see above)

**Compatibility**: Code uses some Windows-specific APIs. Port required for full functionality.

---

## Troubleshooting

### Issue: "CMake Error: Could not find vcpkg toolchain file"

**Solution**: Ensure vcpkg path is correct in CMakeLists.txt or specify manually:

```powershell
cmake -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake ..
```

---

### Issue: "Qt not found"

**Solution**: Add Qt to CMAKE_PREFIX_PATH:

```powershell
cmake -DCMAKE_PREFIX_PATH="C:/Qt/6.10.0/msvc2022_64" -DUSE_QT=ON ..
```

Or edit `CMakeLists.txt` line 76-77 to match your Qt installation path.

---

### Issue: "Wolfram Engine not found"

**Solution**: Verify Wolfram Engine installation path in `CMakeLists.txt` line 67-70:

```cmake
set(WOLFRAM_ROOT "C:/Program Files/Wolfram Research/Wolfram Engine/14.3")
```

Or disable Wolfram integration:

```powershell
cmake -DUSE_WOLFRAM=OFF ..
```

---

### Issue: "WSTP link error" or "wstp64i4.lib not found"

**Solution**: Ensure WSTP library exists at:

```
C:\Program Files\Wolfram Research\Wolfram Engine\14.3\SystemFiles\Links\WSTP\DeveloperKit\Windows-x86-64\CompilerAdditions\wstp64i4.lib
```

Check that Wolfram Engine is properly installed with WSTP SDK.

---

### Issue: "OpenCV not found via vcpkg"

**Solution**: Install OpenCV with specific features:

```powershell
C:\vcpkg\vcpkg install opencv4[highgui,videoio]:x64-windows
```

---

### Issue: Build succeeds but executables crash on startup

**Possible Causes**:

1. Missing DLL files (Qt, OpenCV, AWS SDK)
2. Wolfram Engine not activated
3. Incorrect runtime dependencies

**Solutions**:

**Qt DLLs**: Copy from `C:\Qt\6.10.0\msvc2022_64\bin\` to executable directory:

- Qt6Core.dll
- Qt6Widgets.dll
- Qt6WebEngineWidgets.dll
- Qt6Network.dll

**vcpkg DLLs**: Automatically handled if using vcpkg

**Wolfram Activation**: Run `wolframscript` once to activate license

---

## Advanced Options

### Parallel Builds

Speed up compilation with parallel jobs:

```powershell
# Use all CPU cores
cmake --build . --config Release --parallel

# Specify core count
cmake --build . --config Release --parallel 8
```

---

### Custom Install Prefix

Install to specific directory:

```powershell
cmake -DCMAKE_INSTALL_PREFIX=C:/MyInstall ..
cmake --build . --config Release --target install
```

---

### Enable Verbose Build Output

Debug build issues with verbose output:

```powershell
cmake --build . --config Release --verbose
```

---

### Build Only Specific Target

```powershell
# Build calculator only
cmake --build . --config Release --target uqff_calculator

# Build search interface only
cmake --build . --config Release --target scientific_search
```

---

### CMake Cache Management

Clean CMake cache to reconfigure:

```powershell
# Remove cache
Remove-Item CMakeCache.txt, CMakeFiles -Recurse -Force

# Reconfigure
cmake -G "Visual Studio 17 2022" -A x64 ..
```

---

### Cross-Platform Considerations

**Windows**: Primary development platform, fully tested
**Linux**: Experimental, requires modifications for Wolfram WSTP
**macOS**: Untested, likely requires significant porting

**Windows-Specific Code**:

- WSTP paths (line 67-80 in CMakeLists.txt)
- SimpleMutex threading wrapper
- Windows API calls in source2(HEAD PROGRAM).cpp

---

## Build Performance Tips

1. **Use SSD**: Build on SSD for 2-3x faster builds
2. **Increase RAM**: 16GB+ recommended for parallel builds
3. **Precompiled Headers**: Enabled by default in MSVC
4. **ccache**: Install ccache for faster rebuilds (Linux/macOS)
5. **Ninja**: Use Ninja instead of Visual Studio generator:

   ```powershell
   cmake -G Ninja -DCMAKE_BUILD_TYPE=Release ..
   ninja
   ```

---

## Verification

### Test Calculator

```powershell
.\build\Release\uqff_calculator.exe
```

**Expected Output**:

```
[Logger] UQFF Calculator initializing...
[Logger] Registering 341 PhysicsTerm classes...
[Logger] SOURCE6 modular complete: 29 classes registered (14 graphics + 15 physics)
[Logger] SOURCE4 modular complete: 47 classes registered (24 base + 9 compressed + 13 resonance + 1 wormhole)
[Logger] Wolfram Engine 14.3 WSTP connection established (if USE_WOLFRAM=ON)
[Logger] UQFF Calculator ready
```

---

### Test Scientific Search Interface

```powershell
.\build\Release\scientific_search.exe
```

**Expected Behavior**:

- Qt window opens with 21 browser tabs
- NASA API keys required for full functionality
- Offline mode works without API keys

---

## Documentation

- [README.md](README.md) - Project overview
- [SOURCE4_INTEGRATION_COMPLETE.md](SOURCE4_INTEGRATION_COMPLETE.md) - SOURCE4 architecture
- [SOURCE6_INTEGRATION_COMPLETE.md](SOURCE6_INTEGRATION_COMPLETE.md) - SOURCE6 architecture
- [SOURCE4_SOURCE6_COMBINED_ARCHITECTURE.md](SOURCE4_SOURCE6_COMBINED_ARCHITECTURE.md) - Combined framework
- [FUTURE_SOURCE_INTEGRATION_REFERENCE.md](FUTURE_SOURCE_INTEGRATION_REFERENCE.md) - Integration template

---

## Support

**Issues**: <https://github.com/Daniel8Murphy0007/AethericPropulsion/issues>

**Author**: Daniel T. Murphy
**License**: MIT

---

**Last Updated**: December 11, 2025
**Build System Version**: CMake 3.20+ with vcpkg integration
**Tested Configurations**: Windows 11 + Visual Studio 2022 + vcpkg
