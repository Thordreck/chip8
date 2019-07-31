#ifndef CHIP_8_OPCODES_HPP
#define CHIP_8_OPCODES_HPP

#include <utility>

namespace chip8::opcodes
{
    enum class OpCodes : uint8_t
    {
        OpCode_0NNN,
        OpCode_00EE,
        OpCode_00E0,
        OpCode_1NNN,
        OpCode_2NNN,
        OpCode_3XNN,
        OpCode_4XNN,
        OpCode_DXYN,
        OpCode_ANNN
    };

    using OpBytes = std::pair<uint8_t, uint8_t>;
}

#endif