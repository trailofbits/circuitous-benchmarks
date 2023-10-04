/*
 * Copyright (c) 2022-present Trail of Bits, Inc.
 */

#include <benchmark/benchmark.h>

#include <gflags/gflags.h>
#include <glog/logging.h>

#include <spdlog/cfg/env.h>
#include <spdlog/cfg/argv.h>
#include <spdlog/spdlog.h>
#include <fmt/chrono.h>

#include <circuitous/Support/Log.hpp>

#include <iostream>
#include <chrono>

#include "options.hpp"

#include <boost/filesystem.hpp>

DEFINE_bool(verbose, false, "Enable verbose circuitous.");

DEFINE_string(patterns, "", "Path to the equality saturation patterns.");

DEFINE_string(arch, "x86", "");
DEFINE_string(os, "macos", "");
DEFINE_string(smithy, "muxes", "");

DEFINE_string(out_dir, "", "Output directory.");
DEFINE_bool(keep_files, false, "Keep benchmark produces files.");

DEFINE_bool(simplify, false, "Enable basic simplification optimization.");
DEFINE_bool(collapse_ops, false, "Enable colapse ops optimization.");

DEFINE_bool(conjure_alu, false, "Enable all conjure ALU optimizations.");
DEFINE_bool(conjure_alu_add, false, "Enable conjure ALU optimization for additive operations..");
DEFINE_bool(conjure_alu_mul, false, "Enable conjure ALU optimization for multiplication operations.");
DEFINE_bool(conjure_alu_div, false, "Enable conjure ALU optimization for divisions operations.");
DEFINE_bool(switch_as_mux, false, "Enable verilog to emit switches as muxes.");

DEFINE_bool(postprocess_as_llvm, false, "Enable postprocess llvm optimization passes.");

std::string timestamp() {
    const auto now = std::chrono::system_clock::now();
    return fmt::format("{:%d-%m-%Y-%H:%M:%OS}", now);
}

static std::filesystem::path benchmarks_directory() {
    if (FLAGS_out_dir.empty())
        return std::string("circ-benchmarks-") + timestamp();
    return FLAGS_out_dir;
}

static void setup() {
    auto &opts = circ::bench::options;
    if (std::filesystem::exists(opts.directory)) {
        throw std::runtime_error(fmt::format("output directory {} already exists", opts.directory.string()));
    }
    std::filesystem::create_directory(opts.directory);
}

static void teardown() {
    auto &opts = circ::bench::options;
    if (!opts.keep_files) {
        std::filesystem::remove_all(opts.directory);
    }
}


int main(int argc, char** argv) {
    benchmark::Initialize(&argc, argv);

    google::ParseCommandLineFlags(&argc, &argv, true);
    // setup for remill and circuitous logging
    google::InitGoogleLogging(argv[0]);

    // setup for benchmarking logging
    spdlog::cfg::load_env_levels();
    spdlog::cfg::load_argv_levels(argc, argv);

    auto &opts = circ::bench::options;

    opts.keep_files = FLAGS_keep_files;

    opts.directory = benchmarks_directory();

    opts.arch = circ::bench::arch_from_string(FLAGS_arch);
    opts.os = circ::bench::os_from_string(FLAGS_os);
    opts.smithy_type = circ::bench::smithy_type_from_string(FLAGS_smithy);

    if (!FLAGS_patterns.empty()) {
        opts.eqsat = circ::bench::eqsat_options_t{FLAGS_patterns};
    }

    opts.collapse_ops = FLAGS_collapse_ops || FLAGS_simplify;

    opts.switch_as_mux = FLAGS_switch_as_mux;

    if (FLAGS_conjure_alu) {
        opts.conjure_alu_add = true;
        opts.conjure_alu_mul = true;
        opts.conjure_alu_div = true;
    } else {
        opts.conjure_alu_add = FLAGS_conjure_alu_add;
        opts.conjure_alu_mul = FLAGS_conjure_alu_mul;
        opts.conjure_alu_div = FLAGS_conjure_alu_div;
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

    spdlog::info("[bench] directory: {}", opts.directory.string());
    spdlog::info("[bench] arch: {}", to_string(opts.arch));
    spdlog::info("[bench] os: {}", to_string(opts.os));
    spdlog::info("[bench] smithy: {}", to_string(opts.smithy_type));

    if (opts.eqsat) {
        spdlog::info("[bench] using eqsat with patterns: {}", std::string(opts.eqsat->patterns));
    }

    if (opts.collapse_ops) {
        spdlog::info("[bench] using collapse ops optimization");
    }

    if (opts.conjure_alu_add) {
        spdlog::info("[bench] using conjure addivite ALU optimization");
    }

    if (opts.conjure_alu_mul) {
        spdlog::info("[bench] using conjure multiplication ALU optimization");
    }

    if (opts.conjure_alu_div) {
        spdlog::info("[bench] using conjure division ALU optimization");
    }


    if (opts.switch_as_mux) {
        spdlog::info("[bench] emiting switch as mux");
    }

    if (opts.postprocess_as_llvm) {
        spdlog::info("[bench] posprocess circuit using llvm optimizations");
    }

    setup();

    benchmark::RunSpecifiedBenchmarks();
    benchmark::Shutdown();

    teardown();
}
