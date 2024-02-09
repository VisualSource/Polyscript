#include <iostream>
#include <fstream>
/**
 * .class public Foo {
 *      .method private static int32 Add(int32, int32) cil managed {
 *          .maxstack 2
 *          ldrag.0
 *          ldarg.1
 *          add
 *          ret
 *      }
 * }
 */
// https://www.codeproject.com/articles/362076/understanding-common-intermediate-language-cil
namespace compile
{
    /* std::vector<std::string> astToCil(ast::Program &program, std::string dir, std::string output)
     {
         std::cout << program.toString() << std::endl;

         std::vector<std::string> files;
         std::string fdir = dir + output + program.getName() + ".il";

         files.push_back(dir);

         std::cout << "Write: " << dir << std::endl;
         std::ofstream file;
         file.open(dir, std::ios::trunc | std::ios::out | std::ios::app);
         if (!file)
         {
             std::cout << "Cannot open file, file does not exist. Creating new file.." << std::endl;
             file.open(dir, std::ios::in | std::ios::out | std::ios::trunc);
         }

         file << ".assembly VipProgram {}\n";      // Seems to be required, even if dotPeek does not show this.
         file << ".assembly extern mscorlib {}\n"; // .Net core lib import

         // top level class wrapper
         file << " .class private auto ansi beforefieldinit __APP__ extends [mscorlib]System.Object{\n";
         file << "    .method public hidebysig static void Main() cil managed {\n";
         file << "      .entrypoint\n";
         file << "      .maxstack 8\n";
         file << "      call void __APP__::main()\n";
         file << "      ret\n";
         file << "    }";

         // top level class wrapper constructor function.
         file << ".method public hidebysig specialname rtspecialname instance void .ctor() cil managed {\n";
         file << ".maxstack 8\n";
         file << "ldarg.0\n";                                             // ref to this in the class
         file << "call instance void [mscorlib]System.Object::.ctor()\n"; // calling object constructor
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
     }*/
} // namespace compile
