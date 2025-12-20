#ifndef  LIVIDFUNCTION_H
#define  LIVIDFUNCTION_H

#include "interpreter/Interpreter.h"
#include "fun/Callable.h"
#include "ast/Stmt.h"

#include <any>
#include <vector>

class LividFunction:Callable{
    private:   
        std::shared_ptr<Function> declaration;
        
    public:
        LividFunction(std::shared_ptr<Function> declaration){this->declaration=declaration;}
        std::any call(Interpreter& interpreter,std::vector<std::any> arguements)override;
        int arity()override;
        std::string toString()override;

};
#endif