/*
 * Copyright (c) 2022-present Trail of Bits, Inc.
 */

#include "stats.hpp"

namespace circ::bench
{
    std::size_t count_nodes(const circuit_ptr &circuit) {
        nodes_counter_t counter;
        counter.Run(circuit.get());

        std::size_t count = 0;
        for (const auto &[_, c] : counter.nodes)
            count += counter.total_count(c);
        return count;
    }

    std::size_t count_nodes(const circuit_ptr &circuit, node_kind_t kind) {
        nodes_counter_t counter;
        counter.Run(circuit.get());
        return counter.total_count(kind);
    }

} // namespace circ::bench
