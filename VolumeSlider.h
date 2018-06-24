#ifndef VOLUMESLIDER_H
#define VOLUMESLIDER_H

#include <QSlider>

class VolumeSlider : public QSlider
{
    Q_OBJECT

public:
    explicit VolumeSlider(Qt::Orientation orientation, QWidget *parent = 0);

protected:
    void keyPressEvent(QKeyEvent* e);
};

#endif // VOLUMESLIDER_H
