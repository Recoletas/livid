#ifndef  LIVIDFUNCTION_H
#define  LIVIDFUNCTION_H

#include <any>
#include <vector>
#include <memory>
#include "fun/Callable.h"

class Function;    
class Environment; 
class LividInstance;

class LividFunction:public Callable{
    private:   
        std::shared_ptr<Function> declaration;
        std::shared_ptr<Environment> closure; // 存储闭包
        bool isInitializer;
        
    public:
        LividFunction(std::shared_ptr<Function> declaration, std::shared_ptr<Environment> closure,bool isInitializer)
        {this->declaration=declaration;this->closure=closure;this->isInitializer=isInitializer;}
        std::shared_ptr<LividFunction> bind(std::shared_ptr<LividInstance> instance);
        std::any call(Interpreter& interpreter,std::vector<std::any> arguements)override;
        int arity()override;
        std::string toString()override;

};
#endif