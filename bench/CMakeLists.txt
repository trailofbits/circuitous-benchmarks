add_executable( circ-bench
    driver.cpp
    lifter.cpp
    simple.cpp
    from_source.cpp
    stats.cpp
)

target_link_libraries( circ-bench
    PRIVATE
        gap::gap

        circuitous::ir
        circuitous::lifter
        circuitous::transforms

        circ_bench_project_options
        circ_bench_project_warnings
        benchmark::benchmark
        benchmark::benchmark_main
)
