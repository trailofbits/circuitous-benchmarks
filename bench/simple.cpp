/*
 * Copyright (c) 2022-present Trail of Bits, Inc.
 */

#include "common.hpp"
#include "lifter.hpp"
#include "options.hpp"
#include "stats.hpp"

#include <benchmark/benchmark.h>

namespace circ::bench
{
    constexpr auto options = options_t{ os_t::macos, arch_t::x86 };

    template< typename... args_t >
    void lift(state_t& state, args_t&&... args) {
        std::vector counters = {
            operation_counter_t{ node_kind_t::kMul },
            operation_counter_t{ node_kind_t::kAdd },
            operation_counter_t{ node_kind_t::kOperation }
        };

        auto [bytes] = std::make_tuple(std::move(args)...);
        for (auto _ : state) {
            auto ci = make_circuit(instance_t{ bytes }, options);
            for (auto &counter : counters) {
                counter.count(ci);
            }
        }

        for (const auto &counter : counters) {
            state.counters[counter.name()] = counter_t(counter.get(), average);
        }
    }

    // tiny86 set
    BENCHMARK_CAPTURE(lift, "AND_AL_IMMb", "24c3");
    BENCHMARK_CAPTURE(lift, "LEA_GPRv_AGEN_32", "8db40000008b45");
    BENCHMARK_CAPTURE(lift, "LODSB", "24c3");
    BENCHMARK_CAPTURE(lift, "LODSB", "ac");
    BENCHMARK_CAPTURE(lift, "LOOP_RELBRb", "e200");
    BENCHMARK_CAPTURE(lift, "MOVSB", "a4");
    BENCHMARK_CAPTURE(lift, "MOV_GPR8_GPR8_8A", "8ad0");

    // multiplication by zero
    BENCHMARK_CAPTURE(lift, "MOV_RAX_RAX", "488b00");

    // and with one child
    BENCHMARK_CAPTURE(lift, "AND_ONE_CHILD_1", "90");
    BENCHMARK_CAPTURE(lift, "AND_ONE_CHILD_2", "488b90");

} // namespace circ::bench
