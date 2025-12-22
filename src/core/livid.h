#ifndef LIVID_H
#define LIVID_H

#include <string>
#include "interpreter/Interpreter.h"
#include "core/RuntimeError.h"

class Livid {
private:
    static bool isReplMode;
    static inline bool hadError = false;
    static inline Interpreter interpreter;
public:
    static bool hadRuntimeError;
    static void runtimeError(const RuntimeError& error);
    static void runFile(const std::string& filename);
    static void runPrompt();
    static void run(const std::string &source);
    static void error(int line, const std::string& message);
    static void error(Token token, std::string message);
    static void report(int line, const std::string& where, const std::string& message);

    static void setReplMode(bool mode){isReplMode=mode;}
    static bool hadErrorOccurred() { return hadError; }
    static bool getReplMode() { return isReplMode; }
};

#endif