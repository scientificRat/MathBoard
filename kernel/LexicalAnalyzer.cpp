//
//  LexicalAnalyzer.cpp
//  MathBoard(ConsoleVersion)
//
//  Created by 黄正跃 on 7/23/16.
//  Copyright © 2016 黄正跃. All rights reserved.
//

#include "LexicalAnalyzer.h"

bool LexicalAnalyzer::isCharInArray(char c,const char *array){
    int length = 14;
    for(int i=0;i<length;i++){
        if(c==array[i]){
            return true;
        }
    }
    return false;
}
bool LexicalAnalyzer::isOperatorChar(char c){
    return this->isCharInArray(c, this->operators);
}
vector<ExpressionToken*> LexicalAnalyzer::generateExpressionTokens(string input){
    vector<ExpressionToken*> ans;
    //finite-state machine
    //有限状态机
    const int ON_BEGIN=-1;
    const int ON_OP=1;
    const int ON_VAR=2;
    const int ON_PURE_NUM=3;
    const int ON_NUM_POINT=4;
    const int ON_NUM_MINUS=5;
    int count_points=0;
    int startPos=0;
    int state = ON_BEGIN;
    int i=0;
    for(i=0;i<input.length();i++){
        //current is an operator当前字符为操作符
        if(this->isOperatorChar(input[i])){
            //identify the negative sign
            if (input[i]=='-'){
                if(state==ON_BEGIN){
                    startPos=i;
                    state=ON_NUM_MINUS;
                    continue;
                }
                // if it's an operator before except')' ,the '-'should be a negative sign rather than a subtract operator
                else if(state== ON_OP && input[i-1]!=')'){
                    startPos=i;
                    state=ON_NUM_MINUS;
                    continue;
                }
                else if(state!=ON_VAR && state!=ON_PURE_NUM && input[i-1]!=')'){
                    throw runtime_error("语法错误");
                }
            }
            if(state==ON_VAR){
                string var=input.substr(startPos,i-startPos);
                if(this->isVarFunction(var)){
                    if(input[i]!='(') throw runtime_error(var+" 为系统保留关键词");
                    ans.push_back(ExpressionToken::createFunctionToken(var));
                }
                else{
                    ans.push_back(ExpressionToken::createVariableToken(var));
                    if(input[i]=='('){
                        ans.push_back(ExpressionToken::createOperatorToken("*"));
                    }
                }
            }
            else if (state==ON_PURE_NUM){
                ans.push_back(ExpressionToken::createNumToken(atof(input.substr(startPos,i-startPos).c_str())));
                //deal condition like 2(3+4)-->2*(3+4)
                if(input[i]=='('){
                    ans.push_back(ExpressionToken::createOperatorToken("*"));
                }
            }
            else if (state==ON_NUM_MINUS && input[i]=='('){
                ans.push_back(ExpressionToken::createNumToken(-1));
                ans.push_back(ExpressionToken::createOperatorToken("*"));
            }
            else if (state==ON_BEGIN &&input[i]=='('){
                //
            }
            else if(state!=ON_OP){
                throw runtime_error("语法错误");
            }
            ans.push_back(ExpressionToken::createOperatorToken(input.substr(i,1)));
            state=ON_OP;
        }
        //current is pure num
        else if((input[i]>='0' &&input[i]<='9')){
            if(state==ON_BEGIN){
                startPos=i;
                count_points=0;
            }
            else if(state==ON_OP && input[i-1]!=')'){
                startPos=i;
                count_points = 0;
            }
            else if (input[i-1]==')'|| state==ON_VAR){
                throw runtime_error("语法错误");
            }
            state=ON_PURE_NUM;
            
        }
        else if(input[i]=='.'){
            if(count_points>0){
                throw runtime_error("小数点数过多");
            }
            count_points++;
            if(state==ON_BEGIN){
                startPos=i;
            }
            else if (state==ON_OP && input[i-1]!=')'){
                startPos=i;
            }
            else if(state!=ON_PURE_NUM && state!=ON_NUM_MINUS){
                throw runtime_error("语法错误");
            }
            state=ON_NUM_POINT;
            
        }
        //other characters
        else{
            if (state==ON_BEGIN){
                startPos=i;
            }
            else if (state==ON_OP && input[i-1]!=')'){
                startPos=i;
            }
            else if (state==ON_PURE_NUM){
                ans.push_back(ExpressionToken::createNumToken(atof(input.substr(startPos,i-startPos).c_str())));
                ans.push_back(ExpressionToken::createOperatorToken("*"));
                startPos=i;
            }
            else if (state==ON_NUM_MINUS){
                ans.pop_back();
                ans.push_back(ExpressionToken::createNumToken(-1));
                ans.push_back(ExpressionToken::createOperatorToken("*"));
                startPos=i;
            }
            else if (state!=ON_VAR){
                throw runtime_error("语法错误,小数点错误");
            }
            state=ON_VAR;
        }
    }
    //at the end of the input str:
    if(state==ON_PURE_NUM){
        ans.push_back(ExpressionToken::createNumToken(atof(input.substr(startPos,i-startPos).c_str())));
    }
    else if (state==ON_VAR){
        string var=input.substr(startPos,i-startPos);
        if(this->isVarFunction(var))  throw runtime_error(var+" 为系统保留关键词");
        ans.push_back(ExpressionToken::createVariableToken(var));
    }
    else if(state==ON_BEGIN){
        throw runtime_error("空！");
    }
    else if(input[i-1]!=')'){
        throw runtime_error("语法错误");
    }
    return ans;
}

bool LexicalAnalyzer::isStringInVector(const string& s, vector<string>& array) {
    for(int i=0;i<array.size();i++){
        if (s==array[i]) return true;
    }
    return false;
}

bool LexicalAnalyzer::isVarFunction(const string& var) {
    return isStringInVector(var,this->innerFunctions);
}