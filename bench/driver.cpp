/*
 * Copyright (c) 2022-present Trail of Bits, Inc.
 */

#include <benchmark/benchmark.h>

#include <gflags/gflags.h>
#include <glog/logging.h>

#include <spdlog/cfg/env.h>
#include <spdlog/cfg/argv.h>
#include <spdlog/spdlog.h>

#include <circuitous/Support/Log.hpp>
#include <iostream>

#include "options.hpp"

DEFINE_bool(verbose, false, "Enable verbose circuitous.");

DEFINE_string(patterns, "", "Path to the equality saturation patterns.");

DEFINE_string(arch, "x86", "");
DEFINE_string(os, "macos", "");


DEFINE_bool(simplify, false, "Enable basic simplification optimization.");
DEFINE_bool(overflow_flag_mix, false, "Enable overflow flags mix optimization.");
DEFINE_bool(merge_advices, false, "Enable merge advices optimization.");
DEFINE_bool(collapse_ops, false, "Enable colapse ops optimization.");

DEFINE_bool(conjure_alu, false, "Enable conjure ALU optimization (both additive and multiplicative).");
DEFINE_bool(conjure_alu_add, false, "Enable conjure ALU optimization for additive operations..");
DEFINE_bool(conjure_alu_mul, false, "Enable conjure ALU optimization for multiplicative operations.");
DEFINE_bool(switch_as_mux, false, "Enable verilog to emit switches as muxes.");

DEFINE_bool(postprocess_as_llvm, false, "Enable postprocess llvm optimization passes.");

int main(int argc, char** argv) {
    benchmark::Initialize(&argc, argv);

    google::ParseCommandLineFlags(&argc, &argv, true);
    // setup for remill and circuitous logging
    google::InitGoogleLogging(argv[0]);

    // setup for benchmarking logging
    spdlog::cfg::load_env_levels();
    spdlog::cfg::load_argv_levels(argc, argv);

    auto &opts = circ::bench::options;

    opts.arch = circ::bench::arch_from_string(FLAGS_arch);
    opts.os = circ::bench::os_from_string(FLAGS_os);

    if (!FLAGS_patterns.empty()) {
        opts.eqsat = circ::bench::eqsat_options_t{FLAGS_patterns};
    }

    opts.overflow_flag_mix = FLAGS_overflow_flag_mix || FLAGS_simplify;
    opts.merge_advices = FLAGS_merge_advices || FLAGS_simplify;
    opts.collapse_ops = FLAGS_collapse_ops || FLAGS_simplify;

    opts.switch_as_mux = FLAGS_switch_as_mux;

    if (FLAGS_conjure_alu) {
        opts.conjure_alu_add = true;
        opts.conjure_alu_mul = true;
    } else {
        opts.conjure_alu_add = FLAGS_conjure_alu_add;
        opts.conjure_alu_mul = FLAGS_conjure_alu_mul;
    }

    opts.postprocess_as_llvm = FLAGS_postprocess_as_llvm;

    if (FLAGS_verbose) {
        circ::add_sink< circ::severity::kill >(std::cerr);
        circ::add_sink< circ::severity::error >(std::cerr);
        circ::add_sink< circ::severity::warn >(std::cerr);
        circ::add_sink< circ::severity::info >(std::cout);
        circ::add_sink< circ::severity::trace >(std::cout);
        circ::add_sink< circ::severity::dbg >(std::cout);
    }

    spdlog::info("[bench] arch: {}", to_string(opts.arch));
    spdlog::info("[bench] os: {}", to_string(opts.os));
    if (opts.eqsat) {
        spdlog::info("[bench] using eqsat with patterns: {}", std::string(opts.eqsat->patterns));
    }

    if (opts.overflow_flag_mix) {
        spdlog::info("[bench] using overflow flag mix optimization");
    }

    if (opts.merge_advices) {
        spdlog::info("[bench] using merge advices optimization");
    }

    if (opts.collapse_ops) {
        spdlog::info("[bench] using collapse ops optimization");
    }

    if (opts.conjure_alu_add) {
        spdlog::info("[bench] using conjure addivite ALU optimization");
    }

    if (opts.conjure_alu_mul) {
        spdlog::info("[bench] using conjure multiplicative ALU optimization");
    }

    if (opts.switch_as_mux) {
        spdlog::info("[bench] emiting switch as mux");
    }

    if (opts.postprocess_as_llvm) {
        spdlog::info("[bench] posprocess circuit using llvm optimizations");
    }

    benchmark::RunSpecifiedBenchmarks();
    benchmark::Shutdown();
}
