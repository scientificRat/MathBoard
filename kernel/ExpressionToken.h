//
//  ExpressionToken.h
//  MathBoard(ConsoleVersion)
//
//  Created by 黄正跃 on 7/23/16.
//  Copyright © 2016 黄正跃. All rights reserved.
//

#ifndef ExpressionToken_h
#define ExpressionToken_h
#include <iostream>

using namespace std;
enum class DataType{
    NUM_TYPE,
    OPERATOR_TYPE,
    VARIABLE_TYPE,
    FUNC_TYPE
};

class ExpressionToken {
private:
    string str_data;
    double num_data;
    DataType type;
    int pairOperatorPos;
    
    //private constructors
    ExpressionToken(double num)
    :str_data(""),
    num_data(num),
    type(DataType::NUM_TYPE){}
    
    ExpressionToken(string str,DataType _type)
    :str_data(str),
    num_data(0),
    type(_type){}
    
public:
    
    static ExpressionToken* createNumToken(double num){
        return new ExpressionToken(num);
    }
    
    static ExpressionToken* createOperatorToken(string op){
        return new ExpressionToken(op,DataType::OPERATOR_TYPE);
    }
    
    static ExpressionToken* createFunctionToken(string func){
        return new ExpressionToken(func,DataType::FUNC_TYPE);
    }
    
    static ExpressionToken* createVariableToken(string var){
        return new ExpressionToken(var,DataType::VARIABLE_TYPE);
    }
    
    DataType getDataType(){
        return type;
    }
    
    string getPrintableDateType();
    
    string getPrintableData();
    
    double getNumData(){
        if (this->type!=DataType::NUM_TYPE) {
            throw runtime_error("非数值类型");
        }
        return num_data;
    }
    
    string getStrData(){
        if(this->type==DataType::NUM_TYPE){
            throw runtime_error("数值类型");
        }
        return str_data;
    }
    
    int getPairOperatorPos() const {
        return pairOperatorPos;
    }
    
    void setPairOperatorPos(int pairOperatorPos) {
        ExpressionToken::pairOperatorPos = pairOperatorPos;
    }
};

#endif /* ExpressionToken_h */
