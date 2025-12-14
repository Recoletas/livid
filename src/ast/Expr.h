#ifndef Expr_H
#define Expr_H

#include <memory>
#include <vector>
#include <any>
#include "core/Token.h"

class Assign;
class Binary;
class Grouping;
class Literal;
class Unary;
class Variable;

// Visitor 
class ExprVisitor {
public:
    virtual ~ExprVisitor() = default;

    virtual std::any visitAssignExpr(std::shared_ptr<Assign> expr) = 0;
    virtual std::any visitBinaryExpr(std::shared_ptr<Binary> expr) = 0;
    virtual std::any visitGroupingExpr(std::shared_ptr<Grouping> expr) = 0;
    virtual std::any visitLiteralExpr(std::shared_ptr<Literal> expr) = 0;
    virtual std::any visitUnaryExpr(std::shared_ptr<Unary> expr) = 0;
    virtual std::any visitVariableExpr(std::shared_ptr<Variable> expr) = 0;
};

class Expr {
public:
    virtual ~Expr() = default;
    virtual std::any accept(ExprVisitor& visitor) = 0;
};

class Assign : public Expr,
                                 public std::enable_shared_from_this<Assign> {
public:
    Assign(Token name, std::shared_ptr<Expr> value)
        : name(name), value(value) {}

    // 字段
    Token name;
    std::shared_ptr<Expr> value;

    std::any accept(ExprVisitor& visitor) override {
        return visitor.visitAssignExpr(shared_from_this());
    }
};

class Binary : public Expr,
                                 public std::enable_shared_from_this<Binary> {
public:
    Binary(std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right)
        : left(left), op(op), right(right) {}

    // 字段
    std::shared_ptr<Expr> left;
    Token op;
    std::shared_ptr<Expr> right;

    std::any accept(ExprVisitor& visitor) override {
        return visitor.visitBinaryExpr(shared_from_this());
    }
};

class Grouping : public Expr,
                                 public std::enable_shared_from_this<Grouping> {
public:
    Grouping(std::shared_ptr<Expr> expression)
        : expression(expression) {}

    // 字段
    std::shared_ptr<Expr> expression;

    std::any accept(ExprVisitor& visitor) override {
        return visitor.visitGroupingExpr(shared_from_this());
    }
};

class Literal : public Expr,
                                 public std::enable_shared_from_this<Literal> {
public:
    Literal(std::any value)
        : value(value) {}

    // 字段
    std::any value;

    std::any accept(ExprVisitor& visitor) override {
        return visitor.visitLiteralExpr(shared_from_this());
    }
};

class Unary : public Expr,
                                 public std::enable_shared_from_this<Unary> {
public:
    Unary(Token op, std::shared_ptr<Expr> right)
        : op(op), right(right) {}

    // 字段
    Token op;
    std::shared_ptr<Expr> right;

    std::any accept(ExprVisitor& visitor) override {
        return visitor.visitUnaryExpr(shared_from_this());
    }
};

class Variable : public Expr,
                                 public std::enable_shared_from_this<Variable> {
public:
    Variable(Token name)
        : name(name) {}

    // 字段
    Token name;

    std::any accept(ExprVisitor& visitor) override {
        return visitor.visitVariableExpr(shared_from_this());
    }
};

#endif // Expr_H
