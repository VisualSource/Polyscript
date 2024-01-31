#pragma once
#include <vector>
// https://llvm.org/docs/tutorial/MyFirstLanguageFrontend/LangImpl02.html
class Node
{
private:
    unsigned int type;

public:
    Node();

    virtual ~Node() = default;
};

class VariableNode : public Node
{
private:
    std::string name;

public:
    VariableNode(const std::string &name) : name(name) {}
};
class NumberNode : public Node
{
private:
    double val;

public:
    NumberNode(double val) : val(val) {}
};
class StringNode : public Node
{
private:
    std::string value;

public:
    StringNode(const std::string &value) : value(value) {}
};
class BinaryNode : public Node
{
private:
    char op;
    Node *lhs;
    Node *rhs;

public:
    BinaryNode(char op, Node *lhs, Node *rhs) : op(op), lhs(lhs), rhs(rhs) {}
    ~BinaryNode()
    {
        delete lhs;
        delete rhs;
    }
};
class CallNode : public Node
{
private:
    std::string name;
    std::vector<Node *> args;

public:
    CallNode(const std::string &name, std::vector<Node *> args) : name(name), args(args) {}
    ~CallNode()
    {

        for (auto &&i : args)
        {
            delete i;
        }
    }
};
