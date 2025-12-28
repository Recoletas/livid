#include "class/LividInstance.h"
#include "class/LividClass.h"
#include "fun/LividFunction.h"
#include "core/RuntimeError.h"

std::any LividInstance::get(const Token& name){
    if(fields.count(name.getLexeme())){
        return fields.at(name.getLexeme());
    }
    std::shared_ptr<LividFunction> method =klass->findMethod(name.getLexeme());
    if(method!=nullptr) return method->bind(shared_from_this());
    throw RuntimeError(name,"Undefined property '"+name.getLexeme()+"'.");
}


void LividInstance::set(const Token& name,std::any value){
    fields[name.getLexeme()]=value;
}