#include "qpaintzone.h"
#include <QDebug>
#include <stdlib.h>
using namespace std;
QPaintZone::QPaintZone(QWidget *parent) : QWidget(parent)
{
    background = QBrush(QColor(255, 255, 255));
    framePen=QPen(Qt::black);
    framePen.setWidthF(1.3);
    curvePen=QPen(Qt::black);
    curvePen.setWidthF(1);
    textPen=QPen(Qt::black);
    textFont=QFont("Arial",1,QFont::Normal,true);
    textFont.setPixelSize(9);
    guiInterface=GUI_KernelBridge::getInstance();
    connect(guiInterface,SIGNAL(OnKernelGraphicOutWrite(vector<Point2D *>*)),this,SLOT(OnKernelGraphicOutWrite(vector<Point2D *>*)));
}

void QPaintZone::paintEvent(QPaintEvent *event){
    QPainter painter;
    this->curveAreaWidth=event->rect().width()-40;
    this->curveAreaHeight=event->rect().height()-40;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(event->rect(), background);
    painter.translate(20,20);
    //绘制边框
    painter.setPen(framePen);
    painter.drawLine(0,0,curveAreaWidth,0);
    painter.drawLine(0,0,0,curveAreaHeight);
    painter.drawLine(0,curveAreaHeight,curveAreaWidth,curveAreaHeight);
    painter.drawLine(curveAreaWidth,0,curveAreaWidth,curveAreaHeight);
    double deltax=curveAreaWidth/5.0;
    double deltay=curveAreaHeight/5.0;
    //刻度线
    for(int i=1;i<5;i++){
        //上下
        painter.drawLine(deltax*i,0,deltax*i,4);
        painter.drawLine(deltax*i,curveAreaHeight,deltax*i,curveAreaHeight-4);
        //左右
        painter.drawLine(0,deltay*i,4,deltay*i);
        painter.drawLine(curveAreaWidth,deltay*i,curveAreaWidth-4,deltay*i);

    }
//    painter.setFont(textFont);
//    for(int i=0;i<=5;i++){
//        painter.drawText(deltax*i,curveAreaHeight+15,"1.23");
//        painter.drawText(-20,deltay*i,"1.04");
//    }

    //critical section 临界区--------------------------------------------
    curve_buffer_mutex.lock();
    if(curve.size()>1){
        //绘制曲线
        painter.setPen(this->curvePen);
        for(int i=0;i<curve.size()-1;i++){
            painter.drawLine(curve[i],curve[i+1]);
        }
        //绘制坐标刻度值
        double value_delta_x = (this->lablex_max-this->lablex_min)/5;
        double value_delta_y = (this->labley_max-this->labley_min)/5;
        double format_factor_x=calcFormatFactor(value_delta_x);
        double format_factor_y=calcFormatFactor(value_delta_y);
        painter.setFont(textFont);
        for(int i=0;i<=5;i++){
            double display_x=static_cast<int>((value_delta_x*i+ lablex_min)*format_factor_x)/format_factor_x;
            double display_y=static_cast<int>((value_delta_y*i+ labley_min)*format_factor_y)/format_factor_y;
            char strx[10];
            char stry[10];
            sprintf(strx,"%g",display_x);
            sprintf(stry,"%g",display_y);
            //x-lable
            painter.drawText(deltax*i,curveAreaHeight+15,strx);
            //y-lable
            painter.drawText(-20,curveAreaHeight-deltay*i,stry);
        }



    }
    curve_buffer_mutex.unlock();
    //-------------------------------------------------------------------
    painter.end();
}

void QPaintZone::OnKernelGraphicOutWrite(vector<Point2D *> *graphicPoints){
    //critical section 临界区--------------------------------------------
    curve_buffer_mutex.lock();
    //find the domain and range
    double min_x=(*graphicPoints)[0]->getX();
    double max_x=(*graphicPoints)[graphicPoints->size()-1]->getX();
    double min_y=(*graphicPoints)[0]->getY();
    double max_y=min_y;
    for(int i=0;i<graphicPoints->size();i++){
        if((*graphicPoints)[i]->getY()>max_y){
            max_y=(*graphicPoints)[i]->getY();
        }
        if((*graphicPoints)[i]->getY()<min_y){
            min_y=(*graphicPoints)[i]->getY();
        }
    }
    //calculate the scale factor and the offset
    double margin_y=10;//y-轴方向不抵满，上下留空白
    double scale_x= (double)this->curveAreaWidth/(max_x-min_x);
    double scale_y= (double)(this->curveAreaHeight-2*margin_y)/(max_y-min_y);
    double offset_x= 0 - min_x*scale_x;
    double offset_y= 0 - min_y*scale_y+ margin_y;
    //clear the previous points
    this->curve.clear();
    //generate the points
    for(int i=0;i<graphicPoints->size();i++){
        double tempx= offset_x + scale_x*(*graphicPoints)[i]->getX();
        double tempy= offset_y + scale_y*(*graphicPoints)[i]->getY();
        curve.push_back(QPointF(tempx,this->curveAreaHeight-tempy));
    }
    //update the values
    this->lablex_max=max_x;
    this->labley_max=max_y + margin_y/scale_y;
    this->lablex_min=min_x;
    this->labley_min=min_y - margin_y/scale_y;
    curve_buffer_mutex.unlock();
    //-------------------------------------------------------------------
    this->update();
}
