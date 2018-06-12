#include "PlayerBody.h"
#include "CcMusic.h"
#include <QMediaPlayer>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QVideoWidget>
#include <QPalette>

#define DEFAULT_IMAGE_PATH "../Data/Image/default.png"

extern CcMusic* g_pMusic;
PlayerBody::PlayerBody(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout* layout = new QHBoxLayout;
    QVideoWidget *videow = new QVideoWidget();
    layout->addWidget(videow);
    // 设置属性
    videow->setAspectRatioMode(Qt::IgnoreAspectRatio);
    //videow->setAutoFillBackground(true);
    QPalette* palette = new QPalette();
    palette->setBrush(QPalette::Background, Qt::black);
    videow->setPalette(*palette);
    delete palette;
    //
    setLayout(layout);
    g_pMusic->GetPlayer()->setVideoOutput(videow);
}

PlayerBody::~PlayerBody()
{

}

void PlayerBody::ShowMusicInfo(const QStringList &info, const QImage &img)
{
    if(info.contains("VideoFrameRate"))
    {
        m_musicImage = QImage();
        m_strMusicTitle = QString();
    }
    else
    {
        if(info.contains("ThumbnailImage"))
        {
            m_musicImage = img;
        }
        else
        {
            m_musicImage.load(DEFAULT_IMAGE_PATH);
        }

        // title
        if(info.contains("Title"))
        {
            m_strMusicTitle = g_pMusic->GetPlayer()->metaData("Title").toString();
        }
    }

    //
    update();
}

void PlayerBody::paintEvent(QPaintEvent *e)
{
    QPainter painter;

    painter.begin(this);

    QFont font = painter.font();
    font.setPixelSize(25);
    font.setBold(true);
    painter.setFont(font);
    // pic
    painter.drawPixmap(50 , (height() - 100) / 2, 100, 100, QPixmap::fromImage(m_musicImage));
    //
    painter.drawText(QRect(150, (height() - 50) / 2, width() - 150, 50), Qt::AlignCenter, m_strMusicTitle);

    painter.end();
    return QWidget::paintEvent(e);
}

