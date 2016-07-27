//
//  DisplayResult.h
//  MathBoard(ConsoleVersion)
//
//  Created by 黄正跃 on 7/26/16.
//  Copyright © 2016 黄正跃. All rights reserved.
//

#ifndef DisplayResult_h
#define DisplayResult_h
#include <iostream>
#include <vector>
using namespace std;
struct Point2D{
private:
    double x;
    double y;
public:
    Point2D(double x, double y) : x(x), y(y) { }
    
    double getX() const {
        return x;
    }
    
    void setX(double x) {
        Point2D::x = x;
    }
    
    double getY() const {
        return y;
    }
    
    void setY(double y) {
        Point2D::y = y;
    }
    
};

class DisplayResult {
private:
    string textRst;
    vector<Point2D*>* graphicPoints=nullptr;
public:
    //constructor
    
    
    DisplayResult(const string &textRst) : textRst(textRst) { }
    
    
    DisplayResult(const string &textRst, vector<Point2D*> *graphicPoints) : textRst(textRst),
    graphicPoints(graphicPoints) { }
    
    const string &getTextRst() const {
        return textRst;
    }
    
    vector<Point2D*> *getGraphicPoints() const {
        return graphicPoints;
    }
    
    virtual ~DisplayResult() {
        //没有清除每一个Point2D可能出现内存泄漏
        if(graphicPoints!= nullptr){
            for(int i=0;i<graphicPoints->size();i++){
                delete (*graphicPoints)[i];
            }
            delete graphicPoints;
        }
    }
};

#endif /* DisplayResult_h */
