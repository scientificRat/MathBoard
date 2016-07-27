//
//  ASTBuilder.h
//  MathBoard(ConsoleVersion)
//
//  Created by 黄正跃 on 7/25/16.
//  Copyright © 2016 黄正跃. All rights reserved.
//

#ifndef ASTBuilder_h
#define ASTBuilder_h
#include <string>
#include <vector>
#include "ExpressionToken.h"
#include "AbstractSyntaxTree.h"
using namespace std;
// abstract syntax tree builder
class ASTBuilder {
private:
    int getPriority(string op);
    
    vector<ExpressionToken*>& FindPairedBrackets(vector<ExpressionToken*>& expressionTokens);
    
    ASTNode* buildTreeHelp(vector<ExpressionToken*>& expressionTokens,int start, int end);
    
public:
    AbstractSyntaxTree* buildTree(vector<ExpressionToken*>& expressionTokens);
};

#endif /* ASTBuilder_h */
