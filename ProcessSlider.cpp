#include "ProcessSlider.h"
#include "CcMusic.h"
#include <QMediaPlayer>

extern CcMusic* g_pMusic;
ProcessSlider::ProcessSlider(Qt::Orientation orientation, QWidget *parent)
    : QSlider(orientation, parent)
{

}

ProcessSlider::~ProcessSlider()
{

}

void ProcessSlider::mousePressEvent(QMouseEvent *e)
{
    g_pMusic->disConnectPlayerSignal();
    return QSlider::mousePressEvent(e);
}

void ProcessSlider::mouseReleaseEvent(QMouseEvent *e)
{
    g_pMusic->GetPlayer()->setPosition(this->value());
    g_pMusic->ConnectPlayerSignal();
    return QSlider::mouseReleaseEvent(e);
}

