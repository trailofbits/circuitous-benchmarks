/*
 * Copyright (c) 2022-present Trail of Bits, Inc.
 */

#pragma once

#include <circuitous/IR/IR.hpp>
#include <circuitous/Lifter/Context.hpp>
#include <circuitous/Transforms/PassBase.hpp>

namespace circ::bench
{
    using circuit_ptr = circ::CircuitPtr;
    using circuit_ctx = circ::Ctx;

    using node_kind_t = circ::Operation::kind_t;

} // namespace circ::bench
