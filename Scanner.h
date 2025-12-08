#ifndef SCANNER_H
#define SCANNER_H

#include "Token.h"
#include "livid.h"
#include <variant>
#include <unordered_map>
#include <vector>

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
        void number();
        char peeknext();
        void identifier();
        bool isAlphaNumeric(char c);
        static inline const std::unordered_map<std::string, TokenType> keywords = {
        {"and", TokenType::AND},
        {"class", TokenType::CLASS},
        {"else", TokenType::ELSE},
        {"false", TokenType::FALSE},
        {"for", TokenType::FOR},
        {"fun", TokenType::FUN},
        {"if", TokenType::IF},
        {"nil", TokenType::NIL},
        {"or", TokenType::OR},
        {"print", TokenType::PRINT},
        {"return", TokenType::RETURN},
        {"super", TokenType::SUPER},
        {"this", TokenType::THIS},
        {"true", TokenType::TRUE},
        {"var", TokenType::VAR},
        {"while", TokenType::WHILE}
    };
    public:
        Scanner(const std::string &source);
        std::vector<Token> scanTokens();
        void string();

};
#endif