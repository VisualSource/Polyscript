#pragma once

namespace x16_chip8_unknown
{
    class X16ChipUnknown
    {

        void handleVariableDeclaration();
        void handleFunctionDeclartion();
        void handleExpression();
    };

    /**
     * EXTERN FUNCTION
     *
     * clear_screen() -> void; = 00E0
     * write(int,int, value) -> void; = DXYN
     * rand(int) -> int; = CXNN
     *
     * key_pressed(key) -> bool
     * key_unpressed(key) -> bool
     *
     * wait_for_key(key) -> int
     *
     * set_timeout(func,ms) -> void
     *
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