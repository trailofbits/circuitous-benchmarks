/*
 * Copyright (c) 2022-present Trail of Bits, Inc.
 */

#include "stats.hpp"

namespace circ::bench
{
    std::size_t count_nodes(const circuit_owner_t &circuit) {
        nodes_counter_t counter;
        counter.Run(circuit->root);

        std::size_t count = 0;
        for (const auto &[_, c] : counter.nodes)
            count += counter.total_count(c);
        return count;
    }

    std::size_t count_nodes(const circuit_owner_t &circuit, node_kind_t kind) {
        nodes_counter_t counter;
        counter.Run(circuit->root);
        return counter.total_count(kind);
    }

    void update_varilog_counters(state_t &state, const verilog_cell_counter_t &counter) {
        state.counters["V:CELLS"] = counter_t(counter.get(), average);
        state.counters["V:AND"] = counter_t(counter.get(verilog::cell_kind::and_cell), average);
        state.counters["V:MUX"] = counter_t(counter.get(verilog::cell_kind::mux_cell), average);
        state.counters["V:NOT"] = counter_t(counter.get(verilog::cell_kind::not_cell), average);
        state.counters["V:OR"]  = counter_t(counter.get(verilog::cell_kind::or_cell), average);
        state.counters["V:XOR"] = counter_t(counter.get(verilog::cell_kind::xor_cell), average);
    }

} // namespace circ::bench
