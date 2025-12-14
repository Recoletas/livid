#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <iostream>
#include <any>
#include "core/TokenType.h"

class Token{
    private:
        const TokenType type;
        const std::string lexeme;
        const std::any literal;
        const int line;


    public:
        Token(const Token& other):type(other.type),lexeme(other.lexeme),literal(other.literal),line(other.line){}
        Token& operator =(const Token&)=delete;

        Token(Token&&) =default;
        Token& operator =(Token&&) =default;

        Token(TokenType type,std::string lexeme,std::any literal,int line):
        type(type),lexeme(lexeme),literal(literal),line(line){}
        const std::string& getLexeme() const{
            return lexeme;
        }
        TokenType getType() const{
            return type;
        }
        const std::any & getLiteral() const {
            return literal;
        }
        int getline() const{
            return line;
        }
        friend std::ostream& operator<<(std::ostream& os,const Token&token){
            os <<"Token("<<static_cast<int>(token.type)<<",\""<<token.lexeme<<"\")";
            return os;
        }
};
#endif