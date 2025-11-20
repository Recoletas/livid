#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <iostream>

enum class TokenType{
    IDENTIFIER,STRING,NUMBER,
    LEFT_PAREN,RIGHT_PAREN,
    EOF_TOKEN
};

class Token{
    private:
        TokenType type;
        std::string lexeme;

    public:
        Token(){}
        friend std::ostream& operator<<(std::ostream& os,const Token&token){
            os <<"Token("<<static_cast<int>(token.type)<<",\""<<token.lexeme<<"\")";
            return os;
        }
};
#endif