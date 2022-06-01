/*
 * Copyright (c) 2022-present Trail of Bits, Inc.
 */

#include "lifter.hpp"
#include "options.hpp"
#include "stats.hpp"

#include <benchmark/benchmark.h>

namespace circ::bench
{
    using state_t = benchmark::State;

    using counter_t = benchmark::Counter;

    constexpr auto average = counter_t::kAvgThreads;

    constexpr auto options = options_t{ os_t::macos, arch_t::x86 };

    template< typename... args_t >
    void lift_bench(state_t& state, args_t&&... args) {
        std::vector counters = {
            operation_counter_t{ node_kind_t::kMul },
            operation_counter_t{ node_kind_t::kAdd },
            operation_counter_t{ node_kind_t::kOperation }
        };

        auto [bytes] = std::make_tuple(std::move(args)...);
        for (auto _ : state) {
            auto ci = make_circuit({ bytes }, options);
            for (auto &counter : counters) {
                counter.count(ci);
            }
        }

        for (const auto &counter : counters) {
            state.counters[counter.name()] = counter_t(counter.get(), average);
        }
    }

    BENCHMARK_CAPTURE(lift_bench, "AND_AL_IMMb", "24c3");
    BENCHMARK_CAPTURE(lift_bench, "LEA_GPRv_AGEN_32", "8db40000008b45");
    BENCHMARK_CAPTURE(lift_bench, "LODSB", "24c3");
    BENCHMARK_CAPTURE(lift_bench, "LODSB", "ac");
    BENCHMARK_CAPTURE(lift_bench, "LOOP_RELBRb", "e200");
    BENCHMARK_CAPTURE(lift_bench, "MOVSB", "a4");
    BENCHMARK_CAPTURE(lift_bench, "MOV_GPR8_GPR8_8A", "8ad0");

} // namespace circ::bench
