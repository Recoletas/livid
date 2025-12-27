#ifndef LIVIDCLASS_H
#define LIVIDCLASS_H

#include <vector>
#include <string>
#include "fun/Callable.h"
#include "fun/LividFunction.h"

class LividClass:public Callable,
                 public std::enable_shared_from_this<LividClass>{
    public:
        std::string name;
        LividClass(std::string name):name(name){};
        LividClass(std::string name,std::unordered_map<std::string,std::shared_ptr<LividFunction>> methods):name(name),methods(methods){};
        std::string toString()override;
        std::any call(Interpreter &interpreter,std::vector<std::any> arguments);
        int arity()override=0;
        std::shared_ptr<LividFunction> findMethod(std::string name);
    private:
        std::unordered_map<std::string,std::shared_ptr<LividFunction>>methods;

};

#endif