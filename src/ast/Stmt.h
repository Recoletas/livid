#ifndef Stmt_H
#define Stmt_H

#include <memory>
#include <vector>
#include <any>
#include "core/Token.h"

class Block;
class Expression;
class Function;
class If;
class Print;
class Var;
class While;

// Visitor 
class StmtVisitor {
public:
    virtual ~StmtVisitor() = default;

    virtual void visitBlockStmt(std::shared_ptr<Block> stmt) = 0;
    virtual void visitExpressionStmt(std::shared_ptr<Expression> stmt) = 0;
    virtual void visitFunctionStmt(std::shared_ptr<Function> stmt) = 0;
    virtual void visitIfStmt(std::shared_ptr<If> stmt) = 0;
    virtual void visitPrintStmt(std::shared_ptr<Print> stmt) = 0;
    virtual void visitVarStmt(std::shared_ptr<Var> stmt) = 0;
    virtual void visitWhileStmt(std::shared_ptr<While> stmt) = 0;
};

class Stmt {
public:
    virtual ~Stmt() = default;
    virtual void accept(StmtVisitor& visitor) = 0;
};

class Block : public Stmt,
                                 public std::enable_shared_from_this<Block> {
public:
    Block(std::vector<std::shared_ptr<Stmt>> statements)
        : statements(statements) {}

    // 字段
    std::vector<std::shared_ptr<Stmt>> statements;

    void accept(StmtVisitor& visitor) override {
        return visitor.visitBlockStmt(shared_from_this());
    }
};

class Expression : public Stmt,
                                 public std::enable_shared_from_this<Expression> {
public:
    Expression(std::shared_ptr<Expr> expression)
        : expression(expression) {}

    // 字段
    std::shared_ptr<Expr> expression;

    void accept(StmtVisitor& visitor) override {
        return visitor.visitExpressionStmt(shared_from_this());
    }
};

class Function : public Stmt,
                                 public std::enable_shared_from_this<Function> {
public:
    Function(Token name, std::vector<Token> params, std::vector<std::shared_ptr<Stmt>> body)
        : name(name), params(params), body(body) {}

    // 字段
    Token name;
    std::vector<Token> params;
    std::vector<std::shared_ptr<Stmt>> body;

    void accept(StmtVisitor& visitor) override {
        return visitor.visitFunctionStmt(shared_from_this());
    }
};

class If : public Stmt,
                                 public std::enable_shared_from_this<If> {
public:
    If(std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> thenBranch, std::shared_ptr<Stmt> elseBranch)
        : condition(condition), thenBranch(thenBranch), elseBranch(elseBranch) {}

    // 字段
    std::shared_ptr<Expr> condition;
    std::shared_ptr<Stmt> thenBranch;
    std::shared_ptr<Stmt> elseBranch;

    void accept(StmtVisitor& visitor) override {
        return visitor.visitIfStmt(shared_from_this());
    }
};

class Print : public Stmt,
                                 public std::enable_shared_from_this<Print> {
public:
    Print(std::shared_ptr<Expr> expression)
        : expression(expression) {}

    // 字段
    std::shared_ptr<Expr> expression;

    void accept(StmtVisitor& visitor) override {
        return visitor.visitPrintStmt(shared_from_this());
    }
};

class Var : public Stmt,
                                 public std::enable_shared_from_this<Var> {
public:
    Var(Token name, std::shared_ptr<Expr> initializer)
        : name(name), initializer(initializer) {}

    // 字段
    Token name;
    std::shared_ptr<Expr> initializer;

    void accept(StmtVisitor& visitor) override {
        return visitor.visitVarStmt(shared_from_this());
    }
};

class While : public Stmt,
                                 public std::enable_shared_from_this<While> {
public:
    While(std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> body)
        : condition(condition), body(body) {}

    // 字段
    std::shared_ptr<Expr> condition;
    std::shared_ptr<Stmt> body;

    void accept(StmtVisitor& visitor) override {
        return visitor.visitWhileStmt(shared_from_this());
    }
};

#endif // Stmt_H
