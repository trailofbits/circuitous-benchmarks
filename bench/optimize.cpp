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

    circuit_ptr optimize(circuit_ptr circuit) {
        DefaultOptimizer opt;

        if (options.mwa) {
            auto [ name, pass ] = opt.add_pass( "wip-merge-with-advices" );
            auto mwa_pass = std::dynamic_pointer_cast< circ::MergeWithAdvicesPass >( pass );
            circ::check( mwa_pass );
            mwa_pass->kinds = {
                circ::Mul::kind,  circ::Add::kind,  circ::Sub::kind,  circ::SDiv::kind,
                circ::UDiv::kind, circ::URem::kind, circ::SRem::kind, circ::PopulationCount::kind,
            };
            if (options.mwa.value() == mwa_t::all)
            {
                mwa_pass->kinds.push_back( circ::LShr::kind );
                mwa_pass->kinds.push_back( circ::AShr::kind );
                mwa_pass->kinds.push_back( circ::Shl::kind );
            }

            if (options.mwa.value() == mwa_t::propagate) {
                mwa_pass->propagate = true;
            }

            circuit = opt.run(std::move(circuit));

            auto [ llvm_ctx, lmodule ] = circ::convert_to_llvm( circuit.get(), "test_module" );
            circ::check( llvm_ctx && lmodule );

            for ( auto &fn : *lmodule )
                if ( auto str = circ::verify_function( fn ) )
                    circ::log_kill() << *str;

            circ::optimize_silently( lmodule.get() );

            circuit = circ::lower_module( lmodule.get(), 64 );
        }

        if (options.eqsat) {
            auto &[_, pass] = opt.add_pass("eqsat");
            auto eqsat = std::dynamic_pointer_cast< circ::EqualitySaturationPass >(pass);
            eqsat->add_rules(circ::eqsat::parse_rules(options.eqsat->patterns.string()));
            circuit = opt.run(std::move(circuit));
        }

        return circuit;
    }


} // namespace circ::bench
