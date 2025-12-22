#include "resolve/Resolver.h"


void Resolver::visitBlockStmt(std::shared_ptr<Block> stmt){
    beginScope();
    resolve(stmt->statements);
    endScope();
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
    scopes.push(std::make_shared<std::unordered_map<std::string, bool>>());
}
void Resolver::endScope(){
    scopes.pop();
}