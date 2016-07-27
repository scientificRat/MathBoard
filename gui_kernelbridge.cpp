#include "gui_kernelbridge.h"
GUI_KernelBridge *GUI_KernelBridge::sigleInstance=nullptr;
GUI_KernelBridge::GUI_KernelBridge(QObject *parent) : QObject(parent)
{

}


void GUI_KernelBridge::inputExpression(QString inExpression){
    std::string stdExpression=inExpression.toStdString();
    static DisplayResult* rst=nullptr;
    if(rst!=nullptr){
        delete rst;//清除上一次的运算结果
    }
    rst=interpreter.run(stdExpression);
    QString stdOutAns = QString::fromStdString(rst->getTextRst());
    //发送显示文本
    emit OnKernelStdOutWrite(stdOutAns);
    //发送显示图像
    if(rst->getGraphicPoints()!=nullptr){
        emit OnKernelGraphicOutWrite(rst->getGraphicPoints());
    }
}
