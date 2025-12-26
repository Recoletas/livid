#include <iostream>
#include <fstream>
#include <stdexcept>
#include <vector>
#include "scanner/Scanner.h"
#include "Token.h"
#include "livid.h"
#include "Parser.h"
#include "AstPrinter.h"
#include "RuntimeError.h"
#include "resolve/Resolver.h"

bool Livid::isReplMode =false;

void Livid::runFile(const std::string& filename){
    Livid::setReplMode(false);
    try{
        std::ifstream file(filename);
        if(!file.is_open()){
            std::cerr<<"Error: Could not open the file: ' "<<filename<<"'"<<std::endl;
            exit(66);
        }
        std::string source((std::istreambuf_iterator<char>(file)),std::istreambuf_iterator<char>());
        run(source);
        if(hadError) exit(65);
    }catch(const std::exception &e){
        std::cerr<<"Error: "<<e.what()<<std::endl;
        exit(65);
    }
    if(hadRuntimeError) exit(70);
}
    
void Livid::runPrompt(){
    Livid::setReplMode(true);
    std::cout<<"Livid interactive mode(Ctrl+D to exit)"<<std::endl;
    std::string line;
    while(true){
        std::cout<<">";
        if(!std::getline(std::cin,line)){
            std::cout<<"\nGoodbye!"<<std::endl;
            break;
        }
        if(!line.empty()){
            run(line);
            hadError=false;
        }
    }
    Livid::setReplMode(false);
}

void Livid::run(const std::string &source){
    Scanner scanner(source);
    std::vector<Token> tokens=scanner.scanTokens();
    Parser parser(tokens);
    std::vector<std::shared_ptr<Stmt>> statements=parser.parse();
    
    Resolver resolver(interpreter);
    resolver.resolve(statements);
    if(hadError) return;
    interpreter.interpret(statements);

}
void Livid::error(int line,const std::string& message){
    report(line,"",message);
}
void Livid::error(Token token,std::string message){
    if(token.getType()==TokenType::EOF_OF_FILE){
        report(token.getline()," at end ",message);
    }else{
        report(token.getline()," at '"+token.getLexeme()+"'",message);
    }
}
bool Livid::hadRuntimeError=false;
void Livid::runtimeError(const RuntimeError& error){
    std::cerr<<error.what()<<"\n[ line "<<error.token.getline()<<"]"<<std::endl;
    Livid::hadRuntimeError =true;
}

void Livid::report(int line,const std::string &where,const std::string &message){
    std::cerr<<"[Line"<<line<<"] Error"<<where<<":"<<message<<std::endl;
    hadError=true;
}
