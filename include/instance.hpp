/*
 * Copyright (c) 2022-present Trail of Bits, Inc.
 */

#pragma once

#include "options.hpp"

#include <string_view>

namespace circ::bench
{
    struct instance_t {
        std::string_view bytes;
    };
} // namespace circ::bench
