/*
 * Copyright (c) 2022-present Trail of Bits, Inc.
 */

#pragma once

#include "common.hpp"

#include <circuitous/IR/Circuit.hpp>
#include <circuitous/IR/Cost.hpp>
#include <string>
#include <unordered_map>

namespace circ::bench
{
    using nodes_counter_t = circ::RawNodesCounter;

    std::size_t count_nodes(const circuit_ptr &circuit);
    std::size_t count_nodes(const circuit_ptr &circuit, node_kind_t kind);

    struct operation_counter_t {
        explicit operation_counter_t(node_kind_t kind)
            : _kind(kind) {}

        std::size_t count(const circuit_ptr &circuit) {
            if (_kind == node_kind_t::kOperation)
                return _count += count_nodes(circuit);
            return _count += count_nodes(circuit, _kind);
        }

        std::string name() const {
            if (_kind == node_kind_t::kOperation)
                return "Nodes";
            return circ::op_code_str(_kind);
        }

        std::size_t get() const { return _count; }

      private:
        std::size_t _count = 0;
        node_kind_t _kind;
    };

    enum class verilog_kind
    {
        and_cell,
        not_cell,
        xor_cell
    };

    struct verilog_cell_counter_t {
        std::size_t count(const circuit_ptr &circuit) {
            // return _count += count_nodes(circuit, _kind);
            return 0;
        }

        std::size_t get() const {
            std::size_t cell_count = 0;
            for (auto [_, c] : _counts) {
                cell_count += c;
            }

            return cell_count;
        }

        std::size_t get(verilog_kind cell) const { return _counts.at(cell); }

      private:
        std::unordered_map< verilog_kind, std::size_t > _counts;
    };

} // namespace circ::bench
