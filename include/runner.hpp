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
            operation_counter_t{ node_kind_t::kSub },
            operation_counter_t{ node_kind_t::kUDiv },
            operation_counter_t{ node_kind_t::kSDiv },
            operation_counter_t{ node_kind_t::kURem },
            operation_counter_t{ node_kind_t::kSRem },
            operation_counter_t{ node_kind_t::kLShr },
            operation_counter_t{ node_kind_t::kAShr },
            operation_counter_t{ node_kind_t::kShl },
            operation_counter_t{ node_kind_t::kPopulationCount },
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

        update_varilog_counters(state, verilog_counter);
    }

    template< typename input_t >
    void run_on_verilog(state_t& state, input_t&& input) {
        verilog_cell_counter_t verilog_counter;

        for (auto _ : state) {
            verilog_counter.count(input);
        }

        update_varilog_counters(state, verilog_counter);
    }

} // namespace circ::bench
