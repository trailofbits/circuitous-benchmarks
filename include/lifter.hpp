/*
 * Copyright (c) 2022-present Trail of Bits, Inc.
 */

#pragma once

#include "common.hpp"
#include "instance.hpp"
#include "optimize.hpp"
#include "options.hpp"

#include <circuitous/Lifter/CircuitSmithy.hpp>
#include <circuitous/Lifter/Context.hpp>
#include <circuitous/Support/Ciff.hpp>
#include <circuitous/Transforms/PassBase.hpp>
#include <utility>

#include <spdlog/spdlog.h>

namespace circ::bench
{
    using smithy      = circ::CircuitSmithy;
    using ciff_reader = circ::CIFFReader;

    namespace detail
    {
        circuit_owner_t make_circuit(auto &&bytes) {
            auto ctx  = circuit_ctx{ to_string(options.os), to_string(options.arch) };
            auto circ = smithy(std::move(ctx)).smelt(bytes).forge();
            return optimize(std::move(circ));
        }
    } // namespace detail

    static inline std::string_view as_string_view(const std::vector< uint8_t > &buf)
    {
        return std::string_view( reinterpret_cast<const char *>(buf.data()), buf.size());
    }

    static inline circuit_owner_t make_circuit(const instance_t &instance) {
        return detail::make_circuit(as_string_view(instance.bytes));
    }

    static inline circuit_owner_t make_circuit(ciff_file_t ciff_file) {
        return detail::make_circuit(ciff_reader().read(ciff_file.path).take_bytes());
    }

} // namespace circ::bench
