//
//  Interpreter.h
//  MathBoard(ConsoleVersion)
//
//  Created by 黄正跃 on 7/26/16.
//  Copyright © 2016 黄正跃. All rights reserved.
//

#ifndef Interpreter_h
#define Interpreter_h
#include "LexicalAnalyzer.h"
#include "ASTBuilder.h"
#include "ExecuteUnit.h"
#include "DisplayResult.h"

class Interpreter {
private:
    LexicalAnalyzer lexicalAnalyzer;
    ASTBuilder astBuilder;
    ExecuteUnit executeUnit;
public:
    DisplayResult* run(string& input);
};

#endif /* Interpreter_h */
