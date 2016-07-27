#include "qconsolewidget.h"
#include <QKeyEvent>
#include <QDebug>

QConsoleWidget::QConsoleWidget(QWidget *parent) : QTextEdit(parent)
{
    setUndoRedoEnabled(false);

    //setTextColor(QColor("white"));

    /*QPalette p = this->palette();
    p.setColor(QPalette::Base, QColor("#D35800"));
    this->setPalette(p);*/

    fixedPosition = 0;
    kernelInterface=GUI_KernelBridge::getInstance();
    connect(kernelInterface, SIGNAL(OnKernelStdOutWrite(QString)), this, SLOT(OnKernelStdOutWrite(QString)));
    connect(kernelInterface, SIGNAL(OnKernelStarted()), this, SLOT(OnKernelStarted()));
    connect(kernelInterface, SIGNAL(OnKernelStdErrWrite(QString)), this, SLOT(OnKernelStdErrWrite(QString)));
    connect(kernelInterface, SIGNAL(OnKernelTerminate()), this, SLOT(OnKernelTerminate()));
    kernelInterface->startKernel();
}

QConsoleWidget::~QConsoleWidget()
{
    delete kernelInterface;
}

void QConsoleWidget::printTitle(){
    insertPlainText("[MathBoard v1.0]\nType \"help\", \"copyright\" for more information\n>>>");
    fixedPosition = textCursor().position();
}
void QConsoleWidget::printHelp(){
    insertPlainText("\nWelcome to use MathBoard, you can type expressions.\n Type\"clear\" to clear the screen \n\n>>>");
    fixedPosition = textCursor().position();
}

void QConsoleWidget::OnKernelStarted()
{
    this->printTitle();
}

void QConsoleWidget::OnKernelStdOutWrite(QString szOutput)
{
#ifdef Q_OS_WIN32
    QString content;

    if (fixedPosition != 0) {
        content = szOutput.right(szOutput.count() - szOutput.indexOf('\n') - 1);
    } else {    
        content = szOutput;
    }

    if (content.startsWith("\f\r\n")) {
        clear();
        insertPlainText(content.right(content.count() - 3));
    } else {
        insertPlainText(content);
    }
#else
    insertPlainText("\n"+szOutput+"\n>>>");
#endif
    fixedPosition = textCursor().position();
}

void QConsoleWidget::OnKernelStdErrWrite(QString szOutput)
{
    append(szOutput);
    fixedPosition = textCursor().position();
}

void QConsoleWidget::OnKernelTerminate()
{
    //exit(1);
}

void QConsoleWidget::keyPressEvent(QKeyEvent *event)
{
    bool accept;
    int key = event->key();
    if (key == Qt::Key_Backspace || event->key() == Qt::Key_Left) {
        accept = textCursor().position() > fixedPosition;
    }
    else if (key == Qt::Key_Return) {
        accept = false;
        int count = toPlainText().count() - fixedPosition;
        QString cmd = toPlainText().right(count);
        //压入历史记录
        this->command_history.push_back(cmd);
        history_pos=-1;
        if(cmd=="clear"){
            this->clear();
            this->printTitle();
            return;
        }
        else if(cmd=="help"){
            this->printHelp();
            return;
        }
        kernelInterface->inputExpression(cmd);
//        qDebug()<<cmd << "\n";
    }
    else if (key == Qt::Key_Up) {
        //返回历史
        //清除当前行
        QTextCursor storeCursorPos = textCursor();
        moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
        moveCursor(QTextCursor::StartOfLine, QTextCursor::MoveAnchor);
        moveCursor(QTextCursor::End, QTextCursor::KeepAnchor);
        textCursor().removeSelectedText();
        textCursor().deletePreviousChar();
        setTextCursor(storeCursorPos);
        QString temp_cmd;
        if(!command_history.empty()){
            if(this->history_pos==-1){
                temp_cmd=this->command_history.back();
                this->history_pos=command_history.size()-1;
            }
            else{
                if(history_pos>0){
                    history_pos-=1;
                }
                temp_cmd=this->command_history[history_pos];
            }
            insertPlainText("\n>>>"+temp_cmd);
            fixedPosition=textCursor().position();
        }
        accept = false;
    }
    else if (key == Qt::Key_Down) {
        if(history_pos!=-1){
            //清除当前行
            QTextCursor storeCursorPos = textCursor();
            moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
            moveCursor(QTextCursor::StartOfLine, QTextCursor::MoveAnchor);
            moveCursor(QTextCursor::End, QTextCursor::KeepAnchor);
            textCursor().removeSelectedText();
            textCursor().deletePreviousChar();
            setTextCursor(storeCursorPos);
            QString temp_cmd;
            if(history_pos < command_history.size()-2){
                history_pos+=1;
            }
            temp_cmd=this->command_history[history_pos];
            insertPlainText("\n>>>"+temp_cmd);
            fixedPosition=textCursor().position();
        }

        accept = false;
    }
    else {
        accept = textCursor().position() >= fixedPosition;
    }

    if (accept) {
        QTextEdit::keyPressEvent(event);
    }
}

void QConsoleWidget::cursorPositionChanged()
{
    if (textCursor().position() < fixedPosition) {
        textCursor().setPosition(fixedPosition);
    }
}
