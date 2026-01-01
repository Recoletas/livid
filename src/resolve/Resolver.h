#ifndef RESOLVER_H 
#define RESOLVER_H

#include <vector>
#include <unordered_map>
#include "ast/Expr.h"
#include "ast/Stmt.h"
#include "interpreter/Interpreter.h"

class Resolver :public ExprVisitor,public StmtVisitor{
    private:
        enum class FunctionType{
            NONE,
            FUNCTION,
            INITIALIZER,
            METHOD
        };
        enum class ClassType{
            NONE,
            CLASS,
            SUBCLASS
        };
        ClassType currentClass =ClassType::NONE;
        FunctionType currentFunction =FunctionType::NONE;
        Interpreter& interpreter;
        std::vector<std::shared_ptr<std::unordered_map<std::string, bool>>> scopes;
        
        void beginScope();
        void endScope();
        void declare(Token name);
        void define(Token name);
        void resolveLocal(std::shared_ptr<Expr> expr,Token name);
        void resolveFunction(std::shared_ptr<Function>  function,FunctionType type);
    public:
        Resolver(Interpreter &interpreter): interpreter(interpreter) {}
        void resolve(std::vector<std::shared_ptr<Stmt>> statements);
        void resolve(std::shared_ptr<Stmt> stmt);
        void resolve(std::shared_ptr<Expr> expr);
        void visitBlockStmt(std::shared_ptr<Block> stmt)override;
        void visitClassStmt(std::shared_ptr<Class> stmt)override;
        void visitVarStmt(std::shared_ptr<Var> stmt) override;
        std::any visitVariableExpr(std::shared_ptr<Variable> expr)override;
        std::any visitAssignExpr(std::shared_ptr<Assign> expr)override;
        void visitFunctionStmt(std::shared_ptr<Function> stmt)override;
        void visitExpressionStmt(std::shared_ptr<Expression> stmt)override;
        void visitIfStmt(std::shared_ptr<If> stmt)override;
        void visitPrintStmt(std::shared_ptr<Print> stmt)override;
        void visitReturnStmt(std::shared_ptr<Return> stmt)override;
        void visitWhileStmt(std::shared_ptr<While> stmt)override;
        std::any visitBinaryExpr(std::shared_ptr<Binary> expr)override;
        std::any visitCallExpr(std::shared_ptr<Call> expr)override;
        std::any visitGroupingExpr(std::shared_ptr<Grouping> expr)override;
        std::any visitLiteralExpr(std::shared_ptr<Literal> expr)override;
        std::any visitLogicalExpr(std::shared_ptr<Logical> expr)override;
        std::any visitSetExpr(std::shared_ptr<Set> expr)override;
        std::any visitSuperExpr(std::shared_ptr<Super> expr)override;
        std::any visitThisExpr(std::shared_ptr<This> expr)override;
        std::any visitUnaryExpr(std::shared_ptr<Unary> expr)override;
        std::any visitGetExpr(std::shared_ptr<Get> expr) override;
};

#endif