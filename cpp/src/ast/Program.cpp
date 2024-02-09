#include <vip/ast/Program.hpp>

namespace ast
{
    Program::Program()
    {
        statements = std::vector<Node *>();
        name = "Program";
    }
    Program::~Program()
    {
        if (statements.empty())
            return;
        for (auto &&i : statements)
            if (i != nullptr)
                delete i;
    }
    void Program::setStatements(std::vector<Node *> statements)
    {
        this->statements = statements;
    }

    std::vector<Node *> &Program::getStatements() const
    {
        return const_cast<std::vector<Node *> &>(statements);
    }

    std::string Program::toString()
    {
        auto tag = std::string("<Program>\n");

        for (auto &&i : statements)
        {
            tag += i->toString(3);
        }

        tag += std::string("</Program>");

        return tag;
    }
}
