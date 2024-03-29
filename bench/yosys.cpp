/*
 * Copyright (c) 2022-present Trail of Bits, Inc.
 */

#include "yosys.hpp"

#include <cstdlib>
#include <charconv>

#include <fmt/core.h>
#include <circuitous/Printers/Verilog.hpp>

namespace circ::bench::yosys
{
    struct command_result_t {
        std::string output;
        int exitcode;

        friend std::ostream& operator<<(std::ostream &os, const command_result_t &result) {
            return os << "exit code: " << result.exitcode << " output: " << result.output;
        }

        bool operator==(const command_result_t& other) const = default;
    };

    struct command {
        static command_result_t exec(const std::string &cmd) {
            command_result_t result;

            auto pipe = popen(cmd.c_str(), "r");
            if (!pipe) {
                throw std::runtime_error("popen() failed!");
            }

            try {
                std::array<char, 1024> buffer {};

                std::size_t bytes;
                while ((bytes = std::fread(buffer.data(), sizeof(buffer.at(0)), sizeof(buffer), pipe)) != 0) {
                    result.output += std::string(buffer.data(), bytes);
                }
            } catch (std::exception &e) {
                pclose(pipe);
                throw;
            }

            auto close = pclose(pipe);
            result.exitcode = WEXITSTATUS(close);

            return result;
        }
    };

    std::vector< std::uint64_t > stats(std::string_view output) {
        std::string delim = ":";
        std::vector< std::uint64_t > nums;

        auto get_num = [] (std::string_view str) {
            std::size_t num;
            std::from_chars(str.data(), str.data() + str.size(), num);
            return num;
        };

        std::size_t pos;
        while ((pos = output.find(delim)) != std::string::npos) {
            auto token = output.substr(0, pos);
            nums.push_back(get_num(token));
            output.remove_prefix(pos + 1);
        }

        nums.push_back(get_num(output));
        return nums;
    }

    verilog::cells_count_t run(std::filesystem::path wd, const circuit_owner_t &circuit) {
        auto name = "circuit";
        auto file_name = wd / "circuit.v";

        std::ofstream os(file_name);
        circ::print::verilog::print(os, name, circuit.get(), options.switch_as_mux);
        auto counts = run(verilog_file_t{ file_name });
        return counts;
    }

    verilog::cells_count_t run(const verilog_file_t &file) {
        verilog::cells_count_t count = {
            {verilog::cell_kind::and_cell, 0},
            {verilog::cell_kind::mux_cell, 0},
            {verilog::cell_kind::not_cell, 0},
            {verilog::cell_kind::or_cell,  0},
            {verilog::cell_kind::xor_cell, 0}
        };

        std::string cmd = fmt::format("perl ./scripts/yosys.pl {} {}", file.path.string(), "circuit");
        auto result = command::exec(cmd);

        auto nums = stats(result.output);

        count[verilog::cell_kind::and_cell] += nums[0];
        count[verilog::cell_kind::mux_cell] += nums[1];
        count[verilog::cell_kind::not_cell] += nums[2];
        count[verilog::cell_kind::or_cell]  += nums[3];
        count[verilog::cell_kind::xor_cell] += nums[4];

        return count;
    }

} // namespace circ::bench::yosys
