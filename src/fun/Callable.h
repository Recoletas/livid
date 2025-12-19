#ifndef  CALLABLE_H
#define  CALLABLE_H
#include <any>
#include <vector>
#include "interpreter/Interpreter.h"

class Callable{
    public:
        virtual ~Callable()=default;
        virtual std::any call(Interpreter& interpreter,std::vector<std::any> arguments);
        virtual int arity()=0;
        virtual std::string toString()=0;
};
#endif