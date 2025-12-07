#include "TokenType.h"
#include "Token.h"
#include "./h/Expr.h"
#include "livid.h"
#include <vector>
class Parser{
    Parser(std::vector<Token> tokens){
        this->tokens=tokens;
    }
    private:
        std::vector<Token> tokens;
        int current =0;

        class ParseError :public std::exception{
            private:
                std::string message;
            public:
                explicit ParseError(const std::string&msg): message(msg){}
                const char* what() const noexcept override{
                    return message.c_str();
                }
        };
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
        Token consume(TokenType type,std::string message){
            if(check(type)) return advance();
            throw error(peek(),message);
        }
        bool check(TokenType t){
            if(isAtEnd()) return false;
            return peek().getType()==t;
        }
        [[noreturn]] ParseError error(const Token& token,const std::string & message){
            Livid::error(token.getline(),message);
            throw ParseError(message);
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
        std::shared_ptr<Expr> expression(){
            return equality();
        }
        std::shared_ptr<Expr> equality(){
            std::shared_ptr<Expr> expr = comparision();
            while(match(TokenType::BANG_EQUAL,TokenType::EQUAL_EQUAL)){
                Token op =previous();
                std::shared_ptr<Expr> right=comparision();
                expr = std::make_shared<Binary>(expr,op,right);
            }
            return expr;
        }
        std::shared_ptr<Expr> comparision(){
            std::shared_ptr<Expr> expr =term();
            while (match(TokenType::GREATER,TokenType::GREATER_EQUAL,TokenType::LESS,TokenType::LESS_EQUAL)){
                Token op=previous();
                std::shared_ptr<Expr> right =term();
                expr=std::make_shared<Binary>(expr,op,right);
            }
            return expr;
        }
        std::shared_ptr<Expr> term(){
            std::shared_ptr<Expr> expr =factor();
            while(match(TokenType::MINUS,TokenType::PLUS)){
                Token op=previous();
                std::shared_ptr<Expr> right =factor();
                expr=std::make_shared<Binary>(expr,op,right);
            }
            return expr;
        }
        std::shared_ptr<Expr> factor(){
            std::shared_ptr<Expr> expr =unary();
            while(match(TokenType::SLASH,TokenType::STAR)){
                Token op=previous();
                std::shared_ptr<Expr> right =unary();
                expr=std::make_shared<Binary>(expr,op,right);
            }
            return expr;
        }
        std::shared_ptr<Expr> unary(){
            if(match(TokenType::BANG,TokenType::MINUS)){
                Token op=previous();
                std::shared_ptr<Expr> right=unary();
                return std::make_shared<Unary>(op,right);
            }
            return primary();
        }
        std::shared_ptr<Expr> primary(){
            if(match(TokenType::FALSE)) return std::make_shared<Literal>(false);
            if(match(TokenType::TRUE)) return std::make_shared<Literal>(true);
            if(match(TokenType::NIL)) return std::make_shared<Literal>(NULL);

            if(match(TokenType::NUMBER,TokenType::STRING)){
                return std::make_shared<Literal>(previous().getLiteral());
            }

            if(match(TokenType::LEFT_PAREN)){
                std::shared_ptr<Expr> expr =expression();
                consume(TokenType::RIGHT_PAREN,"Expect ')' after expression.");
                return std::make_shared<Grouping>(expr);
            }
        }
            
};