#ifndef LIVIDINSTANCE_H
#define LIVIDINSTANCE_H

#include <string>
#include <memory>
#include <any>
#include <unordered_map>
#include "core/Token.h"

class LividClass;

class LividInstance: public std::enable_shared_from_this<LividInstance>{
    public:
        LividInstance(std::shared_ptr<LividClass> klass):klass(klass){};
        std::any get(const Token& name);
        void set(const Token& name,std::any value);
        std::string toString();
        std::shared_ptr<LividClass> klass;
    private:
        std::unordered_map<std::string,std::any> fields;

};




#endif


