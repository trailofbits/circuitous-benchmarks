/*
 * Copyright (c) 2022-present Trail of Bits, Inc.
 */

#pragma once

#include "common.hpp"
#include "verilog.hpp"

#include <fstream>
#include <unordered_map>

namespace circ::bench::yosys
{
    verilog::cells_count_t run(const circuit_ptr &circuit);
} // namespace circ::bench::yosys
