#include "class/LividClass.h"
#include "class/LividInstance.h"
#include "interpreter/Interpreter.h"
#include "fun/LividFunction.h"

std::any LividClass::call(Interpreter& interpreter,std::vector<std::any> arguments){
    auto instance=std::make_shared<LividInstance>(shared_from_this());
    std::shared_ptr<LividFunction> initializer=findMethod("init");
    if(initializer !=nullptr){
        initializer->bind(instance)->call(interpreter,arguments);
    }
    return instance;
}
std::shared_ptr<LividFunction> LividClass::findMethod(std::string name){
    if(methods.count(name)){
        return methods.at(name);
    }
    if(superclass!=nullptr){
        return superclass->findMethod(name);
    }
    return nullptr;
}
int LividClass::arity(){
    std::shared_ptr<LividFunction> initializer=findMethod("init");
    if(initializer==nullptr) return 0;
    return initializer->arity();
}
std::string LividClass::toString() {
    return name;
}