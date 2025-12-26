#ifndef LIVIDCLASS_H
#define LIVIDCLASS_H

#include <vector>
#include <string>
#include "fun/Callable.h"

class LividClass:public Callable{
    public:
        std::string name;
        LividClass(std::string name):name(name){};
        std::string toString()override;
        std::any call(Interpreter interpreter,std::vector<std::any> arguments);
        int arity()override=0;

};

#endif