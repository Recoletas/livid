#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <vector>
#include "Scanner.h"
#include "Token.h"

class Livid{
public:
    static inline bool hadError=false;
    static void runFile(const std::string& filename){
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
    }
    
    static void runPrompt(){
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
    }

    static void run(const std::string source){
        Scanner scanner(source);
        std::vector<Token> tokens=scanner.scanTokens();
        for(const Token& token: tokens){
            std::cout<<token<<std::endl;
        }
    }
    static void error(int line,std::string message){
        report(line,"",message);
    }

private:
    static void report(int line,std::string where,std::string message){
        std::cerr<<"[Line"<<line<<"] Error"<<where<<":"<<message<<std::endl;
        hadError=true;
    }
};

int main(int argc,char * argv[]){
    if(argc>2){
        std::cout<<"Usage: Livid [script]"<<std::endl;
        return 64;
    }else if(argc==2){
        Livid::runFile(argv[1]);
    }else{
        Livid::runPrompt();
    }
    return 0;
}