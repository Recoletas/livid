#ifndef RESOLVER_H 
#define RESOLVER_H

#include <vector>
#include <unordered_map>
#include "ast/Expr.h"
#include "ast/Stmt.h"
#include "interpreter/Interpreter.h"

class Resolver :public ExprVisitor,public StmtVisitor{
    private:
        Interpreter interpreter;
        std::vector<std::shared_ptr<std::unordered_map<std::string, bool>>> scopes;
        void resolve(std::vector<std::shared_ptr<Stmt>> statements);
        void resolve(std::shared_ptr<Stmt> stmt);
        void resolve(std::shared_ptr<Expr> expr);
        void beginScope();
        void endScope();
        void declare(Token name);
        void define(Token name);
        void resolveLocal(std::shared_ptr<Expr> expr,Token name);
    public:
        Resolver(Interpreter interpreter){this->interpreter=interpreter;}
        void visitBlockStmt(std::shared_ptr<Block> stmt)override;
        void visitVarStmt(std::shared_ptr<Var> stmt) override;
        std::any visitVariableExpr(std::shared_ptr<Variable> expr)override;
        
};


#endif