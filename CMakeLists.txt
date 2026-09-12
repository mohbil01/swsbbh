cmake_minimum_required(VERSION 3.25)
project(swsbbh VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(SWSBBH_ENABLE_MPI "Build with MPI (requires an MPI installation)" OFF)

# Keep AMReX's build consistent with this project's one-dimensional model.
set(AMReX_SPACEDIM 1 CACHE STRING "Spatial dimension" FORCE)
set(AMReX_PRECISION DOUBLE CACHE STRING "Floating-point precision" FORCE)
set(AMReX_MPI ${SWSBBH_ENABLE_MPI} CACHE BOOL "MPI support" FORCE)
set(AMReX_OMP OFF CACHE BOOL "OpenMP support" FORCE)
set(AMReX_GPU_BACKEND NONE CACHE STRING "GPU backend" FORCE)
set(AMReX_FORTRAN OFF CACHE BOOL "Fortran support" FORCE)
set(AMReX_AMRLEVEL ON CACHE BOOL "AmrLevel support for later milestones" FORCE)
set(AMReX_PARTICLES OFF CACHE BOOL "Particle support" FORCE)
set(AMReX_LINEAR_SOLVERS OFF CACHE BOOL "Elliptic solvers" FORCE)
set(AMReX_EB OFF CACHE BOOL "Embedded boundaries" FORCE)
set(AMReX_FFT OFF CACHE BOOL "FFT support" FORCE)
set(AMReX_INSTALL OFF CACHE BOOL "Install AMReX" FORCE)

include(FetchContent)
# AMReX release 26.09, released 2026-09-01. Do not follow a moving branch.
set(SWSBBH_AMREX_REVISION a52ca73324ac2c7b65ec04f131e6df99eec9c576)
FetchContent_Declare(amrex
    URL https://github.com/AMReX-Codes/amrex/archive/${SWSBBH_AMREX_REVISION}.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(amrex)

add_executable(bh_scatter src/main.cpp)
target_include_directories(bh_scatter PRIVATE include)
target_link_libraries(bh_scatter PRIVATE AMReX::amrex)
target_compile_definitions(bh_scatter PRIVATE
    SWSBBH_AMREX_REVISION="${SWSBBH_AMREX_REVISION}"
)
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(bh_scatter PRIVATE -Wall -Wextra -Wpedantic)
endif()

include(CTest)
if(BUILD_TESTING)
    # Integration check: initialise AMReX and allocate the project grid.
    # Numerical correctness tests are exercises listed in tests/README.md.
    add_test(NAME bootstrap
        COMMAND bh_scatter ${PROJECT_SOURCE_DIR}/inputs/flat_wave
    )
endif()

