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
        bool isAtEnd(){
            return current>=source.length();
        }
        char advance(){
            return source[current++];
        }
        void addToken(TokenType type){
            addToken(type,nullptr);
        }
        void addToken(TokenType type,std::any literal){
            std::string text = source.substr(start,current);
            tokens.push_back(Token(type,text,literal,line));
        }
    public:
        Scanner(const std::string &source):source(source){}
        std::vector<Token> scanTokens(){
            char c=advance();
            switch (c){
                case '(': addToken(TokenType::LEFT_PAREN); break;
                case ')': addToken(TokenType::RIGHT_PAREN); break;
                case '{': addToken(TokenType::LEFT_BRACE); break;
                case '}': addToken(TokenType::RIGHT_BRACE); break;
                case ',': addToken(TokenType::COMMA); break;
                case '.': addToken(TokenType::DOT); break;
                case '-': addToken(TokenType::MINUS); break;
                case '+': addToken(TokenType::PLUS); break;
                case ';': addToken(TokenType::SEMICOLON); break;
                case '*': addToken(TokenType::STAR); break;
                default:
                    Livid::error(line,"Unexpected character.");
                    break;   
                    
            }
            while(!isAtEnd){
                start=current;
                scanTokens();
            }
            tokens.emplace_back(TokenType::EOF_OF_FILE,"",std::monostate{},line);
            return tokens;
        }
};
#endif