#include <iostream>
#include <fstream>
#include "./astToCil.hpp"
#include "../ast/FunctionDeclaration.hpp"
#include "../ast/ExpressionStatement.hpp"
#include "../ast/CallExpression.hpp"
#include "../ast/StringLiteral.hpp"

namespace compile
{
    std::vector<std::string> astToCil(ast::Program &program, std::string dir, std::string output)
    {
        std::cout << program.toString() << std::endl;

        std::vector<std::string> files;
        std::string dir = dir + output + program.getName() + ".il";

        files.push_back(dir);

        std::cout << "Write: " << dir << std::endl;
        std::ofstream file;
        file.open(dir, std::ios::trunc | std::ios::out | std::ios::app);
        if (!file)
        {
            std::cout << "Cannot open file, file does not exist. Creating new file.." << std::endl;
            file.open(dir, std::ios::in | std::ios::out | std::ios::trunc);
        }

        file << ".assembly VipProgram {}\n";
        file << ".assembly extern mscorlib {}\n";
        // wrapping class.
        file << " .class private auto ansi beforefieldinit __APP__ extends [mscorlib]System.Object{\n";
        file << "    .method public hidebysig static void Main() cil managed {\n";
        file << "      .entrypoint\n";
        file << "      .maxstack 8\n";
        file << "      call void __APP__::main()\n";
        file << "      ret\n";
        file << "    }";

        // class constructor.
        file << ".method public hidebysig specialname rtspecialname instance void .ctor() cil managed {\n";
        file << ".maxstack 8\n";
        file << "ldarg.0\n"; // ref to this in the class
        file << "call instance void [mscorlib]System.Object::.ctor()\n";
        file << "ret\n}";

        for (auto &&i : program.getStatements())
        {
            if (auto *fd = dynamic_cast<ast::FunctionDeclartion *>(i); fd != nullptr)
            {
                file << ".method public hidebysig static void " + fd->getName() + "(" + ") cil managed {\n";
                file << ".maxstack 8\n";

                for (auto &&body : fd->getBody())
                {
                    if (auto *es = dynamic_cast<ast::ExpressionStatement *>(body); es != nullptr)
                    {
                        if (auto *ce = dynamic_cast<ast::CallExpression *>(es->getExpression()); ce != nullptr)
                        {
                            if (auto *name = dynamic_cast<ast::Identifier *>(ce->getExpression()); name != nullptr && name->getValue() == "print")
                            {
                                if (ast::StringLiteral *value = dynamic_cast<ast::StringLiteral *>(ce->getArguments().at(0)); value != nullptr)
                                {
                                    file << "ldstr \"" + value->getValue() + "\"\n";
                                    file << "call void [mscorlib]System.Console::WriteLine(string)\n";
                                }
                            }
                        }
                    }
                }

                file << "ret\n";
                file << "}\n";
            }

            file << "}";
        }

        file.close();

        return files;
    }
} // namespace compile
