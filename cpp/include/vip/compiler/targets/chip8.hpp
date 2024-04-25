#pragma once
#include <unordered_map>
#include <optional>
#include <vector>
#include <cstdint>
#include <vip/compiler/targets/Chip8Object.hpp>
#include <vip/ast/FunctionDeclaration.hpp>
#include <vip/ast/ExpressionStatement.hpp>
#include <vip/ast/VariableStatement.hpp>
#include <vip/ast/IfStatement.hpp>
#include <vip/compiler/targets/target.hpp>

namespace compiler
{
    struct Scope
    {
        std::string name;
        uint16_t register_index;
        std::vector<uint16_t> body;
    };

    class X16ChipUnknown : Target
    {

        /**
         * 1
         * 2
         * 3
         * 4
         * 5
         * 6
         * 7
         * 8
         * 9
         * A
         * B == Return register
         * C == param registers
         * D
         * E
         * F
         */
        /***
         * @test
         * $512 - 7xkk (7F03) Add the value 3 to the F param register. (param register should be the last 5 registers with 6 begin return register)
         * $513 - 8xy0 (8BF0) Set value to output register
         * $514 - 00EE (RET)  Return from function
         * @main
         * $515 - 6xkk (6F04) Store argument(The value 4) into a register, would be the stack but we can interact with the stack this way with the chip8
         * $516 - 2nnn (2512) Call function test
         * $517 - 8xy0 (80B0) (Move value out of return)
         * $520 - 7xkk (7004) And value 4
         */

        /**
         *  Opcodes
            00E0 - (CLS)                   Clear the screen

            00EE - (RET)                   Return from a subroutine

            0nnn - (SYS addr)              Execute machine language subroutine at address NNN

            1nnn - (JP addr)               Jump to address NNN

            2nnn - (CALL addr)             Execute subroutine starting at address NNN

            3xkk - (SE Vx, byte)           Skip the following instruction if the value of register VX equals NN

            4xkk - (SNE Vx, byte)          Skip the following instruction if the value of register VX is not equal to NN

            5xy0 - (SE Vx, Vy)             Skip the following instruction if the value of register VX is equal to the value of register VY

            6xkk - (LD Vx, byte)           Store number NN in register VX

            7xkk - (ADD Vx, byte)          Add the value NN to register VX

            8xy0 - (LD Vx, Vy)             Store the value of register VY in register VX

            8xy1 - (OR Vx, Vy)             Set VX to VX OR VY

            8xy2 - (AND Vx, Vy)            Set VX to VX AND VY

            8xy3 - (XOR Vx, Vy)            Set VX to VX XOR VY

            8xy4 - (ADD Vx, Vy)            Add the value of register VY to register VX.
                                             Set VF to 01 if a carry occurs.
                                             Set VF to 00 if a carry does not occur

            8xy5 - (SUB Vx, Vy)            Subtract the value of register VY from register VX
                                             Set VF to 00 if a borrow occurs
                                             Set VF to 01 if a borrow does not occur

            8xy6 - (SHR Vx {, Vy})         Store the value of register VY shifted right one bit in register VX
                                             Set register VF to the least significant bit prior to the shift
                                             VY is unchanged

            8xy7 - (SUBN Vx, Vy)           Set register VX to the value of VY minus VX
                                             Set VF to 00 if a borrow occurs
                                             Set VF to 01 if a borrow does not occur

            8xyE - (SHL Vx {, Vy})         Store the value of register VY shifted left one bit in register VX
                                             Set register VF to the most significant bit prior to the shift
                                             VY is unchanged

            9xy0 - (SNE Vx, Vy)            Skip the following instruction if the value of register VX is not equal to the value of register VY

            Annn - (LD I, addr)            Store memory address NNN in register I

            Bnnn - (JP V0, addr)           Jump to address NNN + V0

            Cxkk - (RND Vx, byte)          Set VX to a random number with a mask of NN

            Dxyn - (DRW Vx, Vy, nibble)    Draw a sprite at position VX, VY with N bytes of sprite data starting at the address stored in I
                                             Set VF to 01 if any set pixels are changed to unset, and 00 otherwise

            Ex9E - (SKP Vx)                Skip the following instruction if the key corresponding to the hex value currently stored in register VX is pressed

            ExA1 - (SKNP Vx)               Skip the following instruction if the key corresponding to the hex value currently stored in register VX is not pressed

            Fx07 - (LD Vx, DT)             Store the current value of the delay timer in register VX

            Fx0A - (LD Vx, K)              Wait for a keypress and store the result in register VX

            Fx15 - (LD DT, Vx)             Set the delay timer to the value of register VX

            Fx18 - (LD ST, Vx)             Set the sound timer to the value of register VX

            Fx1E - (ADD I, Vx)             Add the value stored in register VX to register I

            Fx29 - (LD F, Vx)              Set I to the memory address of the sprite data corresponding to the hexadecimal digit stored in register VX

            Fx33 - (LD B, Vx)              Store the binary-coded decimal equivalent of the value stored in register VX at addresses I, I + 1, and I + 2

            Fx55 - (LD [I], Vx)            Store the values of registers V0 to VX inclusive in memory starting at address I
                                              I is set to I + X + 1 after operation

            Fx65 - (LD Vx, [I])            Fill registers V0 to VX inclusive with the values stored in memory starting at address I
                                              I is set to I + X + 1 after operation

            Opcode Extentions

            5XY2 - (SE VX-VY) save an inclusive range of registers to memory starting at i.

            5XY3 - (LD VX-VY) load an inclusive range of registers from memory starting at i.

            Fx75 - (SE VX-VY) save v0-vn to flag registers.

            Fx85 - (LD Vx-VY) restore v0-vn from flag registers.

        */
    private:
        std::unordered_map<std::string, Scope> function_table;

    public:
        X16ChipUnknown() : Target("chip8-unknown-unknown") {}

        void visitEntryPoint(std::vector<ast::Node *> &statements, Compiler *ctx);
        void visitStatements(std::vector<ast::Node *> &statements, Compiler *ctx, Scope &scope);
        void visitStatement(ast::Node *statement, Compiler *context, Scope &scope);
        void visitIfStatement(ast::IfStatement *value, Compiler *context, Scope &scope);
        void visitVariableStatement(ast::VariableStatement *value, Compiler *context, Scope &scope);
        void visitFunctionDeclartion(ast::FunctionDeclartion *value, Compiler *context, Scope &scope);
        void visitVariableDeclaration(ast::VariableDeclaration *value, Compiler *context, Scope &scope);
        std::optional<compiler::Chip8Object> visitExpression(ast::Node *value, Compiler *context, Scope &scope);
    };

    /***
     * Numeric Literal => 6xkk LD
     * Identifier      => 5XY3
     *
     *
     */

    /***
     *
     *  2 * 2
     *
     *  6xkk LD 2
     *  6xkk LD 2
     *  8xy3 2^2
     *
     */

    /***
     *  ```
     *  let var: u8 = 1;
     *  var = 4 + 5;
     *  ```
     *  0x5XY2 SAVE RANGE
     *
     *  6XNN LD Vx, byte
     *  7XNN ADD
     *  8XY0 VY STORE VX
     */

    /**
     *  ===== FUNCTION DEF ======
     *
     * ==== VIP REPR =====
     *  fn test(){
     *
     *  }
     *
     *  ==== CHIP8 Opcodes =====
     *
     *  RET 00EE
     *
     */

    /**
     * ==== LOOP =====
     *
     * ==== VIP REPR ====
     * loop { }
     *
     * while true {}
     *
     * === CHIP8 ===
     *
     *  // Stuff
     * JMP 1NNN
     *
     *
     * 3XNN | 4XNN | 5XY0
     * JMP 1NNN
     *  // STUF
     * JMP 1NNN
     *
     *
     *
     */

    /**
     *  IF
     *
     *  if expr {}
     *
     *  3XNN | 4XNN | 5XY0
     *  1NNN
     *
     *
     */

    /**
     *  VAR ASSIGN
     *
     *  let a = 0;
     *
     *  6XNN: 10
     *
     */

}