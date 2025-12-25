#include "interpreter/Interpreter.h"
#include "fun/Callable.h"
#include "fun/LividFunction.h"
#include "fun/Return.h"
#include "core/RuntimeError.h"
#include "core/livid.h"
#include "class/LividClass.h"
#include <any>
#include <stdexcept>
#include <iostream>
#include <sstream>

#include <chrono>

class ClockNative : public Callable {
public:
    int arity() override { return 0; }

    std::any call(Interpreter& interpreter, std::vector<std::any> arguments) override {
        auto now = std::chrono::system_clock::now().time_since_epoch();
        double seconds = std::chrono::duration<double>(now).count();
        return seconds;
    }

    std::string toString() override { return "<native fn>"; }
};

Interpreter:: Interpreter():globals(std::make_shared<Environment>()){
    environment = globals;
    globals->define("clock",std::static_pointer_cast<Callable>(std::make_shared<ClockNative>()));
}
void Interpreter::interpret(std::vector<std::shared_ptr<Stmt>> statements) {
    try {
        for (const auto& statement : statements) {
            execute(statement);
        }
    } catch (const RuntimeError& error) {
        Livid::runtimeError(error);
    }
}
void Interpreter::execute( std::shared_ptr<Stmt> stmt){
    stmt->accept(*this);
}
void Interpreter::resolve(std::shared_ptr<Expr> expr,int depth){
    locals[expr]=depth;
}
void Interpreter::executeBlock(std::vector<std::shared_ptr<Stmt>> statements,std::shared_ptr<Environment> environment){
    std::shared_ptr<Environment> previous = this->environment;
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
    this->environment = previous;
}
void Interpreter::visitBlockStmt(std::shared_ptr<Block> stmt){
    auto newEnv=std::make_shared<Environment>(this->environment);
    executeBlock(stmt->statements,newEnv);
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
        }
        case TokenType::LESS_EQUAL:{
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
std::any Interpreter::visitCallExpr(std::shared_ptr<Call> expr){
    std::any callee=evaluate(expr->callee);
    std::vector<std::any> arguments;
    for(std::shared_ptr<Expr> arguement:expr->arguments){
        arguments.push_back(evaluate(arguement));
    }

    std::shared_ptr<Callable> function;
    try{
        function=std::any_cast<std::shared_ptr<Callable>>(callee);
    }catch(const std::bad_any_cast&){
        throw RuntimeError(expr->paren,"Can only call functions and classes.");
    }
    
    if(function==nullptr){
        throw RuntimeError(expr->paren,"Can only call functions and classes.");
    }
    if(arguments.size()!=function->arity()){
        throw RuntimeError(expr->paren,"Expected "+std::to_string(function->arity())+
        " arguments but got "+std::to_string(arguments.size())+".");
    }
    return function->call(*this,arguments);
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
void Interpreter::visitFunctionStmt(std::shared_ptr<Function> stmt){
    auto lividFunc = std::make_shared<LividFunction>(stmt, environment);
    std::shared_ptr<Callable> function = std::static_pointer_cast<Callable>(lividFunc);
    environment->define(stmt->name.getLexeme(),function);
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
void Interpreter::visitReturnStmt(std::shared_ptr<Return> stmt){
    std::any value=std::any{};
    if(stmt->value !=nullptr) value=evaluate(stmt->value);

    throw ReturnException{value};
}
void Interpreter::visitVarStmt(std::shared_ptr<Var> stmt){
    std::any value={};
    if(stmt->initializer!=nullptr){
        value=evaluate(stmt->initializer);
    }

    environment->define(stmt->name.getLexeme(),value);
}
void Interpreter::visitWhileStmt(std::shared_ptr<While> stmt){
    while(isTruthy(evaluate(stmt->condition))){
        execute(stmt->body);
    }
}
std::any Interpreter::visitAssignExpr(std::shared_ptr<Assign> expr){
    std::any value = evaluate(expr->value);
    int distance =locals.at(expr);
    if(distance!=NULL){
        environment->assignAt(distance,expr->name,value);
    }else{
        globals->assign(expr->name,value);
    }
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
    return lookUpVariable(expr->name, expr);
    
}
void Interpreter::visitClassStmt(std::shared_ptr<Class> stmt){
    environment->define(stmt->name.getLexeme(),nullptr);
    LividClass kalss(stmt->name.getLexeme());
    environment->assign(stmt->name,klass);
}
std::any Interpreter::lookUpVariable(Token name, std::shared_ptr<Expr> expr){
    int distance=locals[expr];
    if(distance!=NULL){
        return environment->getAt(distance,name.getLexeme());
    }else{
        return globals->get(name);
    }
}
bool Interpreter::isTruthy(const std::any& obj){
    if(!obj.has_value()) return false;
    if(obj.type()==typeid(bool)){
        //for problems may exist
        try{
            return std::any_cast<bool>(obj);
        }catch(const std::bad_any_cast&e){
            return false;
        }
    }
    return true;
}
        