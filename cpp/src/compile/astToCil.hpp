#pragma once
#include "../ast/Program.hpp"

/* Vip: file./Hello.vip

   fn main(){
      println("Hello, World!");
   }

*/

/* C# 8: file./Hello.cs
   using System;

   class __APP__
   {
      static void Main()
      {
         main();
      }

      public static main(){
         Console.WriteLine("Hello, World!");
      }
   }
*/

/* CIL: file./Hello.il

.assembly Hello {}
.assembly extern mscorlib {}

.method static void Main()
{
   .entrypoint
   .maxstack 1

   ldstr "Hello, World!"
   call void [mscorlib]System.Console::WriteLine(string)
   ret
}
*/

namespace compile
{
   /// @brief translate ast into cil
   /// @param program
   /// @param dir
   /// @param output
   /// @return
   std::vector<std::string> astToCil(ast::Program &program, std::string dir, std::string output);
} // namespace compile
