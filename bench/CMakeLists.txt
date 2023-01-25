add_executable( circ-bench
    driver.cpp
    lifter.cpp
    simple.cpp
    from_source.cpp
    from_verilog.cpp
    optimize.cpp
    options.cpp
    stats.cpp
    yosys.cpp
)

target_link_libraries( circ-bench
    PRIVATE
        gap::gap

        fmt::fmt
        spdlog::spdlog

        circuitous::ir
        circuitous::lifter
        circuitous::transforms
        circuitous::printers

        circ_bench_project_options
        circ_bench_project_warnings
        benchmark::benchmark
        benchmark::benchmark_main
)
