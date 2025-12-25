#ifndef PARSER_H
#define PARSER_H

#include "TokenType.h"
#include "Token.h"
#include "ast/Expr.h"
#include "core/livid.h"
#include <vector>
#include "ast/Stmt.h"

class Parser{
    public:
        Parser(std::vector<Token> tokens);
        std::vector<std::shared_ptr<Stmt>> parse();
    private:
        std::vector<Token> tokens;
        int current =0;

        std::shared_ptr<Stmt> statement();
        std::shared_ptr<Expr> oor();
        std::shared_ptr<Expr> andd();
        std::shared_ptr<Stmt> forStatement();
        std::shared_ptr<Stmt> ifStatement();
        std::shared_ptr<Stmt> printStatement();
        std::shared_ptr<Stmt> returnStatement();
        std::shared_ptr<Stmt> expressionOrPrintStatement();
        std::shared_ptr<Function> function(std::string kind);

        class ParseError :public std::exception{
            private:
                std::string message;
            public:
                explicit ParseError(const std::string&msg);
                const char* what() const noexcept override;
        };
        template<typename... TokenTypes>
        bool match(TokenTypes... types);
        Token consume(TokenType type,std::string message);
        bool check(TokenType t);
        [[noreturn]] ParseError error(const Token& token,const std::string & message);
        void synchronize();
        Token advance();
        bool isAtEnd();
        Token peek();
        Token previous();
        std::shared_ptr<Expr> expression();
        std::shared_ptr<Expr> equality();
        std::shared_ptr<Expr> comparision();
        std::shared_ptr<Expr> term();
        std::shared_ptr<Expr> factor();
        std::shared_ptr<Expr> unary();
        std::shared_ptr<Expr> call();
        std::shared_ptr<Expr> finishCall(std::shared_ptr<Expr> callee);
        std::shared_ptr<Expr> primary();
        std::shared_ptr<Stmt> declaration();
        std::shared_ptr<Stmt> classDeclaration();
        std::shared_ptr<Stmt> varDeclaration();
        std::shared_ptr<Stmt> whileStatement();
        std::shared_ptr<Expr> assignment();
        std::vector<std::shared_ptr<Stmt>> block();
};
template<typename... TokenTypes>
bool Parser::match(TokenTypes... types){
    std::initializer_list<TokenType> typeList ={types...};
    for(TokenType type:typeList){
        if(check(type)){
            advance();
            return true;
        }
    }
    return false;
}

#endif