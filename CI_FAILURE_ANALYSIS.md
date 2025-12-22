# CI Failure Analysis - AethericPropulsion

**Date**: December 22, 2025  
**Investigation**: Remote CI Build Failures

## Executive Summary

The GitHub Actions CI workflows are failing due to **hardcoded Windows-specific paths** in `CMakeLists.txt` that don't exist on GitHub Actions runners. The build system assumes a local development environment with specific Windows installations of vcpkg, Wolfram Engine, and Qt.

---

## Failure Details

### Workflow Status

| Run | Branch | Status | Issue |
|-----|--------|--------|-------|
| #13 (PR #41) | copilot/investigate-remote-failure | `action_required` | Waiting for manual approval |
| #12 (PR #40) | copilot/resolve-merge-conflicts | `action_required` | Waiting for manual approval |
| #10 | main (de8988d) | `failure` | CMake configuration failed |
| #9 | main (d886980) | `failure` | CMake configuration failed |
| #8 | main (7337e06) | `failure` | CMake configuration failed |

### Primary Failure Point

Both Windows and Linux builds fail at the **"Set up job"** or **CMake configuration** step due to:
1. Hardcoded Windows paths that don't exist on runners
2. Platform-specific compiler flags used unconditionally
3. Missing dependencies not handled gracefully

---

## Root Causes

### 1. Hardcoded vcpkg Toolchain Path (Line 4)

```cmake
set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
```

**Problem**: 
- ❌ Assumes vcpkg is installed at `C:/vcpkg` (Windows-only)
- ❌ Fails on Linux (no `C:` drive)
- ❌ Fails on GitHub Actions runners (different vcpkg location)

**Impact**: CMake configuration fails immediately

---

### 2. MSVC-Specific Compiler Flag (Line 52)

```cmake
target_compile_options(uqff_calculator PRIVATE /FI"${CMAKE_CURRENT_SOURCE_DIR}/source4_forward.h")
```

**Problem**:
- ❌ `/FI` is MSVC-only syntax
- ❌ GCC/Clang use `-include` instead
- ❌ Causes compilation failure on Linux

**Impact**: Linux builds fail during compile phase

---

### 3. Hardcoded Wolfram Engine Path (Lines 67-78)

```cmake
if(USE_WOLFRAM)
    set(WOLFRAM_ROOT "C:/Program Files/Wolfram Research/Wolfram Engine/14.3")
    set(WSTP_DEVKIT "${WOLFRAM_ROOT}/SystemFiles/Links/WSTP/DeveloperKit/Windows-x86-64")
    # ...
endif()
```

**Problem**:
- ❌ Windows-specific path with spaces
- ❌ Windows-specific architecture (`Windows-x86-64`)
- ❌ No Linux/macOS equivalents

**Impact**: Wolfram integration fails on non-Windows platforms

---

### 4. Hardcoded Qt Path (Line 100)

```cmake
if(USE_QT)
    list(APPEND CMAKE_PREFIX_PATH "C:/Qt/6.10.0/msvc2022_64")
    # ...
endif()
```

**Problem**:
- ❌ Assumes Qt installed at `C:/Qt`
- ❌ MSVC 2022-specific path
- ❌ Hardcoded Qt version (6.10.0)

**Impact**: Qt-based builds fail on all platforms

---

## Recommended Solutions

### Solution 1: Dynamic vcpkg Toolchain (Priority: HIGH)

Replace hardcoded path with environment variable:

```cmake
# Use vcpkg toolchain if available (set via environment or command line)
if(DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
elseif(DEFINED VCPKG_ROOT)
    set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
else()
    message(STATUS "vcpkg not configured - using system libraries")
endif()
```

**Usage**:
```bash
# Local development
cmake -DVCPKG_ROOT=C:/vcpkg -B build

# GitHub Actions
export VCPKG_ROOT=/path/to/vcpkg
cmake -B build
```

---

### Solution 2: Cross-Platform Force Include (Priority: HIGH)

Use generator expressions for platform-specific flags:

```cmake
# Force include source4_forward.h (cross-platform)
if(MSVC)
    target_compile_options(uqff_calculator PRIVATE /FI"${CMAKE_CURRENT_SOURCE_DIR}/source4_forward.h")
else()
    target_compile_options(uqff_calculator PRIVATE -include "${CMAKE_CURRENT_SOURCE_DIR}/source4_forward.h")
endif()
```

Or use a more portable approach:

```cmake
# Add source4_forward.h to compilation units via target_sources
target_compile_definitions(uqff_calculator PRIVATE FORCE_INCLUDE_SOURCE4)
```

---

### Solution 3: Find Wolfram Dynamically (Priority: MEDIUM)

Use `find_package` or environment variables:

```cmake
if(USE_WOLFRAM)
    # Try to find Wolfram via environment variable first
    if(DEFINED ENV{WOLFRAM_ROOT})
        set(WOLFRAM_ROOT "$ENV{WOLFRAM_ROOT}")
    elseif(WIN32)
        # Windows default location
        file(GLOB WOLFRAM_VERSIONS "C:/Program Files/Wolfram Research/Wolfram Engine/*")
        list(SORT WOLFRAM_VERSIONS ORDER DESCENDING)
        list(GET WOLFRAM_VERSIONS 0 WOLFRAM_ROOT)
    elseif(APPLE)
        # macOS default location
        file(GLOB WOLFRAM_VERSIONS "/Applications/Wolfram Engine.app/Contents/Resources/Wolfram Player/*")
        list(GET WOLFRAM_VERSIONS 0 WOLFRAM_ROOT)
    else()
        # Linux - try common locations
        set(WOLFRAM_ROOT "/usr/local/Wolfram/WolframEngine/14.3")
    endif()
    
    if(EXISTS "${WOLFRAM_ROOT}")
        message(STATUS "Found Wolfram Engine: ${WOLFRAM_ROOT}")
        # Configure WSTP paths...
    else()
        message(WARNING "Wolfram Engine not found - building without Wolfram support")
        set(USE_WOLFRAM OFF)
    endif()
endif()
```

---

### Solution 4: Find Qt via CMake (Priority: MEDIUM)

Let CMake find Qt automatically:

```cmake
if(USE_QT)
    # Remove hardcoded path - let CMake find Qt
    # Users can set Qt5_DIR or Qt6_DIR if needed
    find_package(Qt6 COMPONENTS Core Widgets WebEngineWidgets Network QUIET)
    
    if(Qt6_FOUND)
        message(STATUS "Found Qt6: ${Qt6_DIR}")
    else()
        find_package(Qt5 COMPONENTS Core Widgets WebEngineWidgets Network QUIET)
        if(Qt5_FOUND)
            message(STATUS "Found Qt5: ${Qt5_DIR}")
        else()
            message(WARNING "Qt not found - building without Qt support")
            set(USE_QT OFF)
        endif()
    endif()
endif()
```

---

### Solution 5: Update GitHub Actions Workflow (Priority: HIGH)

Add dependency installation steps:

```yaml
jobs:
  build-windows:
    runs-on: windows-latest
    steps:
      - name: Checkout
        uses: actions/checkout@v4

      - name: Setup vcpkg
        uses: lukka/run-vcpkg@v11
        with:
          vcpkgGitCommitId: 'a42af01b72c28a8e1d7b48107b33e4f286a55ef6'

      - name: Setup CMake
        uses: lukka/get-cmake@v4

      - name: Configure (MSVC)
        run: |
          cmake -S . -B build `
            -G "Visual Studio 17 2022" -A x64 `
            -DCMAKE_TOOLCHAIN_FILE="${env:VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" `
            -DCMAKE_BUILD_TYPE=Release
```

---

## Implementation Priority

1. **HIGH** - Fix vcpkg toolchain path (Solution 1)
2. **HIGH** - Fix force include flags (Solution 2)  
3. **HIGH** - Update GitHub Actions workflow (Solution 5)
4. **MEDIUM** - Make Wolfram path dynamic (Solution 3)
5. **MEDIUM** - Make Qt path dynamic (Solution 4)
6. **LOW** - Add fallback for missing dependencies

---

## Testing Strategy

### Local Testing
```bash
# Test without vcpkg
cmake -B build-minimal -DCMAKE_BUILD_TYPE=Release
cmake --build build-minimal

# Test with vcpkg
cmake -B build-full -DCMAKE_TOOLCHAIN_FILE=path/to/vcpkg.cmake
cmake --build build-full
```

### CI Testing
```bash
# Push to test branch
git checkout -b fix/ci-compatibility
# Apply fixes
git commit -am "fix: Make CMake cross-platform compatible"
git push -u origin fix/ci-compatibility
# Monitor workflow runs
```

---

## Additional Notes

### "action_required" Status
Recent PR workflow runs (#40, #41) show `action_required` status, which means:
- The workflow requires **manual approval** due to environment protection rules
- This is NOT a build failure - it's waiting for user authorization
- Once approved, the workflow will proceed (but will likely hit the same build issues)

### Minimal Build Configuration
For CI, recommend building without optional dependencies:
```bash
cmake -B build -DUSE_QT=OFF -DUSE_WOLFRAM=OFF -DUSE_AWS=OFF -DUSE_OPENCV=OFF
```

This avoids dependency issues and focuses on core UQFF calculator compilation.

---

## References

- **CMakeLists.txt**: Lines 4, 52, 67-78, 100
- **Workflow file**: `.github/workflows/windows-cmake.yml`
- **Failed runs**: 
  - Run #10 (main): https://github.com/Daniel8Murphy0007/AethericPropulsion/actions/runs/20423662403
  - Run #9 (main): https://github.com/Daniel8Murphy0007/AethericPropulsion/actions/runs/20142457359
  - Run #8 (main): https://github.com/Daniel8Murphy0007/AethericPropulsion/actions/runs/20141994887

---

## Conclusion

The CI failures are entirely due to **hardcoded local development paths** in CMakeLists.txt. The code itself is likely sound, but the build system needs to be made **cross-platform** and **CI-friendly**.

**Recommended Immediate Action**: Apply Solutions 1, 2, and 5 (vcpkg path, force include fix, workflow update) to get CI builds passing.
