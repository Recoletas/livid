#ifndef Stmt_H
#define Stmt_H

#include <memory>
#include <vector>
#include <any>
#include "../Token.h"
#include "Expr.h"

class Expression;
class Print;

// Visitor 
class StmtVisitor {
public:
    virtual ~StmtVisitor() = default;

    virtual std::any visitExpressionStmt(std::shared_ptr<Expression> stmt) = 0;
    virtual std::any visitPrintStmt(std::shared_ptr<Print> stmt) = 0;
};

class Stmt {
public:
    virtual ~Stmt() = default;
    virtual std::any accept(StmtVisitor& visitor) = 0;
};

class Expression : public Stmt,
                                 public std::enable_shared_from_this<Expression> {
public:
    Expression(std::shared_ptr<Expr> expression)
        : expression(expression) {}

    // 字段
    std::shared_ptr<Expr> expression;

    std::any accept(StmtVisitor& visitor) override {
        return visitor.visitExpressionStmt(shared_from_this());
    }
};

class Print : public Stmt,
                                 public std::enable_shared_from_this<Print> {
public:
    Print(std::shared_ptr<Expr> expression)
        : expression(expression) {}

    // 字段
    std::shared_ptr<Expr> expression;

    std::any accept(StmtVisitor& visitor) override {
        return visitor.visitPrintStmt(shared_from_this());
    }
};

#endif // Stmt_H
