/*
 * Copyright (c) 2022-present Trail of Bits, Inc.
 */

#include "runner.hpp"
#include <benchmark/benchmark.h>

namespace circ::bench
{
    template< typename input_t >
    void lift_bytes(state_t& state, input_t&& input) {
        return lift(state, instance_t{ std::forward< input_t >(input) });
    }

    // tiny86 set
    BENCHMARK_CAPTURE(lift_bytes, "min:AND_AL_IMMb", "24c3");
    BENCHMARK_CAPTURE(lift_bytes, "min:LEA_GPRv_AGEN_32", "8db40000008b45");
    BENCHMARK_CAPTURE(lift_bytes, "min:LODSB", "24c3");
    BENCHMARK_CAPTURE(lift_bytes, "min:LODSB", "ac");
    BENCHMARK_CAPTURE(lift_bytes, "min:LOOP_RELBRb", "e200");
    BENCHMARK_CAPTURE(lift_bytes, "min:MOVSB", "a4");
    BENCHMARK_CAPTURE(lift_bytes, "min:MOV_GPR8_GPR8_8A", "8ad0");

    // // multiplication by zero
    BENCHMARK_CAPTURE(lift_bytes, "min:MOV_RAX_RAX", "488b00");

    // // and with one child
    BENCHMARK_CAPTURE(lift_bytes, "min:AND_ONE_CHILD_1", "90");
    BENCHMARK_CAPTURE(lift_bytes, "min:AND_ONE_CHILD_2", "488b90");

} // namespace circ::bench
