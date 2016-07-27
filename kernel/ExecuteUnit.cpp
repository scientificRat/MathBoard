//
//  ExecuteUnit.cpp
//  MathBoard(ConsoleVersion)
//
//  Created by 黄正跃 on 7/25/16.
//  Copyright © 2016 黄正跃. All rights reserved.
//

#include "ExecuteUnit.h"
#include "math.h"
#include <random>
DisplayResult* ExecuteUnit::execute(AbstractSyntaxTree *abstractSyntaxTree) {
    //text to display
    char text_rst[128]="null";
    DisplayResult* ans= nullptr;
    //run
    ExecutionResult* result=exeHelp(abstractSyntaxTree->getRoot());
    //output
    switch (result->getType()){
        case ResultType::DOUBLE:{
            sprintf(text_rst,"%g",*((double*)result->getData()));
            ans=new DisplayResult(text_rst);
            break;
        }
        case ResultType::POINTS:{
            sprintf(text_rst,"如图所示");
            ans=new DisplayResult(text_rst, static_cast<vector<Point2D*>* >(result->getData()));
            break;
        }
        case ResultType::MATRIX:{
            //
            break;
        }
        case ResultType::STRING:{
            //
            break;
        }
        default:{
            break;
        }
    }
    delete result;
    return ans;
}

ExecutionResult* ExecuteUnit::exeHelp(ASTNode* node) {
    //random engine, it will be used when random function is called
    static default_random_engine randEngine(time(0));
    //node expression token
    ExpressionToken* expressionToken=node->getExpressionToken();
    //node children
    vector<ASTNode*> children=node->getChildren();
    
    switch (expressionToken->getDataType()){
        case DataType::NUM_TYPE: {
            //数字节点不能有子节点
            if (!children.empty()) throw runtime_error("语法错误");
            return new ExecutionResult(ResultType::DOUBLE,
                                       static_cast<void *>(new double(expressionToken->getNumData())));
        }
        case DataType::VARIABLE_TYPE: {
            //变量节点不能有子节点
            if (!children.empty()) throw runtime_error("语法错误");
            string var = expressionToken->getStrData();
            if (current_variable_table.count(var) != 1) throw runtime_error("语法错误");
            return new ExecutionResult(ResultType::DOUBLE,
                                       static_cast<void *>(new double(current_variable_table[var])));
        }
        case DataType::OPERATOR_TYPE: {
            //操作符一定关联两个以上节点－－－－－－－没有考虑阶乘
            if (children.size() <= 1) throw runtime_error("语法错误");
            string op = expressionToken->getStrData();//the current operator
            //if it's a comma, then return a tuple(represent a list for function parameters)
            if (op == ",") {
                Tuple *parameter_list = new Tuple;
                for (int i = 0; i < children.size(); i++) {
                    parameter_list->append(children[i]);
                }
                return new ExecutionResult(ResultType::TUPLE, parameter_list);
            }
            if (op == "="){
                if(children.size()!=2) throw runtime_error("赋值语法错误");
                if(children[0]->getExpressionToken()->getDataType()!=DataType::VARIABLE_TYPE) throw runtime_error("语法错误,只能对变量赋值");
                ExecutionResult *right_rst = exeHelp(children[1]);
                if (right_rst->getType() != ResultType::DOUBLE) throw runtime_error("语法错误");
                //更新变量表
                double temp_value=*(double*)right_rst->getData();
                this->current_variable_table[children[0]->getExpressionToken()->getStrData()]=temp_value;
                delete right_rst;
                return new ExecutionResult(ResultType::DOUBLE, static_cast<void *>(new double(temp_value)));

            }
            //if it's a math sign
            //calculate all the child-nodes that belong to this operator
            //push the result to the double-value vector children_value
            vector<double> children_value;
            for (int i = 0; i < children.size(); i++) {
                DataType dataType = children[i]->getExpressionToken()->getDataType();
                ExecutionResult *temp_rst = exeHelp(children[i]);
                if (temp_rst->getType() != ResultType::DOUBLE) throw runtime_error("语法错误");
                children_value.push_back(*((double *) temp_rst->getData()));
                //clear the memory
                delete temp_rst;
            }
            //calculate
            double temp_ans = 0;
            if (op == "+") {
                temp_ans = children_value[0];
                for (int i = 1; i < children_value.size(); i++) {
                    temp_ans += children_value[i];
                }
            }
            else if (op == "-") {
                temp_ans = children_value[0];
                for (int i = 1; i < children_value.size(); i++) {
                    temp_ans -= children_value[i];
                }
            }
            else if (op == "*") {
                temp_ans = children_value[0];
                for (int i = 1; i < children_value.size(); i++) {
                    temp_ans *= children_value[i];
                }
            }
            else if (op == "/") {
                temp_ans = children_value[0];
                for (int i = 1; i < children_value.size(); i++) {
                    temp_ans /= children_value[i];
                }
            }
            else if (op == "^") {
                temp_ans = children_value[0];
                for (int i = 1; i < children_value.size(); i++) {
                    temp_ans = pow(temp_ans, children_value[i]);
                }
            }
            return new ExecutionResult(ResultType::DOUBLE, static_cast<void *>(new double(temp_ans)));
        }
        case DataType::FUNC_TYPE: {
            
            if (children.size() != 1) throw runtime_error("语法错误");
            string func = expressionToken->getStrData();
            ExecutionResult *inner_func_rst = this->exeHelp(children[0]);
            // simple functions
            if (inner_func_rst->getType() == ResultType::DOUBLE) {
                double data = *((double *) inner_func_rst->getData());
                //clear memory
                delete inner_func_rst;
                if (func == "sin") return new ExecutionResult(ResultType::DOUBLE, new double(sin(data)));
                if (func == "cos") return new ExecutionResult(ResultType::DOUBLE, new double(cos(data)));
                if (func == "tan") return new ExecutionResult(ResultType::DOUBLE, new double(tan(data)));
                if (func == "cot") return new ExecutionResult(ResultType::DOUBLE, new double(1 / tan(data)));
                if (func == "arcsin") return new ExecutionResult(ResultType::DOUBLE, new double(asin(data)));
                if (func == "arccos") return new ExecutionResult(ResultType::DOUBLE, new double(acos(data)));
                if (func == "arctan") return new ExecutionResult(ResultType::DOUBLE, new double(atan(data)));
                if (func == "arccot") return new ExecutionResult(ResultType::DOUBLE, new double(atan(1 / data)));
                if (func == "ln") return new ExecutionResult(ResultType::DOUBLE, new double(log(data)));
                throw runtime_error("语法错误");
            }
            // complex functions
            else if (inner_func_rst->getType() == ResultType::TUPLE) {
                Tuple *parameters = (Tuple *) inner_func_rst->getData();
                if (func == "log") {
                    if (parameters->size() != 2) {
                        throw runtime_error("log函数要求为两个参数 底数和真数");
                    }
                    ExecutionResult *rst1 = exeHelp((*parameters)[0]);
                    ExecutionResult *rst2 = exeHelp((*parameters)[1]);
                    if (rst1->getType() != ResultType::DOUBLE || rst2->getType() != ResultType::DOUBLE) {
                        throw runtime_error("语法错误");
                    }
                    double data1 = *((double *) rst1->getData());
                    double data2 = *((double *) rst2->getData());
                    return new ExecutionResult(ResultType::DOUBLE, new double(log(data2) / log(data1)));
                    
                }
                else if (func == "random") {
                    if (parameters->size() != 2) {
                        throw runtime_error("random函数要求为两个参数 x1,x2 表示随机数取值范围(x1,x2)");
                    }
                    ExecutionResult *rst1 = exeHelp((*parameters)[0]);
                    ExecutionResult *rst2 = exeHelp((*parameters)[1]);
                    if (rst1->getType() != ResultType::DOUBLE || rst2->getType() != ResultType::DOUBLE) {
                        throw runtime_error("语法错误");
                    }
                    double data1 = *((double *) rst1->getData());
                    double data2 = *((double *) rst2->getData());
                    if (data1 >= data2) {
                        throw runtime_error("语法错误,随机数取值范围错误");
                    }
                    uniform_real_distribution<double> random_real(data1, data2);
                    delete rst1;
                    delete rst2;
                    return new ExecutionResult(ResultType::DOUBLE, new double(random_real(randEngine)));
                }
                else if (func == "integral") {
                    //积分
                    
                }
                else if (func == "diff") {
                    //微分
                }
                else if (func == "sum") {
                    //求和
                }
                else if (func == "plot") {
                    //画图
                    if (parameters->size() != 5) {
                        throw runtime_error("plot函数要求为5个参数 plot(<function>, <var>, start, step, end)");
                    }
                    
                    ExecutionResult *rst_start = exeHelp((*parameters)[2]);
                    ExecutionResult *rst_step = exeHelp((*parameters)[3]);
                    ExecutionResult *rst_end = exeHelp((*parameters)[4]);
                    //检查语法
                    if((*parameters)[1]->getExpressionToken()->getDataType()!=DataType::VARIABLE_TYPE) throw runtime_error("语法错误");
                    if(rst_start->getType()!=ResultType::DOUBLE || rst_step->getType()!=ResultType::DOUBLE || rst_end->getType()!=ResultType::DOUBLE) throw runtime_error("语法错误");
                    //提取变量名
                    string var = (*parameters)[1]->getExpressionToken()->getStrData();
                    //提取常量
                    double start= *(double*)rst_start->getData();
                    double step= *(double*)rst_step->getData();
                    double end= *(double*)rst_end->getData();
                    //开始计算
                    vector<Point2D*> *curve=new vector<Point2D*>();
                    //检查第一个表达式是否合法
                    this->current_variable_table[var]=start;
                    ExecutionResult* temp_rst=exeHelp((*parameters)[0]);
                    if(temp_rst->getType()!=ResultType::DOUBLE) throw runtime_error("语法错误");
                    curve->push_back(new Point2D(start,*(double*)temp_rst->getData()));
                    delete temp_rst;
                    //循环遍历所有点
                    for(double x=start+step; x<end; x+=step){
                        this->current_variable_table[var]=x;
                        temp_rst=exeHelp((*parameters)[0]);
                        curve->push_back(new Point2D(x,*(double*)temp_rst->getData()));
                        delete temp_rst;
                    }
                    return new ExecutionResult(ResultType::POINTS,curve);
                }
                throw runtime_error("语法错误");
            }
        }
    
    }//end switch
    throw runtime_error("未知错误");
}

