/*
 * Copyright (c) 2022-present Trail of Bits, Inc.
 */

#pragma once

#include "instance.hpp"
#include "options.hpp"
#include "common.hpp"

#include <circuitous/Lifter/CircuitSmithy.hpp>
#include <circuitous/Lifter/Context.hpp>
#include <circuitous/Transforms/PassBase.hpp>
#include <utility>

namespace circ::bench
{
    using circuit_ptr = circ::CircuitPtr;
    using circuit_ctx = circ::Ctx;

    using smithy = circ::CircuitSmithy;

    circuit_ptr make_circuit(instance_t instance, options_t options) {
        auto ctx = circuit_ctx{ to_string(options.os), to_string(options.arch) };
        return smithy(std::move(ctx)).smelt(instance.bytes).forge();
    }

} // namespace circ::bench
