#ifndef AST_PRINTER_H
#define AST_PRINTER_H

#include "./h/Expr.h"
#include <string>
#include <memory>
#include <sstream>

class AstPrinter : public ExprVisitor {
public:
    std::string print(std::shared_ptr<Expr> expr) {
        return std::any_cast<std::string>(expr->accept(*this));
    }

private:
    //unelgeant string funcion(fwcpp) 
    std::string anyToString(const std::any& value){
        if(!value.has_value()){
            return "";
        }
        if(value.type()==typeid(std::string)){
            return std::any_cast<std::string>(value);
        }
        return "Error:Not a string";
    }
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

            try {
                builder << std::any_cast<std::string>(expr->accept(*this));
            } catch (const std::bad_any_cast& e) {
                // 如果出现类型错误,输出提示
                builder << "Error-Cast"; 
            }
            
        }
        builder << ")";
        
        return builder.str();
    }
};

#endif