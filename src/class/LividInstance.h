#ifndef LIVIDINSTANCE_H
#define LIVIDINSTANCE_H

#include "class/LividClass.h"
#include <string>

class LividInstance{
    public:
        LividInstance(LividClass klass):klass(klass){};
        std::any get(Token name);
        std::string toString()override;
    private:
        LividClass klass;
        std::unordered_map<std::string,std::any> fields;

};




#endif


