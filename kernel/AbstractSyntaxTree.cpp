//
//  AbstractSyntaxTree.cpp
//  MathBoard(ConsoleVersion)
//
//  Created by 黄正跃 on 7/23/16.
//  Copyright © 2016 黄正跃. All rights reserved.
//

#include "AbstractSyntaxTree.h"
void AbstractSyntaxTree::printTreeInConsole() {
    cout<<root->getExpressionToken()->getPrintableData()<<"";
    printTreeHelp(this->root,0);
}

void AbstractSyntaxTree::printTreeHelp(ASTNode *root, int deep) {
    for(int i=0;i<deep;i++){
        cout<<" ";
    }
    cout<<endl;
    //    cout<<root->getExpressionToken()->getPrintableData()<<"  \n";
    for(vector<ASTNode*>::const_iterator it=root->getChildren().begin();it<root->getChildren().end();it++){
        for(int i=0;i<deep+1;i++){
            cout<<" ";
        }
        cout<<" "<<(*it)->getExpressionToken()->getPrintableData();
        if((*it)->isLeaf()){
            cout<<"\n";
        }
        else{
            printTreeHelp((*it),deep+1);
        }
    }
}