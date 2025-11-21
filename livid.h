#ifndef LOX_H
#define LOX_H

#include <string>

class Livid {
private:
    static inline bool hadError = false;
    
public:
    static void runFile(const std::string& filename);
    static void runPrompt();
    static void run(const std::string &source);
    static void error(int line, const std::string& message);
    static void report(int line, const std::string& where, const std::string& message);
    
    static bool hadErrorOccurred() { return hadError; }
};

#endif