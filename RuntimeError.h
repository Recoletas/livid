#ifndef RUNTIMEERROR_H // <-- 第一行
#define RUNTIMEERROR_H

#include <stdexcept>
#include "Token.h"
#include <string>

class RuntimeError :public std::runtime_error{
    public:
        const Token token;
        RuntimeError(const Token&token,const std::string&message):std::runtime_error(message),token(token){}
};

#endif