/*
 * Copyright (c) 2022-present Trail of Bits, Inc.
 */

#pragma once

namespace circ::bench
{
    enum class os_t
    {
        macos,
        linux
    };

    enum class arch_t
    {
        x86,
        amd64,
        amd64_avx
    };

    struct options_t {
        os_t os;
        arch_t arch;
    };
} // namespace circ::bench
