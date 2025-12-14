#ifndef INTERPRETER_H 
#define INTERPRETER_H

#include "ast/Expr.h"
#include "ast/Stmt.h"
#include <any>
#include <stdexcept>
#include <string>
#include "environment/Environment.h"

class Interpreter:public ExprVisitor,public StmtVisitor{
    public:
        std::any visitLiteralExpr(std::shared_ptr<Literal> expr)override;
        std::any visitGroupingExpr(std::shared_ptr<Grouping> expr)override;
        std::any visitBinaryExpr(std::shared_ptr<Binary> expr)override;
        void interpret(std::vector<std::shared_ptr<Stmt>> statements);
        void visitExpressionStmt(std::shared_ptr<Expression> stmt)override;
        void visitPrintStmt(std::shared_ptr<Print> stmt)override;
        void visitVarStmt(std::shared_ptr<Var> stmt)override;
        std::any visitVariableExpr(std::shared_ptr<Variable> expr)override;
        std::any visitAssignExpr(std::shared_ptr<Assign> expr)override;
        void visitBlockStmt(std::shared_ptr<Block>)override;
        void executeBlock(std::vector<std::shared_ptr<Stmt>> statements,Environment environment);
    private:
        Environment environment;
        void checkNumberOperand(Token op,std::any operand);
        void checkNumberOperands(Token op,std::any left,std::any right);
        bool isEqual(const std::any& a,const std::any& b);
        std::string stringify(std::any obj);
        std::any evaluate(std::shared_ptr<Expr> expr);
        std::any visitUnaryExpr(std::shared_ptr<Unary> expr)override;
        bool isTruthy(std::any& obj);
        void execute( std::shared_ptr<Stmt> stmt);
};

#endif