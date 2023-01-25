/*
 * Copyright (c) 2022-present Trail of Bits, Inc.
 */

#pragma once

#include "options.hpp"

#include <filesystem>
#include <string_view>

namespace circ::bench
{
    struct instance_t {
        std::string_view bytes;
    };

    struct ciff_file_t {
        std::filesystem::path path;
    };

    struct verilog_file_t {
        std::filesystem::path path;
    };
} // namespace circ::bench
