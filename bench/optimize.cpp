/*
 * Copyright (c) 2022-present Trail of Bits, Inc.
 */

#include "optimize.hpp"
#include "options.hpp"

#include <circuitous/Transforms/Passes.hpp>
#include <circuitous/Lifter/ToLLVM.hpp>
#include <circuitous/Lifter/LLVMToCircIR.hpp>
#include <circuitous/Lifter/BaseLifter.hpp>

namespace circ::bench
{
    using DefaultOptimizer = Passes< Defensive< PassesBase > >;

    circuit_owner_t optimize(circuit_owner_t circuit) {
        DefaultOptimizer opt;

        if (options.overflow_flag_mix) {
            opt.template emplace_pass< circ::RemillOFPatch >( "overflow-flag-fix" );
        }

        if (options.merge_advices) {
            opt.template emplace_pass< circ::MergeAdviceConstraints >( "merge-advices" );
        }

        if (options.collapse_ops) {
            opt.template emplace_pass< circ::CollapseOpsPass >( "collapse-ops" );
        }

        if (options.conjure_alu) {
            std::vector< circ::Operation::kind_t > kinds = {
                circ::Mul::kind,
                circ::Add::kind,
                circ::Sub::kind,

                circ::UDiv::kind,
                circ::SDiv::kind,

                circ::URem::kind,
                circ::SRem::kind,

                circ::PopulationCount::kind,
                circ::Select::kind,
            };

            opt.template emplace_pass< circ::ConjureALUPass >( "conjure-alu", kinds );
        }

        // if (options.eqsat) {
        //     auto &[_, pass] = opt.add_pass("eqsat");
        //     auto eqsat = std::dynamic_pointer_cast< circ::EqualitySaturationPass >(pass);
        //     eqsat->add_rules(circ::eqsat::parse_rules(options.eqsat->patterns.string()));
        //     circuit = opt.run(std::move(circuit));
        // }

        return circuit;
    }


} // namespace circ::bench
