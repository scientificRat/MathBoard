#ifndef QPAINTZONE_H
#define QPAINTZONE_H

#include <QWidget>
#include <QBrush>
#include <QFont>
#include <QPen>
#include <qpainter.h>
#include <qevent.h>
#include <vector>
#include "kernel/DisplayResult.h"
#include "gui_kernelbridge.h"
using namespace std;
class QPaintZone : public QWidget
{
    Q_OBJECT
public:
    explicit QPaintZone(QWidget *parent = 0);
protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    int curveAreaWidth;
    int curveAreaHeight;
    double lablex_min;
    double labley_min;
    double lablex_max;
    double labley_max;
    GUI_KernelBridge* guiInterface=nullptr;
    QBrush background;
    QFont textFont;
    QPen framePen;
    QPen curvePen;
    QPen textPen;
    QMutex curve_buffer_mutex;
    QVector<QPointF> curve;

    double calcFormatFactor(double delta){
        double factor=1;
        while(delta<1){
            delta*=10;
            factor*=10;
        }
        factor*=10;
        return factor;
    }

public slots:
    void OnKernelGraphicOutWrite(vector<Point2D*>* graphicPoints);
};

#endif // QPAINTZONE_H
