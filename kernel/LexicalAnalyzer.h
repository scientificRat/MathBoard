//
//  LexicalAnalyzer.h
//  MathBoard(ConsoleVersion)
//
//  Created by 黄正跃 on 7/23/16.
//  Copyright © 2016 黄正跃. All rights reserved.
//

#ifndef LexicalAnalyzer_h
#define LexicalAnalyzer_h
#include "ExpressionToken.h"
#include <vector>
#include <stdlib.h>
using namespace std;
class LexicalAnalyzer {
private:
    const char operators[14]={'+', '-', '*', '/', '^', '!', '(', ')', '[', ']', '{', '}', '=', ','};
    vector<string> innerFunctions{"sin","cos","tan","cot","arcsin","arccos","arctan","arccot","log","ln","integral","diff","random","plot"};
    
    bool isCharInArray(char c,const char* array);
    
    bool isStringInVector(const string&,vector<string>& array);
    
    bool isOperatorChar(char c);
    
    bool isVarFunction(const string& var);
public:
    vector<ExpressionToken*> generateExpressionTokens(string input);
};

#endif /* LexicalAnalyzer_h */
