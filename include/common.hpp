/*
 * Copyright (c) 2022-present Trail of Bits, Inc.
 */

#pragma once

#include <circuitous/IR/IR.hpp>
#include <circuitous/Lifter/Context.hpp>
#include <circuitous/Transforms/PassBase.hpp>

#include <benchmark/benchmark.h>

namespace circ::bench
{
    using circuit_ctx = circ::Ctx;

    using node_kind_t = circ::Operation::kind_t;

    using state_t = benchmark::State;

    using counter_t = benchmark::Counter;

    constexpr auto average = counter_t::kAvgIterations;

} // namespace circ::bench
