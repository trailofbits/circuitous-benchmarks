/*
 * Copyright (c) 2022-present Trail of Bits, Inc.
 */

#include "optimize.hpp"
#include "options.hpp"

#include <circuitous/Transforms/Passes.hpp>
#include <circuitous/Lifter/ToLLVM.hpp>
#include <circuitous/Lifter/LLVMToCircIR.hpp>
#include <circuitous/Lifter/BaseLifter.hpp>

#include <eqsat/pattern/parser.hpp>

namespace circ::bench
{
    using DefaultOptimizer = Passes< Defensive< PassesBase > >;

    circuit_owner_t optimize(circuit_owner_t circuit) {
        DefaultOptimizer opt;

        if (options.collapse_ops) {
            opt.template emplace_pass< circ::CollapseOpsPass >( "collapse-ops" );
        }

        if (options.eqsat) {
            auto pass = opt.template emplace_pass< circ::EqualitySaturationPass >("eqsat-pre");
            pass->add_rules( eqsat::parse_rules(options.eqsat->patterns.string()) );
        }

        if (options.conjure_alu_mul) {
            std::vector< circ::Operation::kind_t > kinds = {
                circ::Mul::kind,
            };

            opt.template emplace_pass< circ::ConjureALUPass >( "conjure-alu-mul", kinds );
        }

        if (options.conjure_alu_div) {
            std::vector< circ::Operation::kind_t > kinds = {
                circ::URem::kind,
                circ::SRem::kind,
                circ::UDiv::kind,
                circ::SDiv::kind,
            };

            opt.template emplace_pass< circ::ConjureALUPass >( "conjure-alu-div", kinds );
        }

        if (options.conjure_alu_add) {
            std::vector< circ::Operation::kind_t > kinds = {
                circ::Sub::kind,
                circ::Add::kind,
            };

            opt.template emplace_pass< circ::ConjureALUPass >( "conjure-alu-add", kinds );
        }

        if (options.eqsat && ( options.conjure_alu_mul || options.conjure_alu_add)) {
            auto pass = opt.template emplace_pass< circ::EqualitySaturationPass >("eqsat-post");
            pass->add_rules( eqsat::parse_rules(options.eqsat->patterns.string()) );
        }

        return opt.run(std::move(circuit));
    }

    circuit_owner_t llvm_optimize(circuit_owner_t circuit) {
        auto ctx = std::make_shared< llvm::LLVMContext >();
        auto mod = std::make_unique< llvm::Module >( "reopt", *ctx );

        auto fn = circ::convert_to_llvm( circuit.get(), mod.get(), "reoptfn" );
        circ::optimize_silently( { fn } );
        std::size_t ptr_size = options.arch == arch_t::x86 ? 32 : 64;
        return circ::lower_fn( fn, ptr_size );
    }

    circuit_owner_t postprocess(circuit_owner_t circuit) {
        if (options.postprocess_as_llvm) {
            circuit = llvm_optimize(std::move(circuit));
        }

        return circuit;
    }

} // namespace circ::bench
