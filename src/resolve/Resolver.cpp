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