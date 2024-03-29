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
        auto make_smithy(auto &&bytes) {
            auto ctx  = circuit_ctx{ to_string(options.os), to_string(options.arch) };
            if (options.smithy_type == smithy_type::muxes) {
                auto type = circ::lifter_kind::mux_heavy;
                return smithy(std::move(ctx)).make(type, bytes);
            }

            if (options.smithy_type == smithy_type::disjunctions) {
                auto type = circ::lifter_kind::disjunctions;
                return smithy(std::move(ctx)).make(type, bytes);
            }

            throw std::runtime_error("unknown smithy type");
        }

        circuit_owner_t make_circuit(auto &&bytes) {
            return postprocess(optimize(make_smithy(bytes)));
        }

    } // namespace detail

    static inline std::string_view as_string_view(const std::vector< uint8_t > &buf)
    {
        return std::string_view(reinterpret_cast<const char *>(buf.data()), buf.size());
    }

    static inline circuit_owner_t make_circuit(const instance_t &instance) {
        return detail::make_circuit(as_string_view(instance.bytes));
    }

    static inline circuit_owner_t make_circuit(ciff_file_t ciff_file) {
        return detail::make_circuit(ciff_reader().read(ciff_file.path).take_bytes());
    }

} // namespace circ::bench
