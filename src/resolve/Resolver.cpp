#include "resolve/Resolver.h"
#include "core/livid.h"
#include "interpreter/Interpreter.h"

void Resolver::visitBlockStmt(std::shared_ptr<Block> stmt){
    beginScope();
    resolve(stmt->statements);
    endScope();
}
void Resolver::visitVarStmt(std::shared_ptr<Var> stmt) {
    declare(stmt->name);
    if(stmt->initializer!=nullptr){
        resolve(stmt->initializer);
    }
    define(stmt->name);
}
void Resolver::resolve(std::vector<std::shared_ptr<Stmt>> statements) {
    for (std::shared_ptr<Stmt> statement : statements) {
      resolve(statement);
    }
}
void Resolver::resolve(std::shared_ptr<Stmt> stmt){
    stmt->accept(*this);
}
void Resolver::resolve(std::shared_ptr<Expr> expr){
    expr->accept(*this);
}
void Resolver::beginScope(){
    scopes.push_back(std::make_shared<std::unordered_map<std::string, bool>>());
}
void Resolver::endScope(){
    scopes.pop_back();
}
void Resolver::declare(Token name){
    if(scopes.empty()) return ;
    auto& scope= *scopes.back();
    if(scope.find(name.getLexeme())!=scope.end()){
        Livid::error(name,"Already a variable with this name in this scope.");
    }
    scope[name.getLexeme()]=false;
}
void Resolver::define(Token name){
    if(scopes.empty()) return ;
    (*scopes.back())[name.getLexeme()]=true;
}
std::any Resolver::visitVariableExpr(std::shared_ptr<Variable> expr){
    if(!scopes.empty()){
        auto& scope=*scopes.back();
        auto it=scope.find(expr->name.getLexeme());
        if(it!=scope.end()&&it->second==false){
            Livid::error(expr->name,"Can't read local variable in its own initializer.");
        }
    }
    resolveLocal(expr,expr->name);
    return std::any{};
}
void Resolver::resolveLocal(std::shared_ptr<Expr> expr,Token name){
    for(int i=scopes.size()-1;i>=0;i--){
        if(scopes[i]->find(name.getLexeme())!=scopes[i]->end()){
            interpreter.resolve(expr,(int)(scopes.size()-1-i));
            return ;
        }
    }
}
std::any Resolver::visitAssignExpr(std::shared_ptr<Assign> expr) {
    resolve(expr->value);
    resolveLocal(expr, expr->name);
    return std::any{};
}
void Resolver::visitFunctionStmt(std::shared_ptr<Function> stmt) {
    declare(stmt->name);
    define(stmt->name);

    resolveFunction(stmt,FunctionType::FUNCTION);
}
void Resolver::resolveFunction(std::shared_ptr<Function>  function,FunctionType type) {
    FunctionType enclosingFunction =currentFunction;
    currentFunction=type;
    beginScope();
    for (Token param : function->params) {
      declare(param);
      define(param);
    }
    resolve(function->body);
    endScope();
    currentFunction=enclosingFunction;
}
void Resolver::visitExpressionStmt(std::shared_ptr<Expression> stmt){
    resolve(stmt->expression);
}
void Resolver::visitIfStmt(std::shared_ptr<If> stmt){
    resolve(stmt->condition);
    resolve(stmt->thenBranch);
    if(stmt->elseBranch !=nullptr) resolve(stmt->elseBranch);
}
void Resolver::visitPrintStmt(std::shared_ptr<Print> stmt){
    resolve(stmt->expression);
}
void Resolver::visitReturnStmt(std::shared_ptr<Return> stmt){
    if(currentFunction==FunctionType::NONE){
        Livid::error(stmt->keyword,"Can't return from top-level code.");
    }
    if(stmt->value!=nullptr){
        resolve(stmt->value);
    }
}
void Resolver::visitWhileStmt(std::shared_ptr<While> stmt){
    resolve(stmt->condition);
    resolve(stmt->body);
}
std::any Resolver::visitBinaryExpr(std::shared_ptr<Binary> expr){
    resolve(expr->left);
    resolve(expr->right);
    return std::any{};
}
std::any Resolver::visitCallExpr(std::shared_ptr<Call> expr){
    resolve(expr->callee);
    for(std::shared_ptr<Expr> arguement:expr->arguments){
        resolve(arguement);
    }
    return std::any{};
}
std::any Resolver::visitGroupingExpr(std::shared_ptr<Grouping> expr){
    resolve(expr->expression);
    return std::any{};
}
std::any Resolver::visitLiteralExpr(std::shared_ptr<Literal> expr){
    return std::any{};
}   
std::any Resolver::visitLogicalExpr(std::shared_ptr<Logical> expr){
    resolve(expr->left);
    resolve(expr->right);
    return std::any{};
}
std::any Resolver::visitUnaryExpr(std::shared_ptr<Unary> expr){
    resolve(expr->right);
    return std::any{};
}