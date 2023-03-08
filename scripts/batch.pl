#!/usr/bin/perl
# Copyright (c) 2023 Trail of Bits, Inc.

my $tool = "./builds/cxx-common-custom/bench/circ-bench";

sub run_instance {
    my $instance = $_;
    my $opts = "--benchmark_counters_tabular=true --benchmark_out_format=json --benchmark_out=$instance.json --out_dir $instance --benchmark_filter=big";

    if (index($instance, "sim") != -1) {
        $opts = $opts . " --simplify";
    }

    if (index($instance, "mux") != -1) {
        $opts = $opts . " --switch_as_mux";
    }

    if (index($instance, "conmul") != -1) {
        $opts = $opts . " --conjure_alu_mul";
    }

    if (index($instance, "conadd") != -1) {
        $opts = $opts . " --conjure_alu_add";
    }

    if (index($instance, "condiv") != -1) {
        $opts = $opts . " --conjure_alu_div";
    }

    if (index($instance, "conall") != -1) {
        $opts = $opts . " --conjure_alu";
    }

    if (index($instance, "llvm") != -1) {
        $opts = $opts . " --postprocess_as_llvm";
    }

    if (index($instance, "eqsat") != -1) {
        $opts = $opts . " --patterns reduce.yml";
    }

    print `screen -d -m $tool $opts`;
}

# @instances = ("noop", "eqsat", "sim-eqsat-llvm", "sim-conmul-mux-llvm", "sim-conmul-condiv-mux-llvm", "sim-conall--mux-llvm", "sim-conall--llvm", "sim-conall-mux-llvm-eqsat");
# @instances = ("llvm", "conall-llvm", "conmul-llvm", "condiv-llvm");
@instances = ("sim-conadd-mux-llvm-eqsat", "sim-conmul-mux-llvm-eqsat", "sim-conmul-condiv-mux-llvm-eqsat", "sim-condiv-mux-llvm-eqsat");

foreach (@instances) {
    run_instance($_)
}
