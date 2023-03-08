#!/usr/bin/env bash

mkdir -p /tmp/yosys
cp $1 /tmp/yosys

cat > /tmp/yosys/my_library.lib << EOF
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

echo "Preparing $2 from $1"
cat > /tmp/yosys/$1.ys << EOF
read_verilog $1;
hierarchy -check -top $2;
proc; opt; memory; opt; fsm; opt; techmap; opt;
flatten; opt -purge;
write_blif $1.blif;
stat;
EOF

yosys /tmp/yosys/$1.ys

exit 0
