#ifndef ENVIRONMENT_H 
#define ENVIRONMENT_H

#include <string>
#include <unordered_map>
#include <any>
#include <stdexcept>
#include <memory>
#include "core/Token.h"


class Environment:public std::enable_shared_from_this<Environment> {
    public:
        std::shared_ptr<Environment> enclosing;
        Environment():enclosing(nullptr){};
        Environment(std::shared_ptr<Environment> enclosing) : enclosing(enclosing) {}
        void define(const std::string& name,const std::any& value);
        std::any getAt(int distance,std::string name);
        std::any get(const Token& name);
        void assign(const Token& name,const std::any& value);
        
    private:
        std::unordered_map<std::string,std::any> values;

};

#endif