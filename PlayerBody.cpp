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
//    videow->setAspectRatioMode(Qt::IgnoreAspectRatio);
//    //videow->setAutoFillBackground(true);
//    QPalette* palette = new QPalette();
//    palette->setBrush(QPalette::Background, Qt::black);
//    videow->setPalette(*palette);
//    delete palette;
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

void PlayerBody::UpdateUi()
{
    update();
}

void PlayerBody::paintEvent(QPaintEvent *e)
{
    QPainter painter;

    painter.begin(this);

    QFont font = painter.font();
    font.setPixelSize(20);
    font.setBold(true);
    painter.setFont(font);
    // pic
    painter.drawPixmap(50 , (height() - 100) / 2, 120, 120, QPixmap::fromImage(m_musicImage));
    // title
    painter.drawText(QRect(10, 30, (width() - 20), 50), Qt::AlignCenter, m_strMusicTitle);
    //
    font.setPixelSize(13);
    font.setBold(false);
    painter.setFont(font);
    QString strVolumn("音量: %1%");
    painter.drawText(QRect(50, (height() - 100) / 2 + 150, 100, 50), Qt::AlignLeft, strVolumn.arg(g_pMusic->GetPlayer()->volume()));
    //
    painter.end();
    return QWidget::paintEvent(e);
}

