/*
 * Copyright (c) 2022-present Trail of Bits, Inc.
 */

#include "runner.hpp"
#include <benchmark/benchmark.h>

namespace circ::bench
{
    template< typename input_t >
    void lift_source(state_t& state, input_t&& input) {
        return lift(state, ciff_file_t{ std::forward< input_t >(input) });
    }

    BENCHMARK_CAPTURE(lift_source, "min:small", "./data/small.ciff");

    BENCHMARK_CAPTURE(lift_source, "min:adds", "./data/adds.ciff");
    BENCHMARK_CAPTURE(lift_source, "min:mul-16", "./data/mul-16.ciff");
    BENCHMARK_CAPTURE(lift_source, "big:muls", "./data/muls.ciff");
    BENCHMARK_CAPTURE(lift_source, "min:idivs", "./data/idivs.ciff");
    BENCHMARK_CAPTURE(lift_source, "big:divs", "./data/divs.ciff");
    BENCHMARK_CAPTURE(lift_source, "min:subs", "./data/subs.ciff");
    BENCHMARK_CAPTURE(lift_source, "min:xors", "./data/xors.ciff");

    BENCHMARK_CAPTURE(lift_source, "big:3d_image_toolkit", "./data/3d_image_toolkit.ciff");
    BENCHMARK_CAPTURE(lift_source, "big:router_simulator", "./data/router_simulator.ciff");
    BENCHMARK_CAPTURE(lift_source, "big:lan_simulator",    "./data/lan_simulator.ciff");
    BENCHMARK_CAPTURE(lift_source, "big:tiny86", "./data/tiny86.ciff");

} // namespace circ::bench
