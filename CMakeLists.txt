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

#
# benchmarking tool
#
add_subdirectory( bench )
