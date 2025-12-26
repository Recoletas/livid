#include "class/LividClass.h"
#include "class/LividInstance.h"


std::any LividClass::call(Interpreter interpreter,std::vector<std::any> arguments){
    LividInstance instance(*this);
    return instance;
}