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
    template< typename input_t >
    void lift(state_t& state, input_t&& input) {
        std::vector counters = {
            operation_counter_t{ node_kind_t::kMul },
            operation_counter_t{ node_kind_t::kAdd },
            // operation_counter_t{ node_kind_t::kUDiv },
            // operation_counter_t{ node_kind_t::kSDiv },
            // operation_counter_t{ node_kind_t::kURem },
            // operation_counter_t{ node_kind_t::kSRem },
            operation_counter_t{ node_kind_t::kOperation }
        };

        verilog_cell_counter_t verilog_counter;

        for (auto _ : state) {
            auto ci = make_circuit(input);
            for (auto &counter : counters) {
                counter.count(ci);
            }
            verilog_counter.count(ci);
        }

        for (const auto &counter : counters) {
            state.counters[counter.name()] = counter_t(counter.get(), average);
        }

        state.counters["VERILOG:CELLS"] = counter_t(verilog_counter.get(), average);
        state.counters["VERILOG:AND"] = counter_t(verilog_counter.get(verilog::cell_kind::and_cell), average);
        state.counters["VERILOG:NOT"] = counter_t(verilog_counter.get(verilog::cell_kind::not_cell), average);
        state.counters["VERILOG:XOR"] = counter_t(verilog_counter.get(verilog::cell_kind::xor_cell), average);
    }

} // namespace circ::bench
