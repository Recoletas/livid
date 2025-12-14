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

    if(enclosing!=nullptr) return enclosing->get(name);

    throw RuntimeError(name,"Undefined variable '"+name.getLexeme()+"'.");
}
void Environment::assign(const Token& name,const std::any& value){
    if(values.count(name.getLexeme())){
        values[name.getLexeme()]=value;
        return ;
    }
    if(enclosing!=nullptr){
        enclosing->assign(name,value);
        return;
    }
    throw RuntimeError(name,"Undefined variable assign '"+name.getLexeme()+"'.");
}