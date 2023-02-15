/*
 * Copyright (c) 2022-present Trail of Bits, Inc.
 */

#pragma once

#include <string>
#include <optional>
#include <filesystem>

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

    static inline os_t os_from_string(std::string_view str) {
        if (str == "macos") return os_t::macos;
        if (str == "linux") return os_t::linux;
        throw std::runtime_error("unknown os");
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

    static inline arch_t arch_from_string(std::string_view str) {
        if (str == "x86") return arch_t::x86;
        if (str == "amd64") return arch_t::amd64;
        if (str == "amd64_avx") return arch_t::amd64_avx;
        throw std::runtime_error("unknown architecture");
    }

    struct eqsat_options_t {
        std::filesystem::path patterns;
    };

    struct options_t {
        os_t os;
        arch_t arch;
        std::optional< eqsat_options_t > eqsat;

        std::filesystem::path directory;
        bool keep_files        = false;

        bool overflow_flag_mix = false;
        bool merge_advices     = false;
        bool collapse_ops      = false;

        bool conjure_alu_mul   = false;
        bool conjure_alu_add   = false;

        bool switch_as_mux     = false;

        bool postprocess_as_llvm  = false;
    };

    extern options_t options;

} // namespace circ::bench
