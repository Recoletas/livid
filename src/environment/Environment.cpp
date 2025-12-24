#include "environment/Environment.h"
#include "core/RuntimeError.h"

void Environment::define(const std::string& name,const std::any& value){
    values[name]=value;
}
std::any Environment::getAt(int distance,std::string name){
    auto env=ancestor(distance);
    return env->values.at(name);
}
void Environment::assignAt(int distance, Token name, std::any value) {
    ancestor(distance)->values[name.getLexeme()]= value;
}
std::shared_ptr<Environment> Environment::ancestor(int distance){
    std::shared_ptr<Environment> environment=shared_from_this();
    for(int i=0;i<distance;i++){
        environment=environment->enclosing;
    }
    return environment;
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