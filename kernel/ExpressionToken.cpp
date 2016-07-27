//
//  ExpressionToken.cpp
//  MathBoard(ConsoleVersion)
//
//  Created by 黄正跃 on 7/23/16.
//  Copyright © 2016 黄正跃. All rights reserved.
//

#include "ExpressionToken.h"
#include <sstream>

string ExpressionToken::getPrintableDateType() {
    string ans="";
    switch (this->type) {
        case DataType::NUM_TYPE:
            ans="NUM_TYPE";
            break;
        case DataType::FUNC_TYPE:
            ans="FUNC_TYPE";
            break;
        case DataType::OPERATOR_TYPE:
            ans="OPERATOR_TYPE";
            break;
        case DataType::VARIABLE_TYPE:
            ans="VARIABLE_TYPE";
            break;
    }
    return ans;
}

string ExpressionToken::getPrintableData() {
    if(this->type==DataType::NUM_TYPE){
        stringstream out;
        out<<this->num_data;
        return out.str();
    }
    else{
        return this->str_data;
    }
}
