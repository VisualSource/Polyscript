#pragma once
#include <unordered_map>
#include <vector>
#include <cstdint>
#include <vip/compiler/targets/target.hpp>

namespace compiler
{
    struct Function
    {
        // "module::function_name"
        std::string name;
        std::vector<uint8_t> body;
    };

    class X16ChipUnknown : Target
    {
    private:
        std::vector<Function> function_table;
        std::vector<uint8_t> body_table;

    public:
        X16ChipUnknown() : Target("chip8-unknown-unknown") {}

        void visitEntryPoint(std::vector<ast::Node *> &statements, Compiler *ctx) override;
        void visitStatements(std::vector<ast::Node *> &statements, Compiler *ctx, std::string scope) override;
        void visitStatement(ast::Node *statement, Compiler *context, std::string scope) override;
        void visitVariableStatement(ast::VariableStatement *value, Compiler *context, std::string scope) override;
        void visitFunctionDeclartion(ast::FunctionDeclartion *value, Compiler *context, std::string scope) override;
        void visitVariableDeclaration(ast::VariableDeclaration *value, Compiler *context, std::string scope) override;
        void visitExpression(ast::Node *value, Compiler *context, std::string scope) override;
    };

  

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