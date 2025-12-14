#ifndef LIVID_H
#define LIVID_H

#include <string>
#include "Interpreter.h"
#include "RuntimeError.h"

class Livid {
private:
    static inline bool hadError = false;
    static inline Interpreter interpreter;
public:
    static bool hadRuntimeError;
    static void runtimeError(const RuntimeError& error);
    static void runFile(const std::string& filename);
    static void runPrompt();
    static void run(const std::string &source);
    static void error(int line, const std::string& message);
    static void report(int line, const std::string& where, const std::string& message);
    
    static bool hadErrorOccurred() { return hadError; }
};

#endif