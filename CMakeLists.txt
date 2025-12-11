cmake_minimum_required(VERSION 3.20)
project(AethericPropulsion VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Optional features - can be toggled via -DUSE_QT=OFF, etc.
option(USE_QT "Build Qt-based scientific search interface" OFF)
option(USE_VTK "Enable VTK visualization" OFF)
option(USE_OPENCV "Enable OpenCV video input" OFF)
option(USE_AWS "Enable AWS cloud sync" OFF)
option(USE_WOLFRAM "Enable Wolfram integration" OFF)
option(USE_OPENMP "Enable OpenMP parallel processing" ON)

# Create stub headers for missing dependencies
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/observational_systems_config.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/include/observational_systems_config.h"
    COPYONLY
)
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/uqff_tracing.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/include/uqff_tracing.h"
    COPYONLY
)

# Include directories
include_directories(${CMAKE_CURRENT_BINARY_DIR}/include)

# ============================================================================
# Executable 1: UQFF Calculator (MAIN_1_CoAnQi.cpp)
# ============================================================================
add_executable(uqff_calculator "MAIN_1_CoAnQi.cpp")

target_compile_features(uqff_calculator PRIVATE cxx_std_20)

# OpenMP support (optional)
if(USE_OPENMP)
    find_package(OpenMP)
    if(OpenMP_CXX_FOUND)
        target_link_libraries(uqff_calculator PRIVATE OpenMP::OpenMP_CXX)
        target_compile_definitions(uqff_calculator PRIVATE USE_OPENMP)
        message(STATUS "OpenMP enabled for parallel processing")
    else()
        message(WARNING "OpenMP not found - compiling without parallel support")
    endif()
endif()

# Force include source4_forward.h for early SOURCE4 namespace declarations
target_compile_options(uqff_calculator PRIVATE /FI"${CMAKE_CURRENT_SOURCE_DIR}/source4_forward.h")

if(WIN32)
    target_compile_definitions(uqff_calculator PRIVATE 
        _WIN32_WINNT=0x0A00
        NOMINMAX
        WIN32_LEAN_AND_MEAN
    )
endif()

# Optional Wolfram integration
if(USE_WOLFRAM)
    target_compile_definitions(uqff_calculator PRIVATE USE_EMBEDDED_WOLFRAM)
    # TODO: find_package(Wolfram) and link libraries
endif()

# Windows-specific libraries
if(WIN32)
    target_link_libraries(uqff_calculator PRIVATE ws2_32)
endif()

# ============================================================================
# Executable 2: Scientific Search Interface (source2)
# ============================================================================
# Only build if Qt is available and enabled
if(USE_QT)
    find_package(Qt6 COMPONENTS Core Widgets WebEngineWidgets Network QUIET)
    if(NOT Qt6_FOUND)
        find_package(Qt5 5.15 COMPONENTS Core Widgets WebEngineWidgets Network QUIET)
    endif()

    if(Qt6_FOUND OR Qt5_FOUND)
        add_executable(scientific_search "source2(HEAD PROGRAM).cpp")
        target_compile_features(scientific_search PRIVATE cxx_std_20)

        if(WIN32)
            target_compile_definitions(scientific_search PRIVATE 
                _WIN32_WINNT=0x0A00
                NOMINMAX
            )
        endif()

        # Link Qt libraries
        if(Qt6_FOUND)
            target_link_libraries(scientific_search PRIVATE 
                Qt6::Core Qt6::Widgets Qt6::WebEngineWidgets Qt6::Network)
        else()
            target_link_libraries(scientific_search PRIVATE 
                Qt5::Core Qt5::Widgets Qt5::WebEngineWidgets Qt5::Network)
        endif()

        # Optional VTK
        if(USE_VTK)
            find_package(VTK QUIET COMPONENTS ChartsCore ViewsContext2D)
            if(VTK_FOUND)
                target_link_libraries(scientific_search PRIVATE ${VTK_LIBRARIES})
                target_compile_definitions(scientific_search PRIVATE USE_VTK)
            endif()
        endif()

        # Optional OpenCV
        if(USE_OPENCV)
            find_package(OpenCV QUIET)
            if(OpenCV_FOUND)
                target_link_libraries(scientific_search PRIVATE opencv_core opencv_videoio)
                target_compile_definitions(scientific_search PRIVATE USE_OPENCV)
            endif()
        endif()

        # Optional AWS SDK
        if(USE_AWS)
            find_package(AWSSDK QUIET COMPONENTS s3 cognito-idp)
            if(AWSSDK_FOUND)
                target_link_libraries(scientific_search PRIVATE ${AWSSDK_LINK_LIBRARIES})
                target_compile_definitions(scientific_search PRIVATE USE_AWS)
            endif()
        endif()

        # Optional dependencies - gracefully skip if not found
        find_package(CURL QUIET)
        if(CURL_FOUND)
            target_link_libraries(scientific_search PRIVATE CURL::libcurl)
            target_compile_definitions(scientific_search PRIVATE USE_CURL)
        endif()

        find_package(SQLite3 QUIET)
        if(SQLite3_FOUND)
            target_link_libraries(scientific_search PRIVATE SQLite::SQLite3)
            target_compile_definitions(scientific_search PRIVATE USE_SQLITE3)
        endif()

        find_package(nlohmann_json QUIET)
        if(nlohmann_json_FOUND)
            target_link_libraries(scientific_search PRIVATE nlohmann_json::nlohmann_json)
            target_compile_definitions(scientific_search PRIVATE USE_JSON)
        endif()

        find_package(pybind11 QUIET)
        if(pybind11_FOUND)
            target_link_libraries(scientific_search PRIVATE pybind11::embed)
            target_compile_definitions(scientific_search PRIVATE USE_PYBIND11)
        endif()

        # Installation
        install(TARGETS scientific_search
            RUNTIME DESTINATION bin
        )

        message(STATUS "Scientific search interface will be built")
    else()
        message(WARNING "Qt not found - skipping scientific_search executable")
    endif()
else()
    message(STATUS "Qt support disabled - skipping scientific_search executable")
endif()

# Installation
install(TARGETS uqff_calculator
    RUNTIME DESTINATION bin
)

# Print configuration summary
message(STATUS "")
message(STATUS "AethericPropulsion Configuration Summary:")
message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  Qt Support: ${USE_QT}")
message(STATUS "  VTK Support: ${USE_VTK}")
message(STATUS "  OpenCV Support: ${USE_OPENCV}")
message(STATUS "  AWS Support: ${USE_AWS}")
message(STATUS "  Wolfram Support: ${USE_WOLFRAM}")
message(STATUS "")
