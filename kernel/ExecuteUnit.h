//
//  ExecuteUnit.h
//  MathBoard(ConsoleVersion)
//
//  Created by 黄正跃 on 7/25/16.
//  Copyright © 2016 黄正跃. All rights reserved.
//

#ifndef ExecuteUnit_h
#define ExecuteUnit_h
#include "AbstractSyntaxTree.h"
#include "DisplayResult.h"
#include <unordered_map>
namespace execute_core_data_struct {
    
    enum class ResultType{
        DOUBLE,
        POINTS,//vector<Point2D*> *
        MATRIX,
        STRING,
        TUPLE //Tuple*
    };
    
    //一般用于储存函数参数列表
    struct Tuple{
    private:
        vector<ASTNode*> parameters;
    public:
        void append(ASTNode* parameter){
            parameters.push_back(parameter);
        }
        int size(){
            return (int)parameters.size();
        }
        ASTNode* operator[](int index){
            return parameters[index];
        }
        virtual ~Tuple() {
            for(int i=0;i<parameters.size();i++){
                delete parameters[i];
            }
        }
    };
    
    struct ExecutionResult{
    private:
        ResultType type;
        void* data;
        vector<ExecutionResult*> tuple;
    public:
        
        ExecutionResult(const ResultType &type, void *data) : type(type), data(data) { }
        const ResultType &getType() const {
            return type;
        }
        
        void setType(const ResultType &type) {
            ExecutionResult::type = type;
        }
        
        template <typename T>
        void setData(T *data) {
            ExecutionResult::data = (void*)data;
        }
        
        void *getData() const {
            return data;
        }
        virtual ~ExecutionResult() {
            switch (type){
                case ResultType::DOUBLE:{
                    delete (double*) data;
                    break;
                }
                case ResultType::TUPLE:{
                    delete (Tuple*) data;
                    break;
                }
                case ResultType::MATRIX:{
                    
                    break;
                }
                case ResultType::POINTS:{
//                    vector<Point2D*> * points=(vector<Point2D*> *) data;
//                    for(int i=0;i<points->size();i++){
//                        delete (*points)[i];
//                    }
//                    delete points;
                    break;
                }
                case ResultType::STRING:{
                    delete (string*) data;
                    break;
                }
            }
        }
    };
}

using namespace execute_core_data_struct;

class ExecuteUnit {
private:
    //a hash table to maintain the current var table
    //!警告,现在假设所有变量都是全局的,因此只用这个哈希表存
    unordered_map<string,double> current_variable_table;
    
    //    double calcHelp(ASTNode* node);
    ExecutionResult* exeHelp(ASTNode* node);
public:
    DisplayResult* execute(AbstractSyntaxTree* abstractSyntaxTree);
    
};



#endif /* ExecuteUnit_h */
