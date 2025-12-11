# AethericPropulsion

C++20 Unified Quantum Field Framework (UQFF) - Dual Application Suite

## Overview

This project contains two sophisticated C++ applications:

1. **UQFF Calculator** (`MAIN_1_CoAnQi.cpp`) - Conscious Quantum Intelligence framework with 200+ physics term classes
2. **Scientific Search Interface** (`source2(HEAD PROGRAM).cpp`) - Qt-based scientific database browser with 21 parallel windows

## Project Structure

```
AethericPropulsion/
├── MAIN_1_CoAnQi.cpp                 # 5.8 MB - UQFF Calculator
├── source2(HEAD PROGRAM).cpp         # 104 KB - Qt Scientific Search
├── source176_auto_full_uqff.cpp      # Physics module 176
├── source177_wolfram_field_unity.cpp # Optional Wolfram integration
├── source178_grok_api.cpp            # Grok AI integration
├── CMakeLists.txt                    # Build configuration
├── .gitignore                        # Git ignore rules
└── cmake/
    ├── observational_systems_config.h.in
    └── uqff_tracing.h.in
```

## Build Instructions

### Prerequisites

- **Visual Studio 2022** with C++ workload (Windows)
- **GCC 10+** or **Clang 12+** (Linux)
- **CMake 3.20+**
- **Optional**: Qt5/Qt6, VTK, OpenCV, AWS SDK, vcpkg

### Quick Start (Minimal Build)

```bash
# Clone repository
git clone https://github.com/Daniel8Murphy0007/AethericPropulsion.git
cd AethericPropulsion

# Build UQFF Calculator only (no dependencies required)
mkdir build
cd build
cmake -G "Visual Studio 17 2022" -A x64 -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --config Release

# Run calculator
.\Release\uqff_calculator.exe
```

### Full Build (With Qt GUI)

```bash
# Install dependencies via vcpkg (recommended)
vcpkg install qt6-base qt6-webengine vtk curl sqlite3 opencv nlohmann-json pybind11 aws-sdk-cpp[core,s3,cognito-idp]

# Configure with Qt support
cmake -G "Visual Studio 17 2022" -A x64 ^
  -DCMAKE_BUILD_TYPE=Release ^
  -DUSE_QT=ON ^
  -DUSE_VTK=ON ^
  -DUSE_OPENCV=ON ^
  -DCMAKE_TOOLCHAIN_FILE=[path-to-vcpkg]/scripts/buildsystems/vcpkg.cmake ^
  ..

# Build
cmake --build . --config Release --parallel

# Run scientific search interface
.\Release\scientific_search.exe
```

### Linux Build

```bash
# Install dependencies (Ubuntu/Debian)
sudo apt-get install build-essential cmake qtbase5-dev qtwebengine5-dev \
  libvtk9-dev libcurl4-openssl-dev libsqlite3-dev libopencv-dev \
  nlohmann-json3-dev pybind11-dev

# Configure and build
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release -DUSE_QT=ON -DUSE_VTK=ON ..
cmake --build . --parallel

# Run
./uqff_calculator
```

## UQFF Framework Overview

The Unified Quantum Field Framework implements a novel physics theory with:

### Four Universal Gravity Arrangements (Δ​Ug₁-₄)

1. **Δ​Ug₁ (DPM)** - Di-Pseudo-Monopole dipole-vortex structure
2. **Δ​Ug₂** - Outer field bubble with superconductive repulsive properties
3. **Δ​Ug₃** - Magnetic strings disk at 90° to DPM dipole
4. **Δ​Ug₄** - Star-black hole galactic-scale interactions

### Universal Buoyancy (Δ​Ub_i)

Repulsive support force opposing each gravity arrangement, enabling:

- Redshift/blueshift through vacuum motion
- Galactic structure formation
- Planetary orbit maintenance

### Key Capabilities

- ✅ 200+ PhysicsTerm plugin classes
- ✅ Self-expanding modular architecture
- ✅ Runtime physics term injection
- ✅ Multi-threaded calculations
- ✅ Cross-module state synchronization
- ✅ Observational data validation

## Scientific Search Interface

Qt-based application for querying astronomical databases:

### Features

- 21 parallel browser windows
- NASA API integration (APOD, DONKI, Horizons)
- MAST archive access (Hubble, JWST data)
- JPL Horizons ephemeris calculator
- VTK scientific visualization
- Voice/video input support
- Offline SQLite caching
- AWS cloud synchronization

### API Keys Required

Configure in `source2(HEAD PROGRAM).cpp`:

```cpp
#define NASA_API_KEY_1 "your_nasa_key"
#define MAST_API_KEY "your_mast_key"
#define OPENAI_API_KEY "your_openai_key"
```

Get free API keys:

- NASA: <https://api.nasa.gov/>
- MAST: <https://auth.mast.stsci.edu/>
- OpenAI: <https://platform.openai.com/api-keys>

## CMake Build Options

| Option | Default | Description |
|--------|---------|-------------|
| `USE_QT` | OFF | Build Qt-based GUI applications |
| `USE_VTK` | OFF | Enable VTK visualization |
| `USE_OPENCV` | OFF | Enable OpenCV video input |
| `USE_AWS` | OFF | Enable AWS cloud sync |
| `USE_WOLFRAM` | OFF | Enable Wolfram integration (requires WSTP) |

## Development Status

**Current State**: Active development with production-ready code

- ✅ Core UQFF calculator functional
- ✅ Qt search interface implemented
- ⚠️  Some dependencies optional (graceful degradation)
- 🚧 Missing some module implementations (source176-178 are stubs)
- 🚧 Build system configured but untested with all dependencies

## Testing

```bash
# Run tests (when implemented)
cd build
ctest -C Release --output-on-failure
```

## Contributing

This is a research project. For questions or collaboration:

**Author**: Daniel T. Murphy  
**Email**: <daniel.murphy00@gmail.com>  
**License**: MIT (see LICENSE.md)

## Documentation

- [Copilot Instructions](.github/copilot-instructions.md) - AI agent development guide
- [GitHub Actions](.github/workflows/windows-cmake.yml) - CI/CD pipeline

## Notes

- **"PImath"** and **"BaZaAm"** refer to theoretical physics concepts in UQFF
- **26-level polynomials** relate to quantum state resonance system
- **DPM scaling** governs interactions from nucleus to cosmic distances
- Code includes self-modifying runtime architecture
- Windows-specific threading used for compatibility

## Acknowledgments

- UQFF theory development: Daniel T. Murphy
- Wolfram integration: Optional WSTP SDK
- Grok AI assistance: xAI integration (stub)
- Astronomical data: NASA, STScI, JPL, ESA

---

**Status**: Research code - use at your own risk. Physics theory is non-standard and experimental.
