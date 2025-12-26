#ifndef LIVIDINSTANCE_H
#define LIVIDINSTANCE_H

#include "class/LividClass.h"
#include <string>

class LividInstance{
    public:
        LividInstance(std::shared_ptr<LividClass> klass):klass(klass){};
        std::any get(const Token& name);
        void set(const Token& name,std::any value);
        std::string toString();
    private:
        std::shared_ptr<LividClass> klass;
        std::unordered_map<std::string,std::any> fields;

};




#endif


