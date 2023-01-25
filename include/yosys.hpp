/*
 * Copyright (c) 2022-present Trail of Bits, Inc.
 */

#pragma once

#include "common.hpp"
#include "verilog.hpp"
#include "instance.hpp"

#include <fstream>
#include <unordered_map>

namespace circ::bench::yosys
{
    verilog::cells_count_t run(const circuit_owner_t &circuit);

    verilog::cells_count_t run(const verilog_file_t &file);
} // namespace circ::bench::yosys
