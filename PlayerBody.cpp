#include "PlayerBody.h"
#include "CcMusic.h"
#include <QMediaPlayer>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QFile>
#include <QDir>
#include <QTime>
#include <QDebug>
#include <QTimer>
#include "LyricWgt.h"
#include "SongPicture.h"

#define DEFAULT_IMAGE_PATH "../Data/Image/default.png"
#define LYRIC_DIR_PATH   "../Data/Lyric"
#define FROM_LOCAL(str)  QString::fromLocal8Bit(str).remove("\n").remove("\r").trimmed()

extern CcMusic* g_pMusic;
PlayerBody::PlayerBody(QWidget *parent)
    : QWidget(parent)
{
    //
    LoadLyric();
    //
    InitUi();
    //
    m_pTimer = new QTimer(this);
    m_pTimer->setInterval(10);
    connect(m_pTimer, &QTimer::timeout, this, &PlayerBody::OnPositionChange);
}

//
PlayerBody::~PlayerBody()
{

}

//
void PlayerBody::InitUi()
{
    //top
    m_pTitleLabel = new QLabel;
    m_pTitleLabel->setObjectName("SongTitle");
    m_pTitleLabel->setAlignment(Qt::AlignCenter);
    // left
    m_pImageWgt = new SongPicture;
    m_pVolumnLabel = new QLabel;
    // right
    m_pLrcWgt = new LyricWgt(this);
    //
    QVBoxLayout* leftLayout = new QVBoxLayout;
    leftLayout->setMargin(0);
    leftLayout->setContentsMargins(20, 0, 20, 0);
    leftLayout->addItem(new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding));
    leftLayout->addWidget(m_pImageWgt);
    leftLayout->addWidget(m_pVolumnLabel);
    leftLayout->addItem(new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding));
    //
    QHBoxLayout* bottomLayout = new QHBoxLayout;
    bottomLayout->setContentsMargins(0, 15, 0, 15);
    bottomLayout->addLayout(leftLayout);
    bottomLayout->addWidget(m_pLrcWgt);
    bottomLayout->setStretch(0, 3);
    bottomLayout->setStretch(1, 8);
    //
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(m_pTitleLabel);
    layout->addLayout(bottomLayout);
    setLayout(layout);}

//
void PlayerBody::ShowMusicInfo(const QStringList &info, const QImage &img)
{
    QMutexLocker locker(&m_mutex);
    // 清除历史数据
    m_mapLstWgtItem.clear();
    if(m_pLrcWgt)
    {
        m_pLrcWgt->clear();
    }

    if(info.contains("ThumbnailImage"))
        m_musicImage = img;
    else
        m_musicImage.load(DEFAULT_IMAGE_PATH);
    //
    m_pImageWgt->UpdateImage(m_musicImage);
    // title
    if(info.contains("Title"))
    {
        m_strMusicTitle = g_pMusic->GetPlayer()->metaData("Title").toString();
        m_pTitleLabel->setText(m_strMusicTitle);
        if(m_mapLyric.contains(m_strMusicTitle))
        {
            QMap<int, QString>& mapLyric = m_mapLyric[m_strMusicTitle];

            for(QMap<int, QString>::iterator it = mapLyric.begin(); it != mapLyric.end(); it++)
            {
                QListWidgetItem* item = new QListWidgetItem(*it);
                item->setTextAlignment(Qt::AlignCenter);
                item->setTextAlignment(Qt::AlignHCenter);
                m_pLrcWgt->addItem(item);
                m_mapLstWgtItem[it.key()] = item;
            }
        }
    }
}

//
void PlayerBody::UpdatVolume()
{
    m_pVolumnLabel->setText(QString("音量: %1%").arg(g_pMusic->GetPlayer()->volume()));
}

//
void PlayerBody::OnPositionChange()
{
    if(m_mapLstWgtItem.size() == 0) return;
    //
    QMutexLocker locker(&m_mutex);
    quint64 position = g_pMusic->GetPlayer()->position();
    QList<quint64> lstTime = m_mapLstWgtItem.keys();
    QList<quint64>::iterator itFind = std::find_if(lstTime.begin(), lstTime.end(), [&](quint64 time)
    {
        return (abs(time - position) < 100);
    });
    if(itFind != lstTime.end() && m_pLrcWgt->currentItem() != m_mapLstWgtItem[*itFind])
    {
        position = *itFind;
        m_pLrcWgt->scrollToItem(m_mapLstWgtItem[position], QAbstractItemView::PositionAtCenter  );
        m_pLrcWgt->setCurrentItem(m_mapLstWgtItem[position]);
        m_mapLstWgtItem[position]->setSelected(true);
    }
}

// 加载所有歌词
void PlayerBody::LoadLyric()
{
    QDir rootDir(LYRIC_DIR_PATH);
    QFileInfoList infoList = rootDir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
    for(QFileInfoList::iterator it = infoList.begin(); it != infoList.end(); it++)
    {
        QFile lyricFile(it->absoluteFilePath());
        if(lyricFile.exists() && lyricFile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QString title = QString();
            //
            bool isLyric = false;
            while (!lyricFile.atEnd())
            {
                QString lyric = FROM_LOCAL(lyricFile.readLine());
                //
                if(!isLyric)
                {
                    if(lyric.startsWith("[00:"))
                        isLyric = true;
                    else if(lyric.startsWith("[ti:"))
                    {
                        title = lyric.mid(4);
                        title.chop(1);
                        continue;
                    }
                    else
                        continue;
                }
                //
                int time = Str2IntTime(lyric);
                lyric = lyric.mid(10);
                if(time >= 0)
                {
                    m_mapLyric[title][time] = lyric;
                }
                //
                if (lyric.startsWith("["))
                {
                    int time2 = Str2IntTime(lyric);
                    lyric = lyric.mid(10);
                    if(time2 >= 0)
                    {
                        m_mapLyric[title][time] = lyric;
                        m_mapLyric[title][time2] = lyric;
                    }
                }
            }
        }
    }
}

//
quint64 PlayerBody::Str2IntTime(const QString &strTime)
{
    quint64 intTime = 0;
    QTime time = QTime::fromString(strTime.mid(1, 8), "mm:ss.z");
    intTime += time.minute() * 60000;
    intTime += time.second() * 1000;
    intTime += time.msec();
    return intTime;
}
