#ifndef RESOLVER_H 
#define RESOLVER_H

#include <vector>
#include <stack>
#include <unordered_map>
#include "ast/Expr.h"
#include "ast/Stmt.h"
#include "interpreter/Interpreter.h"

class Resolver :public ExprVisitor,public StmtVisitor{
    private:
        Interpreter interpreter;
        std::stack<std::shared_ptr<std::unordered_map<std::string, bool>>> scopes;
        void resolve(std::vector<std::shared_ptr<Stmt>> statements);
        void resolve(std::shared_ptr<Stmt> stmt);
        void resolve(std::shared_ptr<Expr> expr);
        void beginScope();
        void endScope();
    public:
        Resolver(Interpreter interpreter){this->interpreter=interpreter;}
        void visitBlockStmt(std::shared_ptr<Block> stmt)override;
        
        
};


#endif