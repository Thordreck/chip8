#include "Interpreter.hpp"
#include "details/memory.hpp"

namespace chip8
{
    Interpreter::Interpreter()
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
            throw std::runtime_error { std::string{ "Error loading rom file " } + _rom.data() + ": " + ex.what() };
        }
    }

    void Interpreter::StartRom()
    {
        program_counter_ = ram_.begin();

        //TODO: when should we stop?
        while(true)
        {
            details::OpBytes op_bytes(*program_counter_, *(program_counter_ + 1));
            auto next_instruction = details::parseInstruction(op_bytes, *this);

            next_instruction();

            delay_timer_.Tick();
            sound_timer_.Tick();
        }
    }

    //Private
    void Interpreter::InitializeRam()
    {
        details::initSystemMemory(ram_.begin(), end_interpreter_memory_);
    }
}
