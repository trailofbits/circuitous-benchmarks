/*
 * Copyright (c) 2022-present Trail of Bits, Inc.
 */

#include "runner.hpp"
#include <benchmark/benchmark.h>

namespace circ::bench
{
    template< typename input_t >
    void run(state_t& state, input_t&& input) {
        return run_on_verilog(state, verilog_file_t{ std::forward< input_t >(input) });
    }

    BENCHMARK_CAPTURE(run, "ver:mwa", "./data/verilog/mwa.v");
    BENCHMARK_CAPTURE(run, "ver:orig", "./data/verilog/original.v");
    BENCHMARK_CAPTURE(run, "ver:purged", "./data/verilog/purged.v");

} // namespace circ::bench
