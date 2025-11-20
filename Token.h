#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <iostream>
#include <any>
#include "TokenType.h"

class Token{
    private:
        const TokenType type;
        const std::string lexeme;
        const std::any literal;
        const int line;


    public:
        Token(TokenType type,std::string lexeme,std::any literal,int line):
        type(type),lexeme(lexeme),literal(literal),line(line){}
        friend std::ostream& operator<<(std::ostream& os,const Token&token){
            os <<"Token("<<static_cast<int>(token.type)<<",\""<<token.lexeme<<"\")";
            return os;
        }
};
#endif