#include "BottomBar.h"
#include "CcMusic.h"
#include "ProcessSlider.h"
#include "VolumeSlider.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMediaPlayer>
#include <QTime>

#define ICON_PALY_PATH       "://Data/Image/play.png"
#define ICON_PAUSE_PATH      "://Data/Image/pause.png"
#define ICON_STOP_PATH       "://Data/Image/stop.png"
#define ICON_NEXT_PATH       "://Data/Image/next.png"
#define ICON_PRE_PATH        "://Data/Image/pre.png"
#define ICON_PRE_PATH        "://Data/Image/pre.png"
#define ICON_SOUND_PATH      "://Data/Image/sound.png"
#define ICON_LIST_PATH       "://Data/Image/list-view.png"
#define ICON_RANDOM_PATH     "://Data/Image/random.png"
#define ICON_REPEAT_PATH     "://Data/Image/repeat.png"
#define ICON_REPEAT_ONE_PATH "://Data/Image/repeat_one.png"
#define ONE_HOUR_SCOND  1000 * 60 * 60


extern CcMusic* g_pMusic;
BottomBar::BottomBar(QWidget *parent)
    : QWidget(parent), m_ucPlayMode(EM_LIST_RECYCLE)
{
    initUi();


    m_mapModeIcon[EM_LIST_RECYCLE] = ICON_REPEAT_PATH;
    m_mapModeIcon[EM_ONE_RECYCLE] = ICON_REPEAT_ONE_PATH;
    m_mapModeIcon[EM_RANDOM] = ICON_RANDOM_PATH;

}

BottomBar::~BottomBar()
{

}

//
void BottomBar::initUi()
{
    //
    m_pBtnPre = new QPushButton();
    m_pBtnPlay = new QPushButton();
    m_pBtnStop = new QPushButton();
    m_pBtnNext = new QPushButton();
    m_pBtnSonglist = new QPushButton();
    m_pBtnPlayMode = new QPushButton();
    m_LabelTime = new QLabel("00:00/00:00");
    //
    m_pSliderProcess = new ProcessSlider(Qt::Horizontal, this);

    m_pBtnVolume = new QPushButton();
    m_pSliderVolume = new VolumeSlider(Qt::Horizontal, this);
    //
    m_pBtnPre->setIcon(QIcon(ICON_PRE_PATH));
    m_pBtnPlay->setIcon(QIcon(ICON_PALY_PATH));
    m_pBtnStop->setIcon(QIcon(ICON_STOP_PATH));
    m_pBtnNext->setIcon(QIcon(ICON_NEXT_PATH));
    m_pBtnVolume->setIcon(QIcon(ICON_SOUND_PATH));
    m_pBtnSonglist->setIcon(QIcon(ICON_LIST_PATH));
    m_pBtnPlayMode->setIcon(QIcon(ICON_REPEAT_PATH));
    m_pBtnVolume->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_pBtnSonglist->setCheckable(true);
    //
    m_pBtnPre->setFocusPolicy(Qt::NoFocus);
    m_pBtnPlay->setFocusPolicy(Qt::NoFocus);
    m_pBtnStop->setFocusPolicy(Qt::NoFocus);
    m_pBtnNext->setFocusPolicy(Qt::NoFocus);
    m_pBtnVolume->setFocusPolicy(Qt::NoFocus);
    m_pBtnSonglist->setFocusPolicy(Qt::NoFocus);
    m_pBtnPlayMode->setFocusPolicy(Qt::NoFocus);
    //
    QHBoxLayout* bottomLayout = new QHBoxLayout;
    bottomLayout->setMargin(0);
    bottomLayout->setSpacing(10);
    bottomLayout->setContentsMargins(5, 0, 5, 5);
    bottomLayout->addWidget(m_pBtnPre);
    bottomLayout->addWidget(m_pBtnPlay);
    bottomLayout->addWidget(m_pBtnStop);
    bottomLayout->addWidget(m_pBtnNext);
    bottomLayout->addWidget(m_LabelTime);
    bottomLayout->addItem(new QSpacerItem(20, 20, QSizePolicy::Expanding));
    bottomLayout->addWidget(m_pBtnPlayMode);
    bottomLayout->addWidget(m_pBtnSonglist);
    //
    QHBoxLayout* topLayout = new QHBoxLayout;
    topLayout->setMargin(0);
    topLayout->addWidget(m_pSliderProcess);
    topLayout->addItem(new QSpacerItem(20, 20));
    topLayout->addWidget(m_pBtnVolume);
    topLayout->addWidget(m_pSliderVolume);
    topLayout->setStretch(0, 6);
    topLayout->setStretch(1, 0);
    topLayout->setStretch(2, 4);
    //
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addLayout(topLayout);
    layout->addLayout(bottomLayout);
    //
    setLayout(layout);
    //
    m_pSliderVolume->setRange(0, 100);
    m_pSliderVolume->setValue(50);
    //
    m_pPlayListWnd = new MusicPlaylist(g_pMusic->GetMediaPlaylist(), g_pMusic);
    m_pPlayListWnd->hide();
    connect(m_pPlayListWnd, &MusicPlaylist::playMusic, this, &BottomBar::OnPlayListDoubleClick);
    //
    connect(m_pSliderVolume, &QSlider::valueChanged, g_pMusic, &CcMusic::setVolume);
    connect(m_pBtnPlay, &QPushButton::clicked, this, &BottomBar::OnPlayBtnClick);
    connect(m_pBtnStop, &QPushButton::clicked, this, &BottomBar::OnStopBtnClick);

    connect(m_pBtnSonglist, &QPushButton::clicked, this, &BottomBar::OnBtnPlayListClick);
    connect(m_pBtnPre, &QPushButton::clicked, g_pMusic, &CcMusic::playPreSong);
    connect(m_pBtnNext, &QPushButton::clicked, g_pMusic, &CcMusic::playNextSong);
    connect(m_pBtnPlayMode, &QPushButton::clicked, this, &BottomBar::SwitchMode);
}

//
void BottomBar::showEvent(QShowEvent *e)
{
    m_pBtnSonglist->isChecked() ? m_pPlayListWnd->show() : m_pPlayListWnd->hide();
    return QWidget::showEvent(e);
}

//
void BottomBar::keyPressEvent(QKeyEvent *e)
{
    g_pMusic->keyPressEvent(e);
}

//
void BottomBar::SwitchMode()
{
    m_ucPlayMode += 1;
    if(m_ucPlayMode == EM_MODE_COUNT)
    {
        m_ucPlayMode = EM_LIST_RECYCLE;
    }
    m_pBtnPlayMode->setIcon(QIcon(m_mapModeIcon[m_ucPlayMode]));
}

void BottomBar::HidePlayList(bool hide)
{
    if(m_pBtnSonglist->isChecked())
    {
        hide ? m_pPlayListWnd->hide() : m_pPlayListWnd->show();
    }
}

//
void BottomBar::OnPlayBtnClick()
{
    if(!g_pMusic) return;

    switch (g_pMusic->GetPlayer()->state())
    {
    case QMediaPlayer::StoppedState:
    case QMediaPlayer::PausedState:
    {
        m_pBtnPlay->setIcon(QIcon( g_pMusic->Play() ? ICON_PAUSE_PATH : ICON_PALY_PATH));
        break;
    }
    case QMediaPlayer::PlayingState:
    {
        m_pBtnPlay->setIcon(QIcon(ICON_PALY_PATH));
        g_pMusic->Pause();
        break;
    }
    }
}

void BottomBar::OnStopBtnClick()
{
    g_pMusic->Stop();
    m_pBtnPlay->setIcon(QIcon(ICON_PALY_PATH));
    m_pBtnPlay->setChecked(false);
}

void BottomBar::OnPositionChange(qint64 position)
{
    m_pSliderProcess->setValue(position);
    if(position == m_pSliderProcess->maximum())
    {
        g_pMusic->playNextSong();
    }
    QString strTime = m_LabelTime->text();
    QString strCount = QTime(0, 0, 0).addMSecs(position)
            .toString(position > ONE_HOUR_SCOND ? "h:mm:ss" : "mm:ss");
    m_LabelTime->setText(QString("%1/%2").arg(strCount, strTime.section("/", 1, 1)));
}

void BottomBar::OnDurationChanged(qint64 duration)
{
    m_pSliderProcess->setRange(0, duration);
    QString strTimeText = QString("00:00/%1").arg(QTime(0, 0, 0)
                         .addMSecs(duration).toString(duration > ONE_HOUR_SCOND ? "h:mm:ss" : "mm:ss"));
    m_LabelTime->setText(strTimeText);
}

void BottomBar::OnBtnPlayListClick(bool isOpen)
{
    if(isOpen)
    {
        QRect rect = g_pMusic->geometry();
        m_pPlayListWnd->setGeometry(QRect(g_pMusic->mapToGlobal(QPoint(rect.width(),0)), QSize(300, rect.height())));
        m_pPlayListWnd->show();
    }
    else
    {
        m_pPlayListWnd->hide();
    }
}

//
void BottomBar::OnPlayListDoubleClick()
{
    m_pBtnPlay->setIcon(QIcon( g_pMusic->Play() ? ICON_PAUSE_PATH : ICON_PALY_PATH));
}
