/*
 * Copyright (c) 2022-present Trail of Bits, Inc.
 */

#pragma once

#include "common.hpp"
#include "yosys.hpp"
#include "verilog.hpp"

#include <circuitous/IR/Circuit.hpp>
#include <circuitous/IR/Cost.hpp>
#include <string>

namespace circ::bench
{
    using nodes_counter_t = circ::RawNodesCounter;

    std::size_t count_nodes(const circuit_owner_t &circuit);
    std::size_t count_nodes(const circuit_owner_t &circuit, node_kind_t kind);

    struct operation_counter_t {
        explicit operation_counter_t(node_kind_t kind)
            : _kind(kind) {}

        void count(const circuit_owner_t &circuit) {
            if (_kind == node_kind_t::kOperation)
                _count += count_nodes(circuit);
            _count += count_nodes(circuit, _kind);
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

    struct verilog_cell_counter_t {
        explicit verilog_cell_counter_t( std::filesystem::path wd )
            : working_directory(wd)
        {}

        void count(const circuit_owner_t &circuit) {
            for (const auto &[k, v] : yosys::run(working_directory, circuit)) {
                _counts[k] += v;
            }
        }

        std::size_t get() const {
            std::size_t cell_count = 0;
            for (auto [_, c] : _counts) {
                cell_count += c;
            }

            return cell_count;
        }

        std::size_t get(verilog::cell_kind cell) const { return _counts.at(cell); }

      private:
        verilog::cells_count_t _counts;
        std::filesystem::path working_directory;
    };

    void update_varilog_counters(state_t &state, const verilog_cell_counter_t &counter);

} // namespace circ::bench
