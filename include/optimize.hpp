/*
 * Copyright (c) 2022-present Trail of Bits, Inc.
 */

#pragma once

#include "common.hpp"

namespace circ::bench
{
    circuit_owner_t optimize(circuit_owner_t circuit);

    circuit_owner_t postprocess(circuit_owner_t circuit);
} // namespace circ::bench
