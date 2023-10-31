/*
 * Copyright (c) 2022-present Trail of Bits, Inc.
 */

#include "lifter.hpp"
#include "options.hpp"
#include "stats.hpp"
#include "common.hpp"

#include <benchmark/benchmark.h>
#include <boost/filesystem.hpp>

#include <string_view>

namespace circ::bench
{
    static inline std::filesystem::path benchmark_directory(state_t& state) {
        auto top_level = options.directory;
        std::string name = state.name();
        std::string_view view = name;
        auto delim = view.find_first_of("/");
        auto pref = view.substr(0, delim);
        auto bench = view.substr(delim + 1);
        bench.remove_prefix(1);
        bench.remove_suffix(1);
        return top_level / pref / bench;
    }

    static inline std::filesystem::path setup(state_t& state) {
        auto path = benchmark_directory(state);
        std::filesystem::create_directories(path);
        return path;
    }

    template< typename input_t >
    void lift(state_t& state, input_t&& input) {
        auto wd = setup(state);

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

        verilog_cell_counter_t verilog_counter(wd);

        state.KeepRunningBatch(1);

        for (auto _ : state) {
            auto ci = make_circuit(input);
            for (auto &counter : counters) {
                counter.count(ci);
            }
            verilog_counter.count(ci);
        }

        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wimplicit-int-float-conversion"

        for (const auto &counter : counters) {
            state.counters[counter.name()] = counter_t(counter.get(), average);
        }

        #pragma GCC diagnostic pop

        update_varilog_counters(state, verilog_counter);
    }

    template< typename input_t >
    void run_on_verilog(state_t& state, input_t&& input) {
        auto wd = setup(state);
        verilog_cell_counter_t verilog_counter(wd);

        state.KeepRunningBatch(1);

        for (auto _ : state) {
            verilog_counter.count(input);
        }

        update_varilog_counters(state, verilog_counter);
    }

} // namespace circ::bench
