/*
 * Copyright (c) 2022-present Trail of Bits, Inc.
 */

#include "lifter.hpp"
#include "options.hpp"
#include "stats.hpp"
#include "common.hpp"

#include <benchmark/benchmark.h>

namespace circ::bench
{
    constexpr auto options = options_t{ os_t::macos, arch_t::x86 };

    template< typename... args_t >
    void lift_source(state_t& state, args_t&&... args) {
        std::vector counters = {
            operation_counter_t{ node_kind_t::kMul },
            operation_counter_t{ node_kind_t::kAdd },
            operation_counter_t{ node_kind_t::kUDiv },
            operation_counter_t{ node_kind_t::kSDiv },
            operation_counter_t{ node_kind_t::kURem },
            operation_counter_t{ node_kind_t::kSRem },
            operation_counter_t{ node_kind_t::kOperation }
        };

        verilog_cell_counter_t verilog_counter;

        auto [source] = std::make_tuple(std::move(args)...);

        for (auto _ : state) {
            auto ci = make_circuit(ciff_file_t{source}, options);
            for (auto &counter : counters) {
                counter.count(ci);
                verilog_counter.count(ci);
            }
        }

        for (const auto &counter : counters) {
            state.counters[counter.name()] = counter_t(counter.get(), average);
        }

        state.counters["verilog cells"] = counter_t(verilog_counter.get(), average);
        state.counters["AND"] = counter_t(verilog_counter.get(verilog_kind::and_cell), average);
        state.counters["NOT"] = counter_t(verilog_counter.get(verilog_kind::not_cell), average);
        state.counters["XOR"] = counter_t(verilog_counter.get(verilog_kind::xor_cell), average);
    }

    // BENCHMARK_CAPTURE(lift_source, "3d_image_toolkit", "./data/3d_image_toolkit.ciff");
    BENCHMARK_CAPTURE(lift_source, "router_simulator", "./data/router_simulator.ciff");
    // BENCHMARK_CAPTURE(lift_source, "lan_simulator",    "./data/lan_simulator.ciff");
    // BENCHMARK_CAPTURE(lift_source, "tiny86", "./data/tiny86.ciff");

} // namespace circ::bench
