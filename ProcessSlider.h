#ifndef PROCESSSLIDER_H
#define PROCESSSLIDER_H

#include <QSlider>

class ProcessSlider : public QSlider
{
    Q_OBJECT
public:
    explicit ProcessSlider(Qt::Orientation orientation, QWidget *parent = 0);
    ~ProcessSlider();

protected:
    void mousePressEvent(QMouseEvent* e);
    void mouseReleaseEvent(QMouseEvent* e);

};

#endif // PROCESSSLIDER_H
