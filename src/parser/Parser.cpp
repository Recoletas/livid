#include "Parser.h"
#include "ast/Stmt.h"

Parser::Parser(std::vector<Token> tokens):tokens(std::move(tokens)){}

std::vector<std::shared_ptr<Stmt>> Parser::parse(){
    std::vector<std::shared_ptr<Stmt>> statements;
    while (!isAtEnd())
    {
        statements.push_back(declaration());
    }
    return statements;
}

Parser::ParseError::ParseError(const std::string&msg): message(msg){}
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
    return assignment();
}
std::shared_ptr<Stmt> Parser::declaration(){
    try{
        if(match(TokenType::VAR)) return varDeclaration();

        return statement();
    }catch(ParseError error){
        synchronize();
        return nullptr;
    }
}
std::shared_ptr<Stmt> Parser::statement(){
    if(match(TokenType::IF)) return ifStatement();
    if(match(TokenType::PRINT)) return printStatement();
    if(match(TokenType::LEFT_BRACE)) return std::make_shared<Block>(block());

    return expressionOrPrintStatement();
}
std::shared_ptr<Stmt> Parser::ifStatement(){
    consume(TokenType::LEFT_PAREN,"Expect '(' after 'if'.");
    std::shared_ptr<Expr> condition =expression();
    consume(TokenType::RIGHT_PAREN,"Expect ')' after if condition.");

    std::shared_ptr<Stmt> thenBranch=statement();
    std::shared_ptr<Stmt> elseBranch=nullptr;
    if(match(TokenType::ELSE)){
        elseBranch=statement();
    }

    return std::make_shared<If>(condition,thenBranch,elseBranch);
}
std::shared_ptr<Stmt> Parser::printStatement(){
    std::shared_ptr<Expr> value=expression();
    consume(TokenType::SEMICOLON,"Expect ';' after value.");
    return std::make_shared<Print>(value);
}
std::shared_ptr<Stmt> Parser::varDeclaration(){
    Token name =consume(TokenType::IDENTIFIER,"Expect variable name.");

    std::shared_ptr<Expr> initializer=nullptr;
    if(match(TokenType::EQUAL)){
        initializer=expression();
    }

    consume(TokenType::SEMICOLON,"Expect ';' after variable declaration.");
    return std::make_shared<Var>(name,initializer);
}
std::shared_ptr<Stmt> Parser::expressionOrPrintStatement(){
    std::shared_ptr<Expr> expr=expression();
    if(match(TokenType::SEMICOLON)){
        return std::make_shared<Expression>(expr);
    }
    if(Livid::getReplMode()){
        return std::make_shared<Print>(expr);
    }else{
        throw error(previous(), "Expect ';' after expression.");
    }
    
}
std::vector <std::shared_ptr<Stmt>> Parser::block(){
    std::vector<std::shared_ptr<Stmt>>statements;

    while(!check(TokenType::RIGHT_BRACE)&&!isAtEnd()){
        statements.push_back(declaration());
    }

    consume(TokenType::RIGHT_BRACE,"Except ')' after block.");
    return statements;
}
std::shared_ptr<Expr> Parser::assignment(){
    std::shared_ptr<Expr> expr=oor();

    if(match(TokenType::EQUAL)){
        Token equals=previous();
        std::shared_ptr<Expr> value=assignment();

        //指针向下转型
        std::shared_ptr<Variable> Variable_expr =std::dynamic_pointer_cast<Variable>(expr);

        if(Variable_expr!= nullptr){
            Token name =Variable_expr->name;
            return std::make_shared<Assign>(name,value);
        }

        error(equals,"Invalid assignment target.");
    }

    return expr;
}
std::shared_ptr<Expr> Parser::oor(){
    std::shared_ptr<Expr> expr =andd();
    
    while(match(TokenType::OR)){
        Token op=previous();
        std::shared_ptr<Expr> right=andd();
        expr=std::make_shared<Logical>(expr,op,right);
    }

    return expr;
}
std::shared_ptr<Expr> Parser::andd(){
    std::shared_ptr<Expr> expr =equality();

    while(match(TokenType::AND)){
        Token op=previous();
        std::shared_ptr<Expr> right=equality();
        expr =std::make_shared<Logical>(expr,op,right);
    }
    return expr;
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
    if(match(TokenType::NIL)) return std::make_shared<Literal>(nullptr);

    if(match(TokenType::NUMBER,TokenType::STRING)){
        return std::make_shared<Literal>(previous().getLiteral());
    }

    if (match(TokenType::IDENTIFIER)) {
      return std::make_shared<Variable> (previous());
    }

    if(match(TokenType::LEFT_PAREN)){
        std::shared_ptr<Expr> expr =expression();
        consume(TokenType::RIGHT_PAREN,"Expect ')' after expression.");
        return std::make_shared<Grouping>(expr);
    }

    throw error(peek(),"Expect expression.");
}           