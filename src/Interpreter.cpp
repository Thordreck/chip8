#include "Interpreter.hpp"

#include "details/memory.hpp"    // for dumpRomToMemory, initSystemMemory
#include "timers/TimerImpl.hpp"  // for GeneralizedTimer::GeneralizedTimer<R...

namespace chip8
{
    Interpreter::Interpreter() :
        sound_timer_([this] () { speaker_.Play(); }, [this] () { speaker_.Stop(); } )
    {
        InitializeRam();
    }

    void Interpreter::LoadRom(std::string_view _rom)
    {
        try
        {
            details::dumpRomToMemory(_rom, program_memory_, ram_.end());
        }
        catch(const std::runtime_error& ex)
        {
            throw Chip8Exception("Error loading rom file ", _rom.data(), ": ", ex.what());
        }
    }

    void Interpreter::StartRom()
    {
        program_counter_ = program_memory_;

        //TODO: when should we stop?
        while(true)
        {
            opcodes::OpBytes op_bytes(*program_counter_, *(program_counter_ + 1));
            processInstruction(op_bytes);

            delay_timer_.Tick();
            sound_timer_.Tick();
        }
    }

    void Interpreter::InitializeRam()
    {
        details::initSystemMemory(ram_.begin(), end_interpreter_memory_);
    }

    void Interpreter::processInstruction(const opcodes::OpBytes& _op_bytes)
    {
        using namespace opcodes;

        //TODO: check if the instructions generated by the compiler are the same with if/else and a switch
        if ((_op_bytes.first & 0xF0) == 0x00 && (_op_bytes.second & 0xF0) != 0xE0)  // NOLINT
        {
            ExecuteInstruction<OpCodes::OpCode_0NNN>(_op_bytes); 
        }
        else if (_op_bytes.first == 0x00 && _op_bytes.second == 0xE0) // NOLINT
        {
            ExecuteInstruction<OpCodes::OpCode_00E0>(_op_bytes); 
        }
        else if (_op_bytes.first == 0x00 && _op_bytes.second == 0xEE) // NOLINT
        {
            ExecuteInstruction<OpCodes::OpCode_00EE>(_op_bytes); 
        }
        else if ((_op_bytes.first & 0xF0) == 0x10) // NOLINT
        {
            ExecuteInstruction<OpCodes::OpCode_1NNN>(_op_bytes); 
        }
        else if ((_op_bytes.first & 0xF0) == 0x20) // NOLINT
        {
            ExecuteInstruction<OpCodes::OpCode_2NNN>(_op_bytes); 
        }
        else if ((_op_bytes.first & 0xF0) == 0x30) // NOLINT
        {
            ExecuteInstruction<OpCodes::OpCode_3XNN>(_op_bytes); 
        }
        else if ((_op_bytes.first & 0xF0) == 0x40) // NOLINT
        {
            ExecuteInstruction<OpCodes::OpCode_4XNN>(_op_bytes); 
        }
        else if ((_op_bytes.first & 0xF0) == 0x50 && (_op_bytes.second & 0x0F) == 0x0) // NOLINT
        {
            ExecuteInstruction<OpCodes::OpCode_5XY0>(_op_bytes); 
        }
        else if ((_op_bytes.first & 0xF0) == 0x60) // NOLINT
        {
            ExecuteInstruction<OpCodes::OpCode_6XNN>(_op_bytes); 
        }
        else if ((_op_bytes.first & 0xF0) == 0x70) // NOLINT
        {
            ExecuteInstruction<OpCodes::OpCode_7XNN>(_op_bytes); 
        }
        else if ((_op_bytes.first & 0xF0) == 0x80 && (_op_bytes.second & 0x0F) == 0x0) // NOLINT
        {
            ExecuteInstruction<OpCodes::OpCode_8XY0>(_op_bytes); 
        }
        else if ((_op_bytes.first & 0xF0) == 0x80 && (_op_bytes.second & 0x0F) == 0x1) // NOLINT
        {
            ExecuteInstruction<OpCodes::OpCode_8XY1>(_op_bytes); 
        }
        else if ((_op_bytes.first & 0xF0) == 0x80 && (_op_bytes.second & 0x0F) == 0x2) // NOLINT
        {
            ExecuteInstruction<OpCodes::OpCode_8XY2>(_op_bytes); 
        }
        else if ((_op_bytes.first & 0xF0) == 0x80 && (_op_bytes.second & 0x0F) == 0x3) // NOLINT
        {
            ExecuteInstruction<OpCodes::OpCode_8XY3>(_op_bytes); 
        }
        else if ((_op_bytes.first & 0xF0) == 0x80 && (_op_bytes.second & 0x0F) == 0x4) // NOLINT
        {
            ExecuteInstruction<OpCodes::OpCode_8XY4>(_op_bytes); 
        }
        else if ((_op_bytes.first & 0xF0) == 0x80 && (_op_bytes.second & 0x0F) == 0x5) // NOLINT
        {
            ExecuteInstruction<OpCodes::OpCode_8XY5>(_op_bytes); 
        }
        else if ((_op_bytes.first & 0xF0) == 0x80 && (_op_bytes.second & 0x0F) == 0x6) // NOLINT
        {
            ExecuteInstruction<OpCodes::OpCode_8XY6>(_op_bytes); 
        }
        else if ((_op_bytes.first & 0xF0) == 0x80 && (_op_bytes.second & 0x0F) == 0x7) // NOLINT
        {
            ExecuteInstruction<OpCodes::OpCode_8XY7>(_op_bytes); 
        }
        else if ((_op_bytes.first & 0xF0) == 0x80 && (_op_bytes.second & 0x0F) == 0xE) // NOLINT
        {
            ExecuteInstruction<OpCodes::OpCode_8XYE>(_op_bytes); 
        }
        else if ((_op_bytes.first & 0xF0) == 0x90 && (_op_bytes.second & 0x0F) == 0x0) // NOLINT
        {
            ExecuteInstruction<OpCodes::OpCode_9XY0>(_op_bytes); 
        }
        else if ((_op_bytes.first & 0xF0) == 0xD0) // NOLINT
        {
            ExecuteInstruction<OpCodes::OpCode_DXYN>(_op_bytes); 
        }
        else if ((_op_bytes.first & 0xF0) == 0xE0 && _op_bytes.second == 0x9E) // NOLINT
        {
            ExecuteInstruction<OpCodes::OpCode_EX9E>(_op_bytes); 
        }
        else if ((_op_bytes.first & 0xF0) == 0xE0 && _op_bytes.second == 0xA1) // NOLINT
        {
            ExecuteInstruction<OpCodes::OpCode_EXA1>(_op_bytes); 
        }
        else if ((_op_bytes.first & 0xF0) == 0xA0) // NOLINT
        {
            ExecuteInstruction<OpCodes::OpCode_ANNN>(_op_bytes); 
        }
        else if ((_op_bytes.first & 0xF0) == 0xB0) // NOLINT
        {
            ExecuteInstruction<OpCodes::OpCode_BNNN>(_op_bytes); 
        }
        else if ((_op_bytes.first & 0xF0) == 0xC0) // NOLINT
        {
            ExecuteInstruction<OpCodes::OpCode_CXNN>(_op_bytes); 
        }
        else if ((_op_bytes.first & 0xF0) == 0xF0 && _op_bytes.second == 0x07) // NOLINT
        {
            ExecuteInstruction<OpCodes::OpCode_FX07>(_op_bytes); 
        }
        else if ((_op_bytes.first & 0xF0) == 0xF0 && _op_bytes.second == 0x0A) // NOLINT
        {
            ExecuteInstruction<OpCodes::OpCode_FX0A>(_op_bytes); 
        }
        else if ((_op_bytes.first & 0xF0) == 0xF0 && _op_bytes.second == 0x15) // NOLINT
        {
            ExecuteInstruction<OpCodes::OpCode_FX15>(_op_bytes); 
        }
        else if ((_op_bytes.first & 0xF0) == 0xF0 && _op_bytes.second == 0x18) // NOLINT
        {
            ExecuteInstruction<OpCodes::OpCode_FX18>(_op_bytes); 
        }
        else if ((_op_bytes.first & 0xF0) == 0xF0 && _op_bytes.second == 0x1E) // NOLINT
        {
            ExecuteInstruction<OpCodes::OpCode_FX1E>(_op_bytes); 
        }
        else
        {
            throw OpCodeException(_op_bytes, "Instruction not recognized");
        }
    }
}
