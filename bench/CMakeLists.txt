add_executable( circ-bench
    driver.cpp
    simple.cpp
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

