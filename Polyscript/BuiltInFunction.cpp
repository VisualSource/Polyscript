#include "BuiltInFunction.h"
#include <iostream>
#include <variant>
#include "Context.h"
#include "SymbolTable.h"
#include "String.h"
#include "RuntimeError.h"
#include "InterpreterUtils.h"
#include "Integer.h"

BuiltInFunction::BuiltInFunction(string name, vector<string> args_names): BaseFunction(name), args_names(args_names)
{
}

any BuiltInFunction::exceute(vector<any> args)
{
    Context* ctx = NewContext();

    check_and_populate(this->args_names,args,ctx);
        
    string func = this->GetName();


    if (func == "print") {
        try{
            using namespace InterTypes;
            ScopeTypes::Var value = ctx->GetScope()->get("__input",ctx);
            auto* v = std::get_if<Integer>(&value);
            if (v != nullptr) {
                cout << get<Integer>(value) << endl;
                return any();
            }
            auto* s = std::get_if<String>(&value);
            if (s != nullptr) {
                cout << get<String>(value) << endl;;
                return any();
            }
            auto* f = std::get_if<Float>(&value);
            if (f != nullptr) {
                cout << get<Float>(value) << endl;;
                return any();
            }
            auto* fn = std::get_if<Function>(&value);
            if (fn != nullptr) {
                cout << get<Function>(value) << endl;;
                return any();
            }
            auto* l = std::get_if<List>(&value);
            if (l != nullptr) {
                cout << get<List>(value) << endl;;
                return any();
            }
            
        } catch (bad_variant_access const&){
            throw RuntimeError("Invaild input",ctx,start,end);
        }
    }
    else if (func == "clear") {
        cout << "\x1B[2J\x1B[H";
    }
    else if (func == "isFloat") {
        Integer out(0);
        out.SetContext(context).PrintBool(true);
        ScopeTypes::Var value = ctx->GetScope()->get("__input",ctx);
        auto* v = std::get_if<Float>(&value);
        if (v != nullptr) {
            return !out;
        }
       return out;
    }
    else if (func == "isInteger") {
        Integer out(0);
        out.SetContext(context).PrintBool(true);
        ScopeTypes::Var value = ctx->GetScope()->get("__input", ctx);
        auto* v = std::get_if<Integer>(&value);
        if (v != nullptr) {
            return !out;
        }
       
       return out;
        
    }
    else if(func == "isFunction"){
        Integer out(0);
        out.SetContext(context).PrintBool(true);
        ScopeTypes::Var value = ctx->GetScope()->get("__input", ctx);
        auto* v = std::get_if<Function>(&value);
        if (v != nullptr) {
            return !out;
        }
       
       return out;
        
    }
    else if (func == "isList") {
        Integer out(0);
        out.SetContext(context).PrintBool(true);
        ScopeTypes::Var value = ctx->GetScope()->get("__input", ctx);
        auto* v = std::get_if<List>(&value);
        if (v != nullptr) {
            return !out;
        }
       return out;
    }
    else if (func == "isString") {
        Integer out(0);
        out.SetContext(context).PrintBool(true);
        ScopeTypes::Var value = ctx->GetScope()->get("__input", ctx);
        auto* v = std::get_if<String>(&value);
        if (v != nullptr) {
            return !out;
        }
        else {
            return out;
        }
    }
    else if (func == "length") {  
        try {
            List len = get<List>(ctx->GetScope()->get("__input", ctx));
            return Integer(len.GetElements().size());
        }
        catch (bad_variant_access const&) {
            throw RuntimeError("Expected a list", ctx, start, end);
        }
    }
    return any();
}
/*
auto* typeA = std::get_if<Integer>(&value);
    if (typeA != nullptr) {
        return *typeA;
    }

    auto* typeString = get_if<String>(&value);
    if (typeString != nullptr) {
        return *typeString;
    }

    auto* typeB = std::get_if<Float>(&value);
    if (typeB != nullptr) {
        return *typeB;
    }

    auto* func = std::get_if<Function>(&value);
    if (func != nullptr) {
        return *func;
    }

    auto* list = get_if<List>(&value);
    if (list != nullptr) {
        return *list;
    }

    auto* builtin = get_if<BuiltInFunction>(&value);
    if (builtin != nullptr) {
        return *builtin;
    }

    throw RuntimeError("Failed to access variable", context,node.GetStart(),node.GetEnd());


*/
