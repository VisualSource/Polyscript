#include "BuiltInFunction.h"
#include <iostream>
#include <variant>
#include "Context.h"
#include "SymbolTable.h"
#include "String.h"
#include "RuntimeError.h"
#include "InterpreterUtils.h"
#include "Integer.h"
#include "fetch.h"
#include "Object.h"
#include "String.h"

using namespace std;

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
                cout << *v << endl;
                return Null();
            }
            auto* s = std::get_if<String>(&value);
            if (s != nullptr) {
                cout << *s << endl;;
                return Null();
            }
            auto* f = std::get_if<Float>(&value);
            if (f != nullptr) {
                cout << *f << endl;;
                return Null();
            }
            auto* fn = std::get_if<Function>(&value);
            if (fn != nullptr) {
                cout << *fn << endl;;
                return Null();
            }
            auto* l = std::get_if<List>(&value);
            if (l != nullptr) {
                cout << *l << endl;;
                return Null();
            }
            auto* e = std::get_if<EnumValue>(&value);
            if (e != nullptr) {
                cout << *e << endl;;
                return Null();
            }
            auto* isNull = std::get_if<Null>(&value);
            if (isNull != nullptr) {
                cout << *isNull << endl;;
                return Null();
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
    else if (func == "isEnum") {
        Integer out(0);
        out.SetContext(context).PrintBool(true);
        ScopeTypes::Var value = ctx->GetScope()->get("__input", ctx);
        auto* v = std::get_if<EnumValue>(&value);
        if (v != nullptr) {
            return !out;
        }
        else {
            return out;
        }
    }
    else if (func == "isNull") {
        Integer out(0);
        out.SetContext(context).PrintBool(true);
        ScopeTypes::Var value = ctx->GetScope()->get("__input", ctx);
        auto* v = std::get_if<Null>(&value);
        if (v != nullptr) {
            return !out;
        }
        else {
            return out;
        }
    }
    else if (func == "length") {  
        try {
            ScopeTypes::Var value = ctx->GetScope()->get("__input", ctx);
            auto* islist = get_if<List>(&value);
            if (islist != nullptr) {
                return Integer((*islist).GetElements().size());
            }
            auto* isString = get_if<String>(&value);
            if (isString != nullptr) {
                return Integer((*isString).GetValue().length());
            }
            return Null();
        }
        catch (bad_variant_access const&) {
            throw RuntimeError("Expected a list", ctx, start, end);
        }
    }
    else if (func == "syncFetch") {
        String url = get<String>(ctx->GetScope()->get("__url", ctx));

        httplib::Result res = fetch::Get(url.GetValue(),ctx);

        vector<ObjectProperties> props;
        props.push_back(ObjectProperties {
            "body",
            String(res->body)
        });
        props.push_back(ObjectProperties{
          "version",
          String(res->version)
        });
        props.push_back(ObjectProperties{
          "status",
          Integer(res->status)
        });
    
        return Object(props, ctx);
    }
    return Null();
}


