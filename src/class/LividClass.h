#include <vector>
#include <string>
#include "fun/Callable.h"

class LividClass:public Callable{
    public:
        std::string name;
        LividClass(std::string name):name(name){};
        std::string toString()override;
        int arity()override=0;
        
};