/*
 * Copyright (c) 2022-present Trail of Bits, Inc.
 */

#include "lifter.hpp"
#include "options.hpp"

#include <benchmark/benchmark.h>

namespace circ::bench
{
    using state_t = benchmark::State;

    using counter_t = benchmark::Counter;

    constexpr auto average = counter_t::kAvgThreads;

    constexpr auto options = options_t{ os_t::macos, arch_t::x86 };

    template< typename... args_t >
    void lift_bench(state_t& state, args_t&&... args) {
        int nodes    = 0;
        auto [bytes] = std::make_tuple(std::move(args)...);
        for (auto _ : state) {
            auto ci = make_circuit({ bytes }, options);
            nodes += 1;
        }
        state.counters["nodes"] = counter_t(nodes, average);
    }

    BENCHMARK_CAPTURE(lift_bench, "AND_AL_IMMb", "24c3");
    BENCHMARK_CAPTURE(lift_bench, "LEA_GPRv_AGEN_32", "8db40000008b45");
    BENCHMARK_CAPTURE(lift_bench, "LODSB", "24c3");
    BENCHMARK_CAPTURE(lift_bench, "LODSB", "ac");
    BENCHMARK_CAPTURE(lift_bench, "LOOP_RELBRb", "e200");
    BENCHMARK_CAPTURE(lift_bench, "MOVSB", "a4");
    BENCHMARK_CAPTURE(lift_bench, "MOV_GPR8_GPR8_8A", "8ad0");

} // namespace circ::bench
