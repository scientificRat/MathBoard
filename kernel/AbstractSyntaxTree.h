//
//  AbstractSyntaxTree.h
//  MathBoard(ConsoleVersion)
//
//  Created by 黄正跃 on 7/23/16.
//  Copyright © 2016 黄正跃. All rights reserved.
//

#ifndef AbstractSyntaxTree_h
#define AbstractSyntaxTree_h
#include <vector>
#include "ExpressionToken.h"
//抽象语法树节点
class ASTNode{
private:
    ExpressionToken*expressionToken=nullptr;
    vector<ASTNode*> children;
    
public:
    //constructor
    ASTNode(ExpressionToken* expressionToken): expressionToken(expressionToken) { }
    
    bool isLeaf(){
        return children.empty();
    }
    
    void addChild(ASTNode *child){
        children.push_back(child);
    }
    
    //getters
    ExpressionToken *getExpressionToken() const {
        return expressionToken;
    }
    
    const vector<ASTNode *> &getChildren() const {
        return children;
    }
    //destructor
    ~ASTNode(){
        if(expressionToken!=nullptr){
            delete expressionToken;
        }
        for(vector<ASTNode*>::iterator it=children.begin();it<children.end();it++){
            delete (*it);
        }
        children.clear();
    }
};
//抽象语法树
class AbstractSyntaxTree {
private:
    
    ASTNode* root;
    void printTreeHelp(ASTNode* root,int deep);
public:
    //constructor
    AbstractSyntaxTree(ASTNode *root) : root(root) { }
    
    ASTNode *getRoot() const {
        return root;
    }
    
    void printTreeInConsole();
    
    ~AbstractSyntaxTree(){
        delete root;
    }
    
};

#endif /* AbstractSyntaxTree_h */
