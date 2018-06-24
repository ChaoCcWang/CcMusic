#include "VolumeSlider.h"
#include "CcMusic.h"

extern CcMusic* g_pMusic;
VolumeSlider::VolumeSlider(Qt::Orientation orientation, QWidget *parent)
    : QSlider(orientation, parent)
{

}

void VolumeSlider::keyPressEvent(QKeyEvent *e)
{
    g_pMusic->keyPressEvent(e);
}

