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

static any run(Context* ctx, const string& name, Position start, Position end){
    if (name == "println" || name == "print") {
        bool ln = name == "println";
        try {
            using namespace InterTypes;
            ScopeTypes::Var value = ctx->GetScope()->get("__input", ctx);
            auto* v = std::get_if<Integer>(&value);
            if (v != nullptr) {
                cout << *v;
                if (ln) cout << endl;
                return Null();
            }
            auto* s = std::get_if<String>(&value);
            if (s != nullptr) {
                cout << *s;
                if (ln) cout << endl;
                return Null();
            }
            auto* f = std::get_if<Float>(&value);
            if (f != nullptr) {
                cout << *f;
                if (ln) cout << endl;
                return Null();
            }
            auto* fn = std::get_if<Function>(&value);
            if (fn != nullptr) {
                cout << *fn;
                if (ln) cout << endl;
                return Null();
            }
            auto* l = std::get_if<List>(&value);
            if (l != nullptr) {
                cout << *l;
                if (ln) cout << endl;
                return Null();
            }
            auto* e = std::get_if<EnumValue>(&value);
            if (e != nullptr) {
                cout << *e;
                if (ln) cout << endl;
                return Null();
            }
            auto* isNull = std::get_if<Null>(&value);
            if (isNull != nullptr) {
                cout << *isNull;
                if (ln) cout << endl;
                return Null();
            }

        }
        catch (bad_variant_access const&) {
            throw RuntimeError("Invaild input", ctx, start, end);
        }
    }
    else if (name == "length") {
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
            throw RuntimeError("Expected a list or string", ctx, start, end);
        }
    }
    else if (name == "fetch") {
        String url = get<String>(ctx->GetScope()->get("__url", ctx));

        httplib::Result res = fetch::Get(url.GetValue(), ctx);

        vector<ObjectProperties> props;
        props.push_back(ObjectProperties{
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
    else if (name == "readFile") {
        try{
            String input = get<String>(ctx->GetScope()->get("__input", ctx));
            
            std::ifstream reader(input.GetValue());
            
            if (!reader.is_open()) {
                throw runtime_error("Failed to open file");
            }
       
            string text = string((std::istreambuf_iterator<char>(reader)), (std::istreambuf_iterator<char>()));
            reader.close();

            return String(text).SetContext(ctx->GetContextParent());
           
        } catch (const std::bad_variant_access&) {
            throw RuntimeError("Expected a string", ctx, start, end);
        }
    }
    else if (name == "clear") {
        cout << "\x1B[2J\x1B[H";
    }
    else if (name == "isFloat") {
        Integer out(0);
        out.SetContext(ctx->GetContextParent()).PrintBool(true);
        ScopeTypes::Var value = ctx->GetScope()->get("__input", ctx);
        auto* v = std::get_if<Float>(&value);
        if (v != nullptr) {
            return !out;
        }
        return out;
    }
    else if (name == "isInteger") {
        Integer out(0);
        out.SetContext(ctx->GetContextParent()).PrintBool(true);
        ScopeTypes::Var value = ctx->GetScope()->get("__input", ctx);
        auto* v = std::get_if<Integer>(&value);
        if (v != nullptr) {
            return !out;
        }

        return out;

    }
    else if (name == "isFunction") {
        Integer out(0);
        out.SetContext(ctx->GetContextParent()).PrintBool(true);
        ScopeTypes::Var value = ctx->GetScope()->get("__input", ctx);
        auto* v = std::get_if<Function>(&value);
        if (v != nullptr) {
            return !out;
        }

        return out;

    }
    else if (name == "isList") {
        Integer out(0);
        out.SetContext(ctx->GetContextParent()).PrintBool(true);
        ScopeTypes::Var value = ctx->GetScope()->get("__input", ctx);
        auto* v = std::get_if<List>(&value);
        if (v != nullptr) {
            return !out;
        }
        return out;
    }
    else if (name == "isString") {
        Integer out(0);
        out.SetContext(ctx->GetContextParent()).PrintBool(true);
        ScopeTypes::Var value = ctx->GetScope()->get("__input", ctx);
        auto* v = std::get_if<String>(&value);
        if (v != nullptr) {
            return !out;
        }
        else {
            return out;
        }
    }
    else if (name == "isEnum") {
        Integer out(0);
        out.SetContext(ctx->GetContextParent()).PrintBool(true);
        ScopeTypes::Var value = ctx->GetScope()->get("__input", ctx);
        auto* v = std::get_if<EnumValue>(&value);
        if (v != nullptr) {
            return !out;
        }
        else {
            return out;
        }
    }
    else if (name == "isNull") {
        Integer out(0);
        out.SetContext(ctx->GetContextParent()).PrintBool(true);
        ScopeTypes::Var value = ctx->GetScope()->get("__input", ctx);
        auto* v = std::get_if<Null>(&value);
        if (v != nullptr) {
            return !out;
        }
        else {
            return out;
        }
    }
  
    return Null();
}

any BuiltInFunction::exceute(vector<any> args)
{
    Context* ctx = NewContext();

    check_and_populate(this->args_names,args,ctx);

    any out = run(ctx, GetName(), this->start, this->end);

    delete ctx;
       
    return out;
}


