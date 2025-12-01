#ifndef AST_PRINTER_H
#define AST_PRINTER_H

#include "./h/Expr.h"
#include <string>
#include <memory>
#include <sstream>

class AstPrinter : public ExprVisitor {
public:
    std::string print(std::shared_ptr<Expr> expr) {
        return expr->accept(*this);
    }

private:
    // 访问各种表达式节点
    std::any visitBinaryExpr(std::shared_ptr<Binary> expr) override {
        return parenthesize(expr->op.getLexeme(), 
                            {expr->left, expr->right});
    }

    std::any visitGroupingExpr(std::shared_ptr<Grouping> expr) override {
        return parenthesize("group", {expr->expression});
    }

    std::any visitLiteralExpr(std::shared_ptr<Literal> expr) override {
        if (!expr->value.has_value()) {
            return "nil";
        }
        
        // 根据实际存储的类型转换为字符串
        if (expr->value.type() == typeid(std::string)) {
            return std::any_cast<std::string>(expr->value);
        } else if (expr->value.type() == typeid(double)) {
            return std::to_string(std::any_cast<double>(expr->value));
        } else if (expr->value.type() == typeid(bool)) {
            return std::any_cast<bool>(expr->value) ? "true" : "false";
        }
        
        return "unknown";
    }

    std::any visitUnaryExpr(std::shared_ptr<Unary> expr) override {
        return parenthesize(expr->op.getLexeme(), {expr->right});
    }

    // 辅助函数：生成括号表达式
    std::string parenthesize(const std::string& name, 
                            const std::vector<std::shared_ptr<Expr>>& exprs) {
        std::ostringstream builder;
        
        builder << "(" << name;
        for (const auto& expr : exprs) {
            builder << " ";
            std::any result = expr->accept(*this);
            
            // 将 any 转换为 string
            if (result.has_value()) {
                if (result.type() == typeid(std::string)) {
                    builder << std::any_cast<std::string>(result);
                }
            }
        }
        builder << ")";
        
        return builder.str();
    }
};

#endif