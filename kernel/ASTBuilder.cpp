//
//  ASTBuilder.cpp
//  MathBoard(ConsoleVersion)
//
//  Created by 黄正跃 on 7/25/16.
//  Copyright © 2016 黄正跃. All rights reserved.
//


#include "ASTBuilder.h"
#include <stack>

AbstractSyntaxTree *ASTBuilder::buildTree(vector<ExpressionToken *>& expressionTokens) {
    //identify paired brackets
    this->FindPairedBrackets(expressionTokens);
    //run,
    return new AbstractSyntaxTree(this->buildTreeHelp(expressionTokens,0,expressionTokens.size()-1));
}

//获取操作符优先级
int ASTBuilder::getPriority(string op) {
    if(op=="=") return -2;
    if(op==",") return -1;
    if(op=="+"|| op=="-") return 0;
    if(op=="*"|| op=="/") return 1;
    if(op=="^") return 2;
    throw runtime_error("不支持的操作符");
}
//找到匹配的括号
vector<ExpressionToken *> &ASTBuilder::FindPairedBrackets(vector<ExpressionToken *> &expressionTokens) {
    stack<int> bracket_stack;
    for(int i=0;i<expressionTokens.size();i++){
        if(expressionTokens[i]->getDataType()!=DataType::OPERATOR_TYPE) continue;
        if ( expressionTokens[i]->getStrData()=="("){
            bracket_stack.push(i);
        }
        else if(expressionTokens[i]->getStrData()==")"){
            if (bracket_stack.empty()){
                throw runtime_error("括号不匹配");
            }
            int temp_pos=bracket_stack.top();
            expressionTokens[temp_pos]->setPairOperatorPos(i);
            bracket_stack.pop();
        }
    }
    if(!bracket_stack.empty()) throw runtime_error("括号不匹配");
    return expressionTokens;
}

ASTNode* ASTBuilder::buildTreeHelp(vector<ExpressionToken *> &expressionTokens, int start,int end) {
    stack<ASTNode*> rst_stack;
    ASTNode* tempNumNode= nullptr;
    ASTNode* new_op= nullptr;
    int priority_new_op;
    ASTNode* op= nullptr;
    int priority_op;
    int i;
    for (i=start;i<=end;i++){
        if(expressionTokens[i]->getDataType()==DataType::NUM_TYPE || expressionTokens[i]->getDataType()==DataType::VARIABLE_TYPE){
            tempNumNode=new ASTNode(expressionTokens[i]);
        }
        else if(expressionTokens[i]->getDataType()==DataType::FUNC_TYPE){
            if(expressionTokens[i+1]->getDataType()!=DataType::OPERATOR_TYPE || expressionTokens[i+1]->getStrData()!="("){
                throw runtime_error("语法错误");
            }
            int pairedPos=expressionTokens[i+1]->getPairOperatorPos();
            ASTNode* innerNode=buildTreeHelp(expressionTokens,i+2,pairedPos-1);
            ASTNode* funcNode=new ASTNode(expressionTokens[i]);
            funcNode->addChild(innerNode);
            tempNumNode=funcNode;
            i = pairedPos;
            continue;
        }
        else{
            // deal with brackets
            if(expressionTokens[i]->getStrData()=="("){
                int pairedPos=expressionTokens[i]->getPairOperatorPos();
                ASTNode* innerNode=buildTreeHelp(expressionTokens,i+1,pairedPos-1);
                tempNumNode=innerNode;
                i = pairedPos;
                continue;
            }
            //build Tree
            new_op= new ASTNode(expressionTokens[i]);
            priority_new_op = this->getPriority(expressionTokens[i]->getStrData());
            op= nullptr;
            if(rst_stack.empty()){
                priority_op=-1000;
            }
            else{
                op = rst_stack.top();
                priority_op = getPriority(op->getExpressionToken()->getStrData());
            }
            if(priority_new_op > priority_op){
                if(tempNumNode == nullptr) throw runtime_error("语法错误");
                new_op->addChild(tempNumNode);
                tempNumNode= nullptr;
                rst_stack.push(new_op);
            }
            else{
                if(op== nullptr) throw runtime_error("!错误");
                if(tempNumNode == nullptr) throw runtime_error("语法错误");
                op->addChild(tempNumNode);
                tempNumNode= nullptr;
                //merge
                while(getPriority(op->getExpressionToken()->getStrData())>priority_new_op && rst_stack.size()>1){
                    ASTNode* top_node= rst_stack.top();
                    rst_stack.pop();
                    rst_stack.top()->addChild(top_node);
                    op =rst_stack.top();
                }
                //add operator node
                string new_op_str=new_op->getExpressionToken()->getStrData();
                string op_str=op->getExpressionToken()->getStrData();
                if(new_op_str!=op_str){
                    new_op->addChild(rst_stack.top());
                    rst_stack.pop();
                    rst_stack.push(new_op);
                }
                else{
                    delete new_op;
                }
            }
        }
    }
    if(tempNumNode == nullptr) throw runtime_error("语法错误");
    // 处理 (2) 4 这种只有一个数字的情况
    if(rst_stack.empty()){
        rst_stack.push(tempNumNode);
    }
    else{
        rst_stack.top()->addChild(tempNumNode);
    }
    //merge
    while(rst_stack.size()>1){
        ASTNode* top_node= rst_stack.top();
        rst_stack.pop();
        rst_stack.top()->addChild(top_node);
    }
    if(rst_stack.size()!=1) throw runtime_error("Syntax error");
    return rst_stack.top();
}

