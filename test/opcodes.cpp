#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "Interpreter.hpp"
#include "io/display/Renderer.hpp"

#include <chrono>
#include <iostream>
#include <thread>
#include <string>

#include "details/opcodes.hpp"

namespace chip8::test
{

    class Interpreter : public chip8::Interpreter { 
        public: 
            /**
            * Test OpCode 0NNN 
            * Jump to a machine code routine at nnn. This instruction is only used on the old computers on which Chip-8
            * was originally implemented. It is ignored by modern interpreters. This will not be implemented.
            */
            void TestOpCode_0NNN() {
                try {
                    chip8::opcodes::OpBytes op{0x01, 0x23}; // NOLINT
                    processInstruction(op);
                    FAIL() << "Expected 0x0NNN chip8::OpCodeException";
                }
                catch(chip8::OpCodeException const & err) {
                    EXPECT_THAT(err.what(), ::testing::MatchesRegex("0x0...: Instruction not implemented. ROM not supported."));
                }
                catch(...) {
                    FAIL() << "Expected 0x0NNN chip8::OpCodeException";
                }
            }

            /**
            * Test OpCode 00E0 
            * Clear the pixel frame buffer.
            */
            void TestOpCode_00E0() {
                pixels_.WriteByteAt(0,0, 0xFF); // NOLINT

                chip8::opcodes::OpBytes op{0x00, 0xE0}; // NOLINT
                processInstruction(op);

                for (const auto& pixel : pixels_)
                {
                    EXPECT_TRUE(pixel == 0);
                }
            }

            /**
            * Test OpCode 00EE
            * Return from a subrutine.
            */
            void TestOpCode_00EE() {
                // Set the program counter to 0x234 and jump to subrutine starting at 0x111
                // The return from subrutine

                program_counter_ = ram_.begin() + 0x234; // NOLINT

                chip8::opcodes::OpBytes op_2NNN{0x21, 0x11}; // NOLINT
                processInstruction(op_2NNN);

                EXPECT_EQ(stack_.back() - ram_.begin(), 0x234); // NOLINT
                EXPECT_EQ(program_counter_ - ram_.begin(), 0x111); // NOLINT

                auto stack_size_before_returning = stack_.size();

                chip8::opcodes::OpBytes op_00EE{0x00, 0xEE}; // NOLINT
                processInstruction(op_00EE);

                auto stack_size_after_returning = stack_.size();

                EXPECT_EQ(stack_size_before_returning - stack_size_after_returning, 1); // NOLINT
                EXPECT_EQ(program_counter_ - ram_.begin(), 0x234); // NOLINT

            }

            /**
            * Test OpCode 1NNN
            * Jump to address NNN.
            */
            void TestOpCode_1NNN() {
                chip8::opcodes::OpBytes op{0x11, 0x11}; // NOLINT
                processInstruction(op);
                EXPECT_EQ(program_counter_ - ram_.begin(), 0x111); // NOLINT
            }

            /**
            * Test OpCode 2NNN 
            * Execute subroutine starting at address NNN
            */
            void TestOpCode_2NNN() {
                // Set the program counter to 0x234 and jump to subrutine starting at 0x111

                program_counter_ = ram_.begin() + 0x234; // NOLINT

                chip8::opcodes::OpBytes op{0x21, 0x11}; // NOLINT
                processInstruction(op);

                EXPECT_EQ(stack_.back() - ram_.begin(), 0x234); // NOLINT
                EXPECT_EQ(program_counter_ - ram_.begin(), 0x111); // NOLINT

            }

            /**
            * Test OpCode 3XNN 
            * Skip the following instruction if the value of register VX equals NN
            */
            void TestOpCode_3XNN()
            {
                program_counter_ = program_memory_; // NOLINT

                data_registers_[0].Set(10);
                data_registers_[1].Set(11);

                chip8::opcodes::OpBytes op {0x31, 11}; // NOLINT
                processInstruction(op);

                //The program counter should have jumped the next instruction
                EXPECT_EQ(std::distance(program_memory_, program_counter_), 2); // NOLINT

                op = {0x30, 11}; // NOLINT
                processInstruction(op);

                //The program counter shouldn't jump with the second instrution
                EXPECT_EQ(std::distance(program_memory_, program_counter_), 2); // NOLINT
            }

            /**
            * Test OpCode 4XNN 
            * Skip the following instruction if the value of register VX is not equal to NN
            */
            void TestOpCode_4XNN()
            {
                program_counter_ = program_memory_; // NOLINT

                data_registers_[0].Set(10);
                data_registers_[1].Set(11);

                chip8::opcodes::OpBytes op {0x41, 11}; // NOLINT
                processInstruction(op);

                //The program counter should not have jumped the next instruction
                EXPECT_EQ(std::distance(program_memory_, program_counter_), 0); // NOLINT

                op = {0x40, 11}; // NOLINT
                processInstruction(op);

                //The program counter should have jumped with the second instrution
                EXPECT_EQ(std::distance(program_memory_, program_counter_), 2); // NOLINT
            }

            /**
            * OpCode DXYN 
            * Draw a sprite at position VX, VY with N bytes of sprite data starting at the address 
            * stored in I. Set VF to 1 if any set pixels are changed to unset, and 0 otherwise.
            */
            void TestOpCode_DXYN() {
                chip8::io::display::Renderer displayRenderer(pixels_);
                using namespace std::literals::chrono_literals;

                chip8::opcodes::OpBytes op_00E0{0x00, 0xE0}; // NOLINT
                processInstruction(op_00E0);

                i_register_.Set(5*0); // NOLINT

                chip8::opcodes::OpBytes op_D005{0xD0, 0x05}; // NOLINT
                processInstruction(op_D005);

                EXPECT_EQ(data_registers_[0xF].Get(), 0); // NOLINT

                displayRenderer.Update();
                std::this_thread::sleep_for(0.2s);

                i_register_.Set(5*1); // NOLINT
                processInstruction(op_D005);

                EXPECT_EQ(data_registers_[0xF].Get(), 1); // NOLINT

                displayRenderer.Update();
                std::this_thread::sleep_for(0.2s);

                i_register_.Set(5*2); // NOLINT
                processInstruction(op_D005);

                EXPECT_EQ(data_registers_[0xF].Get(), 1); // NOLINT

                displayRenderer.Update();
                std::this_thread::sleep_for(0.2s);

                i_register_.Set(5*3); // NOLINT
                processInstruction(op_D005);

                displayRenderer.Update();
                std::this_thread::sleep_for(0.2s);

            }

            /**
            * OpCode ANNN 
            * Store memory address NNN in register I
            */
            void TestOpCode_ANNN() {
                chip8::opcodes::OpBytes op_A123{0xA1, 0x23}; // NOLINT
                processInstruction(op_A123);
                EXPECT_EQ(i_register_.Get(), 0x123);

                chip8::opcodes::OpBytes op_A234{0xA2, 0x34}; // NOLINT
                processInstruction(op_A234);
                EXPECT_EQ(i_register_.Get(), 0x234);

            }

    };

    TEST(Chip8TestSuite, OpCodes)
    {
        chip8::test::Interpreter interpreterTests{};
        
        interpreterTests.TestOpCode_0NNN();
        interpreterTests.TestOpCode_00E0();
        interpreterTests.TestOpCode_00EE();
        interpreterTests.TestOpCode_1NNN();
        interpreterTests.TestOpCode_2NNN();
        interpreterTests.TestOpCode_3XNN();
        interpreterTests.TestOpCode_4XNN();
        interpreterTests.TestOpCode_DXYN();
        interpreterTests.TestOpCode_ANNN();
    }
}