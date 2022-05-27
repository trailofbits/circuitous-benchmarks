/*
 * Copyright (c) 2022-present Trail of Bits, Inc.
 */

#pragma once

#include <string>

namespace circ::bench
{
    enum class os_t
    {
        macos,
        linux
    };

    static inline std::string to_string(os_t os) {
        switch (os) {
            case os_t::macos: return "macos";
            case os_t::linux: return "linux";
        }
    }

    enum class arch_t
    {
        x86,
        amd64,
        amd64_avx
    };

    static inline std::string to_string(arch_t arch) {
        switch (arch) {
            case arch_t::x86: return "x86";
            case arch_t::amd64: return "amd64";
            case arch_t::amd64_avx: return "amd64_avx";
        }
    }

    struct options_t {
        os_t os;
        arch_t arch;
    };
} // namespace circ::bench
