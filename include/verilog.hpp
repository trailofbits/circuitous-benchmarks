/*
 * Copyright (c) 2022-present Trail of Bits, Inc.
 */

#pragma once

#include <unordered_map>

namespace circ::bench::verilog
{
    enum class cell_kind
    {
        and_cell,
        mux_cell,
        not_cell,
         or_cell,
        xor_cell
    };

    using cells_count_t = std::unordered_map< cell_kind, std::size_t >;

} // namespace circ::bench::verilog
