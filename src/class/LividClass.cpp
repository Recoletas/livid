#include "class/LividClass.h"
#include "class/LividInstance.h"


std::any LividClass::call(Interpreter& interpreter,std::vector<std::any> arguments){
    auto instance=std::make_shared<LividInstance>(shared_from_this());
    return instance;
}