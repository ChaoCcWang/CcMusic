#ifndef LYRICWGT_H
#define LYRICWGT_H

#include <QListWidget>
#include <QMouseEvent>
#include <QKeyEvent>

class LyricWgt : public QListWidget
{
public:
    LyricWgt(QWidget* parent = 0);
    ~LyricWgt();

protected:
    void mousePressEvent(QMouseEvent* e);
    void mouseDoubleClickEvent(QMouseEvent* e);
    void keyPressEvent(QKeyEvent* e);
};

#endif // LYRICWGT_H
