/*
 * Copyright (c) 2022-present Trail of Bits, Inc.
 */

#include "optimize.hpp"
#include "options.hpp"

#include <circuitous/Transforms/Passes.hpp>

namespace circ::bench
{
    using DefaultOptimizer = Passes< Defensive< PassesBase > >;

    circuit_ptr optimize(circuit_ptr circuit) {
        DefaultOptimizer opt;

        if (options.eqsat) {
            auto &[_, pass] = opt.add_pass("eqsat");
            auto eqsat = std::dynamic_pointer_cast<circ::EqualitySaturationPass>(pass);
            eqsat->add_rules(circ::eqsat::parse_rules(options.eqsat->patterns.string()));
            return opt.run(std::move(circuit));
        }

        return circuit;
    }


} // namespace circ::bench
