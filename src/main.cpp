#include "core/livid.h"
#include <iostream>
#include <string>

int main(int argc,char * argv[]){
    if(argc>2){
        std::cout<<"Usage: Livid [script]"<<std::endl;
        return 64;
    }else if(argc==2){
        Livid::runFile(std::string(argv[1]));
    }else{
        Livid::runPrompt();
    }
    return 0;
}