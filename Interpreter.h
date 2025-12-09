#include "./h/Expr.h"
#include <any>
#include <stdexcept>
#include <string>

class Interpreter:public ExprVisitor{
    public:
        std::any visitLiteralExpr(std::shared_ptr<Literal> expr)override;
        std::any visitGroupingExpr(std::shared_ptr<Grouping> expr);
        std::any visitBinaryExpr(std::shared_ptr<Binary> expr);
        void interpret(std::shared_ptr<Expr> expression);
    private:
        void checkNumberOperand(Token op,std::any operand);
        void checkNumberOperands(Token op,std::any left,std::any right);
        bool isEqual(const std::any& a,const std::any& b);
        std::string stringify(std::any obj);
        std::any evaluate(std::shared_ptr<Expr> expr);
        std::any visitUnaryExpr(std::shared_ptr<Unary> expr);
        bool isTruthy(std::any& obj);
};