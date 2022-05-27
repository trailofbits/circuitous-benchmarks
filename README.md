# circuitous-benchmarks

Build with prebuilt `circuitous`

```
cmake --preset ninja-cxx-common-x64-osx-rel

cmake --build --preset ninja-cxx-common-osx-rel
```

## Benchmark

```
./builds/ninja-cxx-common-x64-osx-rel/bench/circ-bench --benchmark_counters_tabular=true
```

## Distribution and Licensing

The views, opinions, and/or findings expressed are those of the author(s) and
should not be interpreted as representing the official views or policies of the
Department of Defense or the U.S. Government.

*circuitous-benchmarks* are licensed under the GNU AGPLv3 License. A copy of the
terms can be found in the [LICENSE](./LICENSE) file.
