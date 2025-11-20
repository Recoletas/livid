#ifndef SCANNER_H
#define SCANNER_H

#include <string>
#include <vector>
#include "Token.h"

class Scanner{
    private:
        std::string source;
    public:
        Scanner(const std::string &source):source(source){}
        std::vector<Token> scanTokens(){
            std::vector<Token> tokens;
            return tokens;
        }
};
#endif