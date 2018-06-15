#include "CcMusic.h"
#include "ui_CcMusic.h"
#include "TitleBar.h"
#include "BottomBar.h"
#include "PlayerBody.h"
#include "CloseWnd.h"
#include <QVBoxLayout>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QMessageBox>
#include <QFile>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QDebug>

#define QSS_FILE_PATH "../Data/Style/default.css"

extern CcMusic* g_pMusic = NULL;
CcMusic::CcMusic(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CcMusic)
{
    //
    g_pMusic = this;
    //
    ui->setupUi(this);
    // 去边框
    setWindowFlags(Qt::Dialog|Qt::WindowMinMaxButtonsHint|Qt::FramelessWindowHint);
    //
    SetQssStyle();
    //
    initMusic();
    //
    initUi();
}

CcMusic::~CcMusic()
{
    delete ui;
}

void CcMusic::initUi()
{
    // top
    QHBoxLayout* topLayout = new QHBoxLayout;
    topLayout->setMargin(0);
    m_pTitleBar = new TitleBar;
    topLayout->addWidget(m_pTitleBar);
    ui->WgtTop->setLayout(topLayout);
    ui->WgtTop->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    // main
    QHBoxLayout* bodyLayout = new QHBoxLayout;
    bodyLayout->setMargin(0);
    m_pPlayerBody = new PlayerBody;
    bodyLayout->addWidget(m_pPlayerBody);
    ui->WgtMain->setLayout(bodyLayout);
    ui->WgtTop->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    // bottom
    QHBoxLayout* bottomLayout = new QHBoxLayout;
    bottomLayout->setMargin(0);
    m_pBottomBar = new BottomBar;
    m_pBottomBar->setObjectName("BottomBar");
    bottomLayout->addWidget(m_pBottomBar);
    ui->WgtBottom->setLayout(bottomLayout);
    ui->WgtBottom->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    // closeWnd
    m_pCloseWnd = new CloseWnd(this);
    //
    setWindowIcon(QIcon(WND_ICON_PATH));
    // 系统托盘
    m_pSysTray = new QSystemTrayIcon(QIcon(WND_ICON_PATH), this);
    m_pSysTray->setToolTip("CcMusic");
    QMenu* trayMenu = new QMenu();
    m_pSysTray->setContextMenu(trayMenu);
    QAction* showHideAction = new QAction("显示&&隐藏", trayMenu);
    QAction* quitAction = new QAction("退出", trayMenu);
    trayMenu->addAction(showHideAction);
    trayMenu->addSeparator();
    trayMenu->addAction(quitAction);
    m_pSysTray->show();
    connect(showHideAction, &QAction::triggered, [&](){
        if(this->isHidden())
        {
            this->show();
        }
        else
        {
            this->hide();
            GetPlayList()->hide();
        }
    });
    connect(quitAction, &QAction::triggered, [&](){
        exit(0);
    });
    //
    connect(m_pTitleBar, &TitleBar::showWndMax, m_pBottomBar, &BottomBar::HidePlayList);
    ConnectPlayerSignal();
}

//
void CcMusic::initMusic()
{
    m_pPlayer = new QMediaPlayer;
    //
    m_pMediaPlaylist = new QMediaPlaylist;
    m_pPlayer->setPlaylist(m_pMediaPlaylist);
    m_pPlayer->setVolume(50);

    connect(m_pPlayer, &QMediaPlayer::metaDataAvailableChanged, [&](bool available)
    {
        if(available)
        {
            QImage img = m_pPlayer->metaData("ThumbnailImage").value<QImage>();
            m_pPlayerBody->ShowMusicInfo(m_pPlayer->availableMetaData(), img);
        }
    });

}

void CcMusic::ConnectPlayerSignal()
{
    connect(m_pPlayer, &QMediaPlayer::positionChanged, m_pBottomBar, &BottomBar::OnPositionChange);
    connect(m_pPlayer, &QMediaPlayer::durationChanged, m_pBottomBar, &BottomBar::OnDurationChanged);
}

void CcMusic::disConnectPlayerSignal()
{
    disconnect(m_pPlayer, &QMediaPlayer::positionChanged, m_pBottomBar, &BottomBar::OnPositionChange);
    disconnect(m_pPlayer, &QMediaPlayer::durationChanged, m_pBottomBar, &BottomBar::OnDurationChanged);
}

void CcMusic::closeEvent(QCloseEvent *e)
{
    OnClose();
    return QWidget::closeEvent(e);
}

//
void CcMusic::SetQssStyle()
{
    QFile qssFile(QSS_FILE_PATH);
    if(qssFile.open(QIODevice::ReadOnly))
    {
        setStyleSheet(qssFile.readAll());
        qssFile.close();
    }
}

//
void CcMusic::OnClose()
{
    m_pPlayer->pause();
    //
    m_pCloseWnd->exec();
    if(m_pCloseWnd->GetCloseWndState() == CloseWnd::EM_TRAY)
    {
        this->hide();
        GetPlayList()->hide();
    }
    else if(m_pCloseWnd->GetCloseWndState() == CloseWnd::EM_QUIT)
    {
        GetPlayList()->WriteConf();
        exit(0);
    }
    else
    {
        m_pPlayer->play();
    }
}

//
void CcMusic::SetCurrentMusic(int index)
{
    m_pMediaPlaylist->setCurrentIndex(index);
}

//
bool CcMusic::Play()
{
    if(m_pMediaPlaylist->currentIndex() != -1)
    {
        m_pPlayer->play();
        return true;
    }
    return false;
}

//
void CcMusic::Pause()
{
    m_pPlayer->pause();
}

//
void CcMusic::Stop()
{
    m_pPlayer->stop();
}

//
QMediaPlayer *CcMusic::GetPlayer()
{
    if(m_pPlayer)
    {
        return m_pPlayer;
    }
    return NULL;
}

//
MusicPlaylist* CcMusic::GetPlayList()
{
    if(m_pBottomBar->m_pPlayListWnd)
    {
        return m_pBottomBar->m_pPlayListWnd;
    }
    return NULL;
}

//
QMediaPlaylist* CcMusic::GetMediaPlaylist()
{
    if(m_pMediaPlaylist)
    {
        return m_pMediaPlaylist;
    }
    return NULL;
}

//
void CcMusic::setVolume(int v)
{
    m_pPlayer->setVolume(v);
}

//
void CcMusic::playPreSong()
{
    bool isPlay = m_pPlayer->state() == QMediaPlayer::PlayingState;
    if(m_pMediaPlaylist->previousIndex() != -1)
    {
        m_pMediaPlaylist->previous();
        // 恢复初始位置
        m_pPlayer->setPosition(0);
        // 播放？
        isPlay ? m_pPlayer->play() : m_pPlayer->stop();
    }
}

void CcMusic::playNextSong()
{
    bool isPlay = m_pPlayer->state() == QMediaPlayer::PlayingState;
    if(m_pMediaPlaylist->nextIndex() != -1)
    {
        m_pMediaPlaylist->next();
        // 恢复初始位置
        m_pPlayer->setPosition(0);
        //
        isPlay ? m_pPlayer->play() : m_pPlayer->stop();
    }
}

