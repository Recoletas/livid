#ifndef SCANNER_H
#define SCANNER_H

#include <string>
#include <vector>
#include "Token.h"
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
    public:
        Scanner(const std::string &source):source(source){}
        std::vector<Token> scanTokens(){
            while(!isAtEnd){
                start=current;
                scanTokens();
            }
            tokens.emplace_back(TokenType::EOF_OF_FILE,"",std::monostate{},line);
            return tokens;
        }
};
#endif