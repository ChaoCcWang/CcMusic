#include "SongPicture.h"
#include <QPainter>

SongPicture::SongPicture(QWidget *parent) : QWidget(parent)
{
    setFixedSize(120, 120);
}

SongPicture::~SongPicture()
{

}

void SongPicture::UpdateImage(const QImage &img)
{
    m_musicImage = img;
    update();
}

void SongPicture::paintEvent(QPaintEvent *e)
{
    QPainter painter;
    painter.begin(this);
    painter.drawPixmap(0, 0, 120, 120, QPixmap::fromImage(m_musicImage));
    painter.end();
    return QWidget::paintEvent(e);
}

