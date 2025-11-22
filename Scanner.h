#ifndef SCANNER_H
#define SCANNER_H

#include "Token.h"
#include "livid.h"
#include <variant>

class Scanner{
    private:
        const std::string source;
        std::vector<Token> tokens;
        int start =0;
        int current =0;
        int line =1;
        bool isAtEnd();
        char advance();
        void addToken(TokenType type);
        void addToken(TokenType type,std::any literal);
        bool match(char expected);
        char peek();
    public:
        Scanner(const std::string &source);
        std::vector<Token> scanTokens();

};
#endif