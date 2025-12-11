# AethericPropulsion - AI Agent Instructions

## Project Status & Architecture

**Current State**: Active C++ development with two major applications - **CODE NOW EXISTS**

**Actual Project**: C++20 Unified Quantum Field Framework (UQFF) with dual applications:
1. **MAIN_1_CoAnQi.cpp** (5.8 MB) - Conscious Quantum Intelligence Calculator
2. **source2(HEAD PROGRAM).cpp** (104 KB) - Qt-based Scientific Search Interface

**Critical Reality Check**: The README files describe aspirational Python projects that don't exist. The actual codebase is sophisticated C++20 with Qt, VTK, and extensive physics frameworks.

## Technology Stack (ACTUAL - from existing code)

### Primary Language: C++20
- **Compiler**: MSVC 14.44+ (Visual Studio 2022)
- **Standard**: C++20 features used throughout
- **Platform**: Windows primary (extensive Windows API usage)

### Major Dependencies (REQUIRED for build)

#### GUI & Visualization
- **Qt Framework** (Qt5/Qt6)
  - QApplication, QMainWindow, QWebEngineView, QTabWidget
  - QDialog, QDockWidget, QToolBar
  - Network: QCoreApplication for async operations
- **VTK (Visualization Toolkit)**
  - vtkSmartPointer, vtkScatterPlotMatrix, vtkChartXY
  - Scientific data visualization (scatter plots, charts)

#### Networking & Data
- **libcurl** - HTTP/HTTPS API requests (NASA, MAST, JPL Horizons)
- **WebSocket** - Real-time data streams (LIGO, JWST)
- **SQLite3** - Local caching and offline search

#### Cloud & AI Services
- **AWS SDK for C++**
  - S3Client (cloud storage sync)
  - CognitoIdentityProvider (OAuth authentication)
- **OpenCV** - Video input and gesture recognition
- **PocketSphinx** - Voice input processing

#### Computational Libraries
- **pybind11** - Python interpreter embedding
- **qalculate** - Mathematical computation engine
- **nlohmann/json** - JSON parsing

#### Physics Framework (Internal)
- 200+ custom PhysicsTerm classes
- Self-expanding modular architecture
- Includes references to external source files:
  - `observational_systems_config.h`
  - `source176_auto_full_uqff.cpp`
  - `source177_wolfram_field_unity.cpp`
  - `source178_grok_api.cpp`
  - `uqff_tracing.h`

### Python Path (ABANDONED - READMEs are outdated)
- No Python code exists in repository
- `coanqi_node.py` referenced in README never created
- READMEs describe non-existent Python/AI projects

## Build Instructions (TO BE CREATED)

**Current Issue**: No build system exists yet despite code being ready

```powershell
# TODO: Create CMakeLists.txt with these requirements:
# - Qt5 or Qt6 (Widgets, WebEngine, Network modules)
# - VTK (Charts, Context modules)
# - libcurl, sqlite3, websocket
# - AWS SDK C++ (s3, cognito-idp)
# - OpenCV, PocketSphinx
# - pybind11, qalculate
# - nlohmann-json

# Proposed build commands (after CMakeLists.txt created):
mkdir build
cd build
cmake -G "Visual Studio 17 2022" -A x64 -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --config Release
```

**Prerequisites**: 
- Visual Studio 2022 with C++ workload
- CMake 3.20+
- vcpkg or Conan for dependency management

## Development Conventions (to establish)

### Current File Organization
```
AethericPropulsion/
├── .github/
│   └── copilot-instructions.md
├── .vs/                              # ⚠️ SHOULD NOT BE COMMITTED
├── MAIN_1_CoAnQi.cpp                 # 5.8 MB - Main calculator (200+ PhysicsTerm classes)
├── source2(HEAD PROGRAM).cpp         # 104 KB - Qt scientific search UI
├── LICENSE.md                        # MIT License
├── README.md                         # Outdated (describes non-existent project)
├── README                            # Outdated (describes Python project)
└── SECURITY.md                       # Template (needs actual content)
```

### Missing Critical Files
1. **No .gitignore** - `.vs/` directory is being tracked (2+ GB of IDE cache)
2. **No CMakeLists.txt** - Cannot build despite having complete source code
3. **Missing headers** - Code references but files not present:
   - `observational_systems_config.h`
   - `source176_auto_full_uqff.cpp`
   - `source177_wolfram_field_unity.cpp`
   - `source178_grok_api.cpp`
   - `uqff_tracing.h`
4. **No dependency manifest** - Need `vcpkg.json` or `conanfile.txt`

### Recommended Structure (next steps)
```
AethericPropulsion/
├── .github/
│   └── copilot-instructions.md
├── src/
│   ├── main_calculator.cpp       # Rename from MAIN_1_CoAnQi.cpp
│   ├── search_interface.cpp      # Rename from source2(HEAD PROGRAM).cpp
│   └── modules/                  # Physics modules (source176-178)
├── include/
│   ├── uqff_tracing.h
│   ├── observational_systems_config.h
│   └── physics_terms/            # Header files for PhysicsTerm classes
├── tests/                        # Unit tests
├── docs/                         # Technical documentation
├── .gitignore                    # Exclude .vs/, build/, etc.
├── CMakeLists.txt                # Build configuration
├── vcpkg.json                    # Dependency manifest
└── README.md                     # Accurate project description
```

## Critical Questions to Answer

1. ~~What is the actual project?~~ **ANSWERED**: C++20 UQFF Calculator with dual applications
2. ~~What language?~~ **ANSWERED**: C++20 exclusively
3. **What does it calculate?** Unified Quantum Field Framework with 4 gravity arrangements (Δ​Ug₁-₄)
4. **Where are the missing header files?** Need to locate or create referenced .h/.cpp files
5. **Which dependencies are actually used?** Audit #include statements for minimal viable set

## Architecture Overview

### MAIN_1_CoAnQi.cpp (Conscious Quantum Intelligence)
- **Size**: 5.8 MB (extensive physics framework)
- **Purpose**: UQFF Calculator with self-expanding physics engine
- **Key Concepts**:
  - 4 Universal Gravity Arrangements (Δ​Ug₁, Δ​Ug₂, Δ​Ug₃, Δ​Ug₄)
  - DPM (Di-Pseudo-Monopole) dipole-vortex structure
  - Universal Buoyancy (Δ​Ub_i) as repulsive support
  - 200+ PhysicsTerm plugin classes
  - Self-modifying, self-simulating framework
- **Dependencies**: Standard library + optional Wolfram integration
- **Notable Classes**:
  - `PhysicsTerm` (abstract base for physics equations)
  - `ModuleRegistry` (dynamic module loading)
  - `CalculatorCore` (orchestration engine)
  - `UQFFModule` (mount/dismount interface)

### source2(HEAD PROGRAM).cpp (Scientific Search Interface)
- **Size**: 104 KB
- **Purpose**: Qt-based browser for scientific databases
- **Key Features**:
  - 21 parallel browser windows
  - NASA, MAST, JPL Horizons API integration
  - Scientific calculator dialogs (calculus, Ramanujan numbers)
  - Voice/video input (PocketSphinx, OpenCV)
  - Offline SQLite caching
  - AWS cloud sync
  - VTK data visualization
- **Main Classes**:
  - `MainWindow` (primary GUI)
  - `BrowserWindow` (detachable browsers)
  - `ScientificCalculatorDialog`
  - `RamanujanCalculatorDialog`

## Notes for AI Agents

- **Code DOES exist**: Two large C++ applications are ready for compilation
- **Missing infrastructure**: Build system, headers, dependency manifests needed
- **README mismatch**: Documentation describes Python projects that were never implemented
- **"PImath" and "BaZaAm"**: Part of UQFF theory, not placeholders - see MAIN_1_CoAnQi.cpp comments
- **Gravity theory**: Non-standard physics - 4 gravity types with Universal Buoyancy (see file header)
- **26-level polynomials**: Related to quantum state system (26 resonant states, "quantum alphabet")
- **DPM scaling**: Di-Pseudo-Monopole governs nucleus → cosmic distance interactions
- **Self-modifying architecture**: Code includes runtime code generation and module injection
- **Thread safety**: Uses Windows-specific threading (SimpleMutex wrapper for compatibility)

## Getting Started (for AI agents helping with this project)

1. **Immediate priorities**:
   - Create `.gitignore` to exclude `.vs/` directory
   - Create `CMakeLists.txt` for build configuration
   - Locate or stub out missing header files
   - Create `vcpkg.json` for dependency management
   
2. **Build system setup**:
   - Determine which dependencies are truly needed vs. aspirational
   - Create modular CMake structure for two executables
   - Configure Qt, VTK, and other third-party libraries
   
3. **Code organization**:
   - Consider renaming files to standard naming convention
   - Extract large monolithic files into modules
   - Separate headers from implementations
   
4. **Documentation**:
   - Update READMEs to reflect actual C++ project
   - Document UQFF theory for maintainability
   - Create developer guide for PhysicsTerm plugin system

Update this file as build system is established and architectural patterns solidify.
