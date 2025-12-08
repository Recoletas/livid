#include "livid.h"
#include <iostream>

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