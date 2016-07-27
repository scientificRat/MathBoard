#ifndef QCONSOLEWIDGET_H
#define QCONSOLEWIDGET_H

#include <QTextEdit>
#include <QDir>
#include <qvector.h>
#include "gui_kernelbridge.h"

class QConsoleWidget : public QTextEdit
{
    Q_OBJECT
public:
    QConsoleWidget(QWidget *parent = 0);
    ~QConsoleWidget();
private:
    //此位置之后才能填写东西
    int fixedPosition;
    //历史记录
    QVector<QString> command_history;
    int history_pos=-1;//负一表示从最后一个位置取历史记录

    GUI_KernelBridge* kernelInterface=nullptr;
    //打印欢迎信息
    void printTitle();
    //打印帮助文档
    void printHelp();

protected:
    void keyPressEvent (QKeyEvent * event);
public slots:
    void OnKernelStarted();
    void OnKernelStdOutWrite(QString stdOutput);
    void OnKernelStdErrWrite(QString stdOutput);
    void OnKernelTerminate();

    void cursorPositionChanged();
};

#endif // QCONSOLEWIDGET_H
