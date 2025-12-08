#include "Parser.h"

Parser::Parser(std::vector<Token> tokens){
    this->tokens=tokens;
}
std::shared_ptr<Expr> Parser::parse(){
    try{
        return expression();
    }catch (ParseError error){
        return NULL;
    }
}

explicit Parser::ParseError::ParseError(const std::string&msg): message(msg){}
const char* Parser::ParseError::what() const noexcept {
    return message.c_str();
}


Token Parser::consume(TokenType type,std::string message){
    if(check(type)) return advance();
    throw error(peek(),message);
}
bool Parser::check(TokenType t){
    if(isAtEnd()) return false;
    return peek().getType()==t;
}
[[noreturn]] Parser::ParseError Parser::error(const Token& token,const std::string & message){
    Livid::error(token.getline(),message);
    throw ParseError(message);
}
void Parser::synchronize(){
    advance();
    while (!isAtEnd())
    {
        if(previous().getType()==TokenType::SEMICOLON) return;

        switch ( peek().getType())
        {
        case TokenType::CLASS:
        case TokenType::FUN:
        case TokenType::VAR:
        case TokenType::FOR:
        case TokenType::IF:
        case TokenType::WHILE:
        case TokenType::PRINT:
        case TokenType::RETURN:
            return;
        }
        advance();
    }
}
Token Parser::advance(){
    if(!isAtEnd()) current++;
    return previous();
}
bool Parser::isAtEnd(){
    return peek().getType()==TokenType::EOF_OF_FILE;
}
Token Parser::peek(){
    return tokens[current];
}
Token Parser::previous(){
    return tokens[current-1];
}
std::shared_ptr<Expr> Parser::expression(){
    return equality();
}
std::shared_ptr<Expr> Parser::equality(){
    std::shared_ptr<Expr> expr = comparision();
    while(match(TokenType::BANG_EQUAL,TokenType::EQUAL_EQUAL)){
        Token op =previous();
        std::shared_ptr<Expr> right=comparision();
        expr = std::make_shared<Binary>(expr,op,right);
    }
    return expr;
}
std::shared_ptr<Expr> Parser::comparision(){
    std::shared_ptr<Expr> expr =term();
    while (match(TokenType::GREATER,TokenType::GREATER_EQUAL,TokenType::LESS,TokenType::LESS_EQUAL)){
        Token op=previous();
        std::shared_ptr<Expr> right =term();
        expr=std::make_shared<Binary>(expr,op,right);
    }
    return expr;
}
std::shared_ptr<Expr> Parser::term(){
    std::shared_ptr<Expr> expr =factor();
    while(match(TokenType::MINUS,TokenType::PLUS)){
        Token op=previous();
        std::shared_ptr<Expr> right =factor();
        expr=std::make_shared<Binary>(expr,op,right);
    }
    return expr;
}
std::shared_ptr<Expr> Parser::factor(){
    std::shared_ptr<Expr> expr =unary();
    while(match(TokenType::SLASH,TokenType::STAR)){
        Token op=previous();
        std::shared_ptr<Expr> right =unary();
        expr=std::make_shared<Binary>(expr,op,right);
    }
    return expr;
}
std::shared_ptr<Expr> Parser::unary(){
    if(match(TokenType::BANG,TokenType::MINUS)){
        Token op=previous();
        std::shared_ptr<Expr> right=unary();
        return std::make_shared<Unary>(op,right);
    }
    return primary();
}
std::shared_ptr<Expr> Parser::primary(){
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

    throw error(peek(),"Expect expression.");
}           