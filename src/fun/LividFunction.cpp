#include "fun/LividFunction.h"
#include "fun/Return.h"
#include "environment/Environment.h"
#include "interpreter/Interpreter.h"
#include "class/LividInstance.h"
#include "ast/Stmt.h"

std::any LividFunction::call(Interpreter& interpreter,std::vector<std::any> arguements){
    auto environment=std::make_shared<Environment>(closure);
    for(int i=0;i<declaration->params.size();i++){
        environment->define(declaration->params[i].getLexeme(),arguements[i]);
    }

    try{
        interpreter.executeBlock(declaration->body,environment);
    }catch(const ReturnException& returnvalue){
        if(isInitializer) return closure->getAt(0,"this");
        return returnvalue.value;
    }
    if(isInitializer) return closure->getAt(0,"this");
    return std::any{};
}
int LividFunction::arity(){
    return declaration->params.size();
}
std::string LividFunction::toString(){
    return "<fn "+declaration->name.getLexeme()+">";
}
std::shared_ptr<LividFunction> LividFunction::bind(std::shared_ptr<LividInstance> instance){
    auto environment =std::make_shared<Environment>(closure);
    environment->define("this",instance);
    return std::make_shared<LividFunction>(declaration,environment,isInitializer);
}