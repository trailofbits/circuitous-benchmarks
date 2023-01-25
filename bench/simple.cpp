/*
 * Copyright (c) 2022-present Trail of Bits, Inc.
 */

#include "runner.hpp"
#include <benchmark/benchmark.h>

namespace circ::bench
{
    static std::vector< uint8_t > convert_bytes(std::string_view token)
    {
        std::vector< uint8_t > out;
        for (auto i = 0U; i < token.size(); i += 2)
        {
            std::string aux = {token[i], token[i + 1]};
            auto casted = static_cast< uint8_t >(std::strtoul(aux.data(), nullptr, 16));
            out.push_back(casted);
        }
        return out;
    }

    template< typename input_t >
    void lift_bytes(state_t& state, input_t&& input) {
        return lift(state, instance_t{ convert_bytes(std::forward< input_t >(input)) });
    }

    // tiny86 set
    BENCHMARK_CAPTURE(lift_bytes, "min:AND_AL_IMMb", "24c3");
    BENCHMARK_CAPTURE(lift_bytes, "min:LEA_GPRv_AGEN_32", "8db40000008b45");
    BENCHMARK_CAPTURE(lift_bytes, "min:LODSB", "24c3");
    BENCHMARK_CAPTURE(lift_bytes, "min:LODSB", "ac");
    BENCHMARK_CAPTURE(lift_bytes, "min:LOOP_RELBRb", "e200");
    BENCHMARK_CAPTURE(lift_bytes, "min:MOVSB", "a4");
    BENCHMARK_CAPTURE(lift_bytes, "min:MOV_GPR8_GPR8_8A", "8ad0");

    BENCHMARK_CAPTURE(lift_bytes, "min:OR_GPRv_IMMz_32", "81c9000c0000");
    BENCHMARK_CAPTURE(lift_bytes, "min:POPAD_32", "61");

    BENCHMARK_CAPTURE(lift_bytes, "min:PUSH_IMMz_32", "68ddffff50");
    BENCHMARK_CAPTURE(lift_bytes, "min:PUSH_MEMv_32", "ff31");
    BENCHMARK_CAPTURE(lift_bytes, "min:SUB_AL_IMMb", "2c01");

    BENCHMARK_CAPTURE(lift_bytes, "min:SUB_GPRv_MEMv_32", "2b048dc4150508");
    BENCHMARK_CAPTURE(lift_bytes, "min:XOR_GPR8_GPR8_32", "32ff");
    BENCHMARK_CAPTURE(lift_bytes, "min:XOR_GPRv_IMMz_32", "81f3ffff0fb7");
    BENCHMARK_CAPTURE(lift_bytes, "min:XOR_OrAX_IMMz_32", "35fbffff8b");
    BENCHMARK_CAPTURE(lift_bytes, "min:DIV_GPRv_32", "f7f3");
    BENCHMARK_CAPTURE(lift_bytes, "min:DIV_MEMv_32", "f775f4");
    BENCHMARK_CAPTURE(lift_bytes, "min:AND_MEMb_GPR8", "205e5f");
    BENCHMARK_CAPTURE(lift_bytes, "min:ADD_GPR8_GPR8_02", "02ef");
    BENCHMARK_CAPTURE(lift_bytes, "min:ADD_MEMb_GPR8", "260083ec0c6a02");
    BENCHMARK_CAPTURE(lift_bytes, "min:IMUL_GPRv_MEMv_IMMz_16", "66691c610f85");
    BENCHMARK_CAPTURE(lift_bytes, "min:IMUL_GPRv_MEMv_IMMz_32", "699ce7ff31c08b8d98feff");
    BENCHMARK_CAPTURE(lift_bytes, "min:IMUL_MEMb", "f62ccdff83f800");
    BENCHMARK_CAPTURE(lift_bytes, "min:MOV_MEMv_GPRv_16", "6766895424");

    // multiplication by zero
    BENCHMARK_CAPTURE(lift_bytes, "min:MOV_RAX_RAX", "488b00");

    // and with one child
    BENCHMARK_CAPTURE(lift_bytes, "min:AND_ONE_CHILD_1", "90");
    BENCHMARK_CAPTURE(lift_bytes, "min:AND_ONE_CHILD_2", "488b90");

} // namespace circ::bench
