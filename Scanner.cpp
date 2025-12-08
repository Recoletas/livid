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
    std::string text = source.substr(start,current-start);
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
        case '"':string();break;
        default:
            if(isdigit(c)){
                number();
            }else if(isalpha(c)){
                identifier();
            }
            else{
                Livid::error(line,"Unexpected character.");
            }
            break;  
    }
    while(!isAtEnd()){
        start=current;
        scanTokens();
    }
    tokens.emplace_back(TokenType::EOF_OF_FILE,"",std::monostate{},line);
    return tokens;
}
void Scanner::string(){
    while(peek()!='"'&&!isAtEnd()){
        if(peek()=='\n') line++;
        advance();
    }
    if(isAtEnd()){
        Livid::error(line,"Untermined string.");
        return ;
    }
    advance();
    std::string value =source.substr(start+1,current-start-2);
    addToken(TokenType::STRING,value);
}
void Scanner::number(){
    while(isdigit(peek())) advance();
    //for fractional part.
    if(peek()=='.'&&isdigit(peeknext())){
        advance();
        while(isdigit(peek())) advance();
    }
    
    addToken(TokenType::NUMBER,std::stod(source.substr(start,current-start)));
}
char Scanner::peeknext(){
    if(current+1>=source.length()) return '\0';
    return source[current+1];
}       
void Scanner::identifier(){
    while(isAlphaNumeric(peek())) advance();

    std::string text = source.substr(start, current-start);
    TokenType type = TokenType::IDENTIFIER;  
    auto it = keywords.find(text);
    if (it != keywords.end()) {
        type = it->second;  
    }
    addToken(type);
}
bool Scanner::isAlphaNumeric(char c){
    return isalpha(c)||isdigit(c);
}