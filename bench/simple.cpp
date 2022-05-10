/*
 * Copyright (c) 2022-present Trail of Bits, Inc.
 */

#include <benchmark/benchmark.h>

namespace circ::bench {
    using state_t = benchmark::State;

    static void simple_bench(state_t& state) {
        for (auto _ : state)
            std::string empty_string;
    }

    BENCHMARK(simple_bench);
}
