#ifndef  LIVIDFUNCTION_H
#define  LIVIDFUNCTION_H

#include "interpreter/Interpreter.h"
#include "class/LividInstance.h"
#include "fun/Callable.h"
#include "ast/Stmt.h"

#include <any>
#include <vector>

class LividFunction:public Callable{
    private:   
        std::shared_ptr<Function> declaration;
        std::shared_ptr<Environment> closure; // 存储闭包
        
    public:
        LividFunction(std::shared_ptr<Function> declaration, std::shared_ptr<Environment> closure)
        {this->declaration=declaration;this->closure=closure;}
        std::shared_ptr<LividFunction> bind(std::shared_ptr<LividInstance> instance);
        std::any call(Interpreter& interpreter,std::vector<std::any> arguements)override;
        int arity()override;
        std::string toString()override;

};
#endif