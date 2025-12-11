#include "Environment.h"
#include "RuntimeError.h"

void Environment::define(const std::string& name,std::any& value){
    values[name]=value;
}
std::any Environment::get(const Token& name){
    const std::string& key =name.getLexeme();

    if(values.count(key)){
        return values.at(key);
    }

    throw RuntimeError(name,"Undefined variable '"+name.getLexeme()+"'.");
}