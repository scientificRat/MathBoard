#ifndef GUI_KERNELBRIDGE_H
#define GUI_KERNELBRIDGE_H

#include <QObject>
#include "kernel/Interpreter.h"
#include "kernel/DisplayResult.h"
#include <iostream>
//采用桥接模式连接GUI和kernel
class GUI_KernelBridge : public QObject
{
    Q_OBJECT
public:

    static GUI_KernelBridge* getInstance(){
        if(sigleInstance==nullptr){
            sigleInstance=new GUI_KernelBridge();
        }
        return sigleInstance;
    }

    //the kernel componet
    Interpreter interpreter;
    //the interface to GUI input
    void inputExpression(QString inExpression);

    void startKernel(){
        emit OnKernelStarted();
    }

signals:
    void OnKernelStarted();
    void OnKernelStdOutWrite(QString stdOutput);
    void OnKernelStdErrWrite(QString stdOutput);
    void OnKernelGraphicOutWrite(vector<Point2D*>* graphicPoints);
    void OnKernelTerminate();
private:
    static GUI_KernelBridge *sigleInstance;
    explicit GUI_KernelBridge(QObject *parent = 0);
};

#endif // GUI_KERNELBRIDGE_H
