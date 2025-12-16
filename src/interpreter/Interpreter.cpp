#include "interpreter/Interpreter.h"
#include "core/RuntimeError.h"
#include "core/livid.h"
#include <any>
#include <stdexcept>
#include <iostream>
#include <sstream>

void Interpreter:: interpret(std::vector<std::shared_ptr<Stmt>> statements){
    try{
        for( std::shared_ptr<Stmt> statement:statements){
            execute(statement);
        }
    }catch(RuntimeError error){
        Livid::runtimeError(error);
    }
}
void Interpreter::execute( std::shared_ptr<Stmt> stmt){
    stmt->accept(*this);
}
void Interpreter::executeBlock(std::vector<std::shared_ptr<Stmt>> statements,Environment environment){
    Environment previous = this->environment;
    try{
        this->environment=environment;
        for(std::shared_ptr<Stmt> statement:statements){
            execute(statement);
        }
        this->environment=previous;
    }catch(...){
        this->environment=previous;
        throw;
    }
}
void Interpreter::visitBlockStmt(std::shared_ptr<Block> stmt){
    executeBlock(stmt->statements,Environment(environment));
    return ;
}
std::any Interpreter::visitLiteralExpr(std::shared_ptr<Literal> expr){
    return expr->value;
}
std::any Interpreter::visitLogicalExpr(std::shared_ptr<Logical> expr){
    std::any left=evaluate(expr->left);

    if(expr->op.getType()==TokenType::OR){
        if(isTruthy(left)) return left;
    }else{
        if(!isTruthy(left)) return left;
    }

    return evaluate(expr->right);
}
std::any Interpreter::visitGroupingExpr(std::shared_ptr<Grouping> expr){
    return evaluate(expr->expression);
}
std::any Interpreter::visitBinaryExpr(std::shared_ptr<Binary> expr){
    std::any left=evaluate(expr->left);
    std::any right=evaluate(expr->right);

    switch (expr->op.getType())
    {
        case TokenType::BANG_EQUAL:return !isEqual(left,right);
        case TokenType::EQUAL_EQUAL:return isEqual(left,right);
        case TokenType::GREATER:{
            checkNumberOperands(expr->op,left,right);
            return std::any_cast<double>(left)>std::any_cast<double>(right);
        }
        case TokenType::GREATER_EQUAL:{
            checkNumberOperands(expr->op,left,right);
            return std::any_cast<double>(left)>=std::any_cast<double>(right);
        }
        case TokenType::LESS:{
            checkNumberOperands(expr->op,left,right);
            return std::any_cast<double>(left)<std::any_cast<double>(right);
            case TokenType::LESS_EQUAL:
            checkNumberOperands(expr->op,left,right);
            return std::any_cast<double>(left)<=std::any_cast<double>(right); 
        }   
        case TokenType::MINUS:{
            checkNumberOperands(expr->op,left,right);
            return std::any_cast<double>(left)-std::any_cast<double>(right);
        }
        case TokenType::PLUS:{
            if(left.type()==typeid(double)&&right.type()==typeid(double)){
                return std::any_cast<double>(left)+std::any_cast<double>(right);
            }
            
            if(left.type()==typeid(std::string)&&right.type()==typeid(std::string)){
                return std::any_cast<std::string>(left)+std::any_cast<std::string>(right);
            }

            throw RuntimeError(expr->op,"Operands must be two numbers or two strings.");
            break;
        }
        case TokenType::SLASH:{
            checkNumberOperands(expr->op,left,right);
            return std::any_cast<double>(left)/std::any_cast<double>(right);
        }
            
        case TokenType::STAR:{
            checkNumberOperands(expr->op,left,right);
            return std::any_cast<double>(left)*std::any_cast<double>(right);
        }
            
    }
    return nullptr;
}
void Interpreter::checkNumberOperands(Token op,std::any left,std::any right){
    if(left.type()==typeid(double)&&right.type()==typeid(double)) return ;
    throw RuntimeError(op,"Operands must be numbers.");
}
void Interpreter::checkNumberOperand(Token op,std::any operand){
    if(operand.type()==typeid(double)) return ;
    throw RuntimeError(op,"Operand must be a number.");
}
bool Interpreter::isEqual(const std::any& a,const std::any& b){
    if(!a.has_value()&&!b.has_value()) return true;
    if(!a.has_value()||!b.has_value()) return false;
    if(a.type()!=b.type()) return false;

    if(a.type()==typeid(double)){
        return std::any_cast<double>(a)==std::any_cast<double>(b);
    }
    if (a.type() == typeid(bool)) {
        return std::any_cast<bool>(a) == std::any_cast<bool>(b);
    }

    if (a.type() == typeid(std::string)) {
        return std::any_cast<std::string>(a) == std::any_cast<std::string>(b);
    }
    return false;
}
std::string Interpreter::stringify(std::any obj){
    if(!obj.has_value()) return "nil";

    if(obj.type()==typeid(bool)){
        return std::any_cast<bool>(obj)?"true":"false";
    }

    if(obj.type()==typeid(double)){
        double number =std::any_cast<double>(obj);
        std::string text=std::to_string(number);

        if(text.length()>2&&text.substr(text.length()-2)==".0"){
            text=text.substr(0,text.length()-2);
        }
        return text;
    }
    if(obj.type()==typeid(std::string)){
        return std::any_cast<std::string>(obj);
    }
    return "<Unknown object>";
}
std::any Interpreter::evaluate(std::shared_ptr<Expr> expr){
    return expr->accept(*this);
}
void Interpreter::visitExpressionStmt(std::shared_ptr<Expression> stmt){
    evaluate(stmt->expression);
}
void Interpreter::visitIfStmt(std::shared_ptr<If> stmt){
    std::any conditionValue = evaluate(stmt->condition);
    if(isTruthy(conditionValue)){
        execute(stmt->thenBranch);
    }else if(stmt->elseBranch != nullptr){
        execute(stmt->elseBranch);
    }
}
void Interpreter::visitPrintStmt(std::shared_ptr<Print> stmt){
    std::any value = evaluate(stmt->expression);
    std::cout<<stringify(value)<<std::endl;
}
void Interpreter::visitVarStmt(std::shared_ptr<Var> stmt){
    std::any value={};
    if(stmt->initializer!=nullptr){
        value=evaluate(stmt->initializer);
    }

    environment.define(stmt->name.getLexeme(),value);
}
std::any Interpreter::visitAssignExpr(std::shared_ptr<Assign> expr){
    std::any value = evaluate(expr->value);
    environment.assign(expr->name,value);
    return value;
}
std::any Interpreter::visitUnaryExpr(std::shared_ptr<Unary> expr){
    std::any right =evaluate(expr->right);
    switch (expr->op.getType())
    {
    case TokenType::MINUS:{
        checkNumberOperand(expr->op,right);
        return -std::any_cast<double>(right);
    }
        
    case TokenType::BANG:
        return !isTruthy(right);
    }
    return std::any{};
}
std::any Interpreter::visitVariableExpr(std::shared_ptr<Variable> expr){
    return environment.get(expr->name);
}
bool Interpreter::isTruthy(std::any& obj){
    if(!obj.has_value()) return false;
    if(obj.type()==typeid(bool)){
        //for may problems
        try{
            return std::any_cast<bool>(obj);
        }catch(const std::bad_any_cast&e){
            return false;
        }
    }
    return true;
}
        