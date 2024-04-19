#include <vip/jit/jit.hpp>
#include <vip/vip.hpp>

namespace jit
{

    std::shared_ptr<Object> JustInTime::execute(std::string input)
    {
        ast::Program program = vip::tokenize(input);
        return rt.execute(program, cliMode);
    }

    void JustInTime::registerFn(std::string name, jit::CallbackFunction callback)
    {
        auto fn = std::shared_ptr<InternalFunction>(new InternalFunction(name, callback));

        rt.declare(name, fn);
    }

}