#
# Copyright (c) 2022-present, Trail of Bits, Inc.
# All rights reserved.
#
# This source code is licensed in accordance with the terms specified in
# the LICENSE file found in the root directory of this source tree.
#
cmake_minimum_required( VERSION 3.23 )

# check that imported target misses its location
if ( POLICY CMP0111 )
  cmake_policy( SET CMP0111 NEW )
endif ()

project( circuitous-benchmarks
  LANGUAGES C CXX ASM
  VERSION 0.1.1
  DESCRIPTION "Binary to circuit lifter tool benchmarks"
  HOMEPAGE_URL "https://github.com/trailofbits/circuitous-benchmark.git"
)

find_package( zstd CONFIG REQUIRED )
find_package( gap CONFIG REQUIRED )

set_property( GLOBAL PROPERTY USE_FOLDERS ON )

# prevent in source builds check
include( prevent_in_source_builds )

# general project options
include( project_settings )

add_library( circ_bench_project_options INTERFACE )
target_compile_features( circ_bench_project_options INTERFACE cxx_std_20 )
target_include_directories( circ_bench_project_options INTERFACE include )

# link benchmarks to use the warnings specified in compiler_warnings.cmake
add_library( circ_bench_project_warnings INTERFACE )

# standard compiler warnings
include( compiler_warnings )
set_project_warnings( circ_bench_project_warnings )

# sanitizer options if supported by compiler
include( sanitizers )
enable_sanitizers( circ_bench_project_options )

# allow for static analysis options
include( static_analyzers )

#
# dependencies
#
find_package( benchmark CONFIG REQUIRED )
find_package( circuitous CONFIG REQUIRED )
find_package( fmt CONFIG REQUIRED )
find_package( spdlog CONFIG REQUIRED )
find_package( Boost COMPONENTS system filesystem REQUIRED )

find_package(LLVM 16 REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")

include(AddLLVM)

include_directories(${LLVM_INCLUDE_DIRS})
link_directories(${LLVM_BUILD_LIBRARY_DIR})
add_definitions(${LLVM_DEFINITIONS})

#
# benchmarking tool
#
add_subdirectory( bench )

configure_file( scripts/bench-min.sh.in bench-min.sh @ONLY )
configure_file( scripts/yosys.pl scripts/yosys.pl @ONLY )
