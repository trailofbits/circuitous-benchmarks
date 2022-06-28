/*
 * Copyright (c) 2022-present Trail of Bits, Inc.
 */

#pragma once

#include "common.hpp"
#include "instance.hpp"
#include "options.hpp"

#include <circuitous/Lifter/CircuitSmithy.hpp>
#include <circuitous/Lifter/Context.hpp>
#include <circuitous/Support/Ciff.hpp>
#include <circuitous/Transforms/PassBase.hpp>
#include <utility>

namespace circ::bench
{
    using circuit_ptr = circ::CircuitPtr;
    using circuit_ctx = circ::Ctx;

    using smithy = circ::CircuitSmithy;

    using ciff_reader = circ::CIFFReader;

    namespace detail
    {
        circuit_ptr make_circuit(auto &&bytes, const options_t &options) {
            auto ctx = circuit_ctx{ to_string(options.os), to_string(options.arch) };
            return smithy(std::move(ctx)).smelt(bytes).forge();
        }
    } // namespace detail

    static inline circuit_ptr make_circuit(instance_t instance, const options_t &options) {
        return detail::make_circuit(instance.bytes, options);
    }

    static inline circuit_ptr make_circuit(ciff_file_t ciff_file, const options_t &options) {
        return detail::make_circuit(ciff_reader().read(ciff_file.path).take_bytes(), options);
    }

} // namespace circ::bench
