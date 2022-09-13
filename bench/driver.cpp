/*
 * Copyright (c) 2022-present Trail of Bits, Inc.
 */

#include <benchmark/benchmark.h>

#include <gflags/gflags.h>
#include <glog/logging.h>

#include "options.hpp"

DEFINE_string(patterns, "", "Path to the equality saturation patterns.");

DEFINE_string(arch, "x86", "");
DEFINE_string(os, "macos", "");

DEFINE_string(mwa, "", "Merge Advice Nodes Preprocessing.");

int main(int argc, char** argv) {
    benchmark::Initialize(&argc, argv);

    google::ParseCommandLineFlags(&argc, &argv, true);
    google::InitGoogleLogging(argv[0]);

    auto &opts = circ::bench::options;

    opts.arch = circ::bench::arch_from_string(FLAGS_arch);
    opts.os = circ::bench::os_from_string(FLAGS_os);

    if (!FLAGS_patterns.empty()) {
        opts.eqsat = circ::bench::eqsat_options_t{FLAGS_patterns};
    }

    if (!FLAGS_mwa.empty()) {
        opts.mwa = circ::bench::mwa_from_string(FLAGS_mwa);
    }

    benchmark::RunSpecifiedBenchmarks();
    benchmark::Shutdown();
}
