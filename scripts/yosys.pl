#!/usr/bin/perl
# Copyright (c) 2022-2023 Trail of Bits, Inc.

use strict;
use warnings;

use File::Temp qw/ tempfile tempdir /;
use File::Copy;

sub  trim { my $s = shift; $s =~ s/^\s+|\s+$//g; return $s };

my $dir = tempdir( CLEANUP => 1 );

my $script = <<EOF;
read_verilog $ARGV[0];
hierarchy -check -top $ARGV[1];
proc; opt; memory; opt; fsm; opt; techmap; opt;
flatten; opt -purge;
write_blif $ARGV[1]_$ARGV[0].blif;
stat;
EOF

my $scriptfile = "$dir/script.ys";
open(SCRIPT, '>', $scriptfile) or die $!;
print SCRIPT $script;
close(SCRIPT);

my $out = `yosys $scriptfile`;

my $and = 0;
my $mux = 0;
my $not = 0;
my $or  = 0;
my $xor = 0;

foreach (split(/\n/, $out)) {
    my $line = trim($_);
    if ($line =~ /_AND_\s+(\d+)/ ) {
        $and += $1;
    }

    if ($line =~ /_MUX_\s+(\d+)/ ) {
        $mux += $1;
    }

    if ($line =~ /_NOT_\s+(\d+)/ ) {
        $not += $1;
    }

    if ($line =~ /_OR_\s+(\d+)/ ) {
        $or += $1;
    }

    if ($line =~ /_XOR_\s+(\d+)/ ) {
        $xor += $1;
    }
}

print "$and:$mux:$not:$or:$xor\n";
