#include "Scanner.h"
#include <any>
#include<vector>

bool Scanner::isAtEnd(){
    return current>=source.length();
}
char Scanner::advance(){
    return source[current++];
}
void Scanner::addToken(TokenType type){
    addToken(type,nullptr);
}
void Scanner::addToken(TokenType type,std::any literal){
    std::string text = source.substr(start,current);
    tokens.push_back(Token(type,text,literal,line));
}
bool Scanner::match(char expected){
    if(isAtEnd()) return false;
    if(source[current]!=expected) return false;
    current++;
    return true;
}
char Scanner::peek(){
    if(isAtEnd()) return '\0';
    return source[current];
}
Scanner::Scanner(const std::string &source):source(source){}
std::vector<Token> Scanner::scanTokens(){
    char c=advance();
    switch (c){
        case '(': addToken(TokenType::LEFT_PAREN); break;
        case ')': addToken(TokenType::RIGHT_PAREN); break;
        case '{': addToken(TokenType::LEFT_BRACE); break;
        case '}': addToken(TokenType::RIGHT_BRACE); break;
        case ',': addToken(TokenType::COMMA); break;
        case '.': addToken(TokenType::DOT); break;
        case '-': addToken(TokenType::MINUS); break;
        case '+': addToken(TokenType::PLUS); break;
        case ';': addToken(TokenType::SEMICOLON); break;
        case '*': addToken(TokenType::STAR); break;
        case '!':addToken(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);break;
        case '=': addToken(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);break;
        case '<':addToken(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);break;
        case '>':addToken(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER); break;
        case '/':
            if (match('/')) {
                while (peek() != '\n' && !isAtEnd()) advance();
            } else {
                    addToken(TokenType::SLASH);
            }
            break;
        case ' ':
        case '\r':
        case '\t':
            break;
        case '\n':
            line++;
            break;
        default:
            Livid::error(line,"Unexpected character.");
            break;  
    }
    while(!isAtEnd){
        start=current;
        scanTokens();
    }
    tokens.emplace_back(TokenType::EOF_OF_FILE,"",std::monostate{},line);
    return tokens;
}
        
