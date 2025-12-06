#include "TokenType.h"
#include "Token.h"
#include "./h/Expr.h"
#include <vector>
class Parser{
    Parser(std::vector<Token> tokens){
        this->tokens=tokens;
    }
    private:
        std::vector<Token> tokens;
        int current =0;

        template<typename... TokenTypes>
        bool match(TokenTypes... types){
            std::initializer_list<TokenType> typeList ={types...};
            for(TokenType type:typeList){
                if(check(type)){
                    advance();
                    return true;
                }
            }
            return false;
        }
        bool check(TokenType t){
            if(isAtEnd()) return false;
            return peek().getType()==t;
        }
        Token advance(){
            if(!isAtEnd()) current++;
            return previous();
        }
        bool isAtEnd(){
            return peek().getType()==TokenType::EOF_OF_FILE;
        }
        Token peek(){
            return tokens[current];
        }
        Token previous(){
            return tokens[current-1];
        }
        Expr expression(){
            return equality();
        }
        Expr equality(){
            Expr expr = comparision();
            while(match(TokenType::BANG_EQUAL,TokenType::EQUAL_EQUAL)){
                Token op =previous();
                Expr right=comparison();
                expr = std::make_shared<Binary>(expr,op,right);
            }
            return expr;
        }
        Expr comparision(){
            Expr expr =term();
            while (match(TokenType::GREATER,TokenType::GREATER_EQUAL,))
        }    
};