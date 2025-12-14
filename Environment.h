#ifndef ENVIRONMENT_H 
#define ENVIRONMENT_H

#include <string>
#include <unordered_map>
#include <any>
#include <stdexcept>
#include <memory>
#include "Token.h"


class Environment:public std::enable_shared_from_this<Environment> {
    public:
        Environment():enclosing(nullptr){};
        void define(const std::string& name,std::any& value);
        std::any get(const Token& name);
        void assign(const Token& name,const std::any& value);
        
    private:
        std::unordered_map<std::string,std::any> values;

        std::shared_ptr<Environment> enclosing;

};

#endif