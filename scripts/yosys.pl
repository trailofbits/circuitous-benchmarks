#!/usr/bin/perl
# Copyright (c) 2022 Trail of Bits, Inc.

use strict;
use warnings;

use File::Temp qw/ tempfile tempdir /;
use File::Copy;

sub  trim { my $s = shift; $s =~ s/^\s+|\s+$//g; return $s };

my $dir = tempdir( CLEANUP => 1 );

my $lib = <<EOF;
library(demo) {
    cell(NOT) {
        area: 2;
        pin(IN) { direction: input; }
        pin(OUT) { direction: output; function: "IN'"; }
    }
    cell(XOR) {
        area: 1;
        pin(A) { direction: input; }
        pin(B) { direction: input; }
        pin(OUT) { direction: output; function: "(A^B)"; }
    }
    cell(AND) {
        area: 100;
        preferred: false;
        pin(A) { direction: input; }
        pin(B) { direction: input; }
        pin(OUT) { direction: output; function: "(A&B)"; }
    }
    cell(BUF) {
        area: 1;
        pin(IN) { direction: input; }
        pin(OUT) { direction: output; function: "IN"; }
    }
}
EOF

my $libfile = "$dir/library.lib";
open(LIB, '>', $libfile) or die $!;
print LIB $lib;
close(LIB);

my $script = <<EOF;
read_verilog $ARGV[0];
hierarchy -check -top $ARGV[1];
proc; opt; memory; opt; fsm; opt; techmap; opt;
flatten; opt -purge;
abc -liberty $libfile;
opt;
read_liberty -lib $libfile;
write_verilog netlist_$ARGV[1]_$ARGV[0];
stat;
EOF

my $scriptfile = "$dir/script.ys";
open(SCRIPT, '>', $scriptfile) or die $!;
print SCRIPT $script;
close(SCRIPT);

my $out = `yosys $scriptfile`;

my $and = 0;
my $not = 0;
my $xor = 0;
foreach (split(/\n/, $out)) {
    my $line = trim($_);
    if($line =~ /AND\s+(\d+)/ ) {
        $and += $1;
    }

    if($line =~ /NOT\s+(\d+)/ ) {
        $not += $1;
    }

    if($line =~ /XOR\s+(\d+)/ ) {
        $xor += $1;
    }
}

print "$and:$not:$xor\n";
