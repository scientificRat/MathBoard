//
//  Interpreter.cpp
//  MathBoard(ConsoleVersion)
//
//  Created by 黄正跃 on 7/26/16.
//  Copyright © 2016 黄正跃. All rights reserved.
//

#include "Interpreter.h"
DisplayResult *Interpreter::run(string& input) {
    try {
        vector<ExpressionToken*> expressionTokens=lexicalAnalyzer.generateExpressionTokens(input);
        AbstractSyntaxTree* abstractSyntaxTree=astBuilder.buildTree(expressionTokens);
        DisplayResult*result= executeUnit.execute(abstractSyntaxTree);
        delete abstractSyntaxTree;
        return result;
    }
    catch (runtime_error error){
        return new DisplayResult(error.what());
    }
}
