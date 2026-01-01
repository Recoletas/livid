#include "class/LividInstance.h"
#include "class/LividClass.h"
#include "fun/LividFunction.h"
#include "core/RuntimeError.h"

std::any LividInstance::get(const Token& name){
    if(fields.count(name.getLexeme())){
        return fields[name.getLexeme()];
    }
    auto method =klass->findMethod(name.getLexeme());
    if(method!=nullptr){
        auto bound = method->bind(shared_from_this());
        return std::static_pointer_cast<Callable>(bound);
    } 
    throw RuntimeError(name,"Undefined property '"+name.getLexeme()+"'.");
}


void LividInstance::set(const Token& name,std::any value){
    fields[name.getLexeme()]=value;
}
std::string LividInstance::toString() {
    return klass->name + " instance";
}