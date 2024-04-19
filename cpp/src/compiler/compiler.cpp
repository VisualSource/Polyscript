#include <vip/compiler/compiler.hpp>
#include <vip/utils/load_file.hpp>
#include <vip/ast/Program.hpp>
#include <vip/vip.hpp>
#include <stdexcept>
namespace compiler
{
    Compiler::~Compiler()
    {
        delete this->target;
        if (this->file.is_open())
            this->file.close();
    }

    // compile ast into target
    void Compiler::compile()
    {
        // check if output is open
        if (!this->file.is_open())
            throw std::logic_error("Failed to open output file");

        // read entrypoint file
        std::string content;
        vip::utils::load_file(this->entrypoint, content);

        // tokenize
        ast::Program program = vip::tokenize(content);

        auto statements = program.getStatements();

        this->target->visitEntryPoint(statements, this);
    }
}