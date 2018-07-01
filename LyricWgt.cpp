#include "LyricWgt.h"
#include "CcMusic.h"

extern CcMusic* g_pMusic;
LyricWgt::LyricWgt(QWidget *parent)
    : QListWidget(parent)
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFocusPolicy(Qt::NoFocus);
    setMovement(QListWidget::Static);
    setFrameShape(QListWidget::NoFrame);
}

//
LyricWgt::~LyricWgt()
{

}

void LyricWgt::mousePressEvent(QMouseEvent *e)
{
    e->ignore();
}

void LyricWgt::mouseDoubleClickEvent(QMouseEvent *e)
{
    e->ignore();
}

void LyricWgt::keyPressEvent(QKeyEvent *e)
{
    g_pMusic->keyPressEvent(e);
}

