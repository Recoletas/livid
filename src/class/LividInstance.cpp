#include "class/LividInstance.h"
#include "core/RuntimeError.h"

std::any LividInstance::get(Token name){
    if(fields.count(name.getLexeme())){
        return fields.at(name.getLexeme());
    }
    throw RuntimeError(name,"Undefined property '"+name.getLexeme()+"'.");
}
void LividInstance::set(Token name,std::any value){
    fields[name.getLexeme()]=value;
}