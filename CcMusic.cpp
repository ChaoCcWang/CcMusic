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
#include <QTime>
#include <QDebug>
#include <QTimer>

#define QSS_FILE_PATH    "../Data/Style/default.css"

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
    //
    m_pPlayerBody->UpdatVolume();
    //
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
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
    bool isPlay = (m_pPlayer->state() == QMediaPlayer::PlayingState);
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
    if(isPlay)
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
        m_pPlayerBody->GetTimer()->start();
        return true;
    }
    return false;
}

//
void CcMusic::Pause()
{
    m_pPlayer->pause();
    m_pPlayerBody->GetTimer()->stop();
}

//
void CcMusic::Stop()
{
    m_pPlayer->stop();
    m_pPlayerBody->GetTimer()->stop();
    m_pPlayer->setPosition(0);
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
uchar CcMusic::GetPlayMode()
{
    return m_pBottomBar->m_ucPlayMode;
}

//
void CcMusic::SetPlayMode(uchar mode)
{
    m_pBottomBar->SetPlayMode(mode);
}

//
void CcMusic::setVolume(int v)
{
    m_pPlayer->setVolume(v);
    m_pPlayerBody->UpdatVolume();
}

//
void CcMusic::playPreSong()
{
    disConnectPlayerSignal();
    //
    bool isPlay = (m_pPlayer->state() == QMediaPlayer::PlayingState);
    bool isPreBtnClick = (m_pBottomBar->GetPreButton() == (QPushButton*)sender());
    uchar mode = GetPlayMode();
    if(mode == EM_LIST_RECYCLE || (mode == EM_ONE_RECYCLE && isPreBtnClick))
    {
        if(m_pMediaPlaylist->currentIndex() != 0)
            m_pMediaPlaylist->previous();
        else
            m_pMediaPlaylist->setCurrentIndex(GetPlayList()->GetSongCount() - 1);
    }
    else if(mode == EM_RANDOM)
    {
        int index = qrand() % (GetPlayList()->GetSongCount() - 1);   //随机生成0到9的随机数
        m_pMediaPlaylist->setCurrentIndex(index);
    }

    // 恢复初始位置
    m_pPlayer->setPosition(0);
    //
    isPlay ? m_pPlayer->play() : m_pPlayer->stop();
    ConnectPlayerSignal();
}

void CcMusic::playNextSong()
{
    disConnectPlayerSignal();
    //
    bool isPlay = (m_pPlayer->state() == QMediaPlayer::PlayingState);
    bool isNextBtnClick = (m_pBottomBar->GetNextButton() == (QPushButton*)sender());
    uchar mode = GetPlayMode();
    if(mode == EM_LIST_RECYCLE || (mode == EM_ONE_RECYCLE && isNextBtnClick))
    {
        if(m_pMediaPlaylist->nextIndex() != -1)
            m_pMediaPlaylist->next();
        else
            m_pMediaPlaylist->setCurrentIndex(0);
    }
    else if(mode == EM_RANDOM)
    {
        int index = qrand() % (GetPlayList()->GetSongCount() - 1);   //随机生成0到9的随机数
        m_pMediaPlaylist->setCurrentIndex(index);
    }


    // 恢复初始位置
    m_pPlayer->setPosition(0);
    //
    isPlay ? m_pPlayer->play() : m_pPlayer->stop();
    ConnectPlayerSignal();
}

//
void CcMusic::keyPressEvent(QKeyEvent *e)
{
    m_pPlayerBody->GetTimer()->stop();
    switch (e->key()) {
    case Qt::Key_Space:
        m_pBottomBar->GetPlayButton()->click();
        break;
    case Qt::Key_Left:
        m_pBottomBar->GetPreButton()->click();
        break;
    case Qt::Key_Right:
        m_pBottomBar->GetNextButton()->click();
        break;
    case Qt::Key_Up:
        m_pPlayer->setVolume(m_pPlayer->volume() + 1);
        break;
    case Qt::Key_Down:
        m_pPlayer->setVolume(m_pPlayer->volume() - 1);
        break;
    default:
        break;
    }
    m_pPlayerBody->UpdatVolume();
    //return QWidget::keyPressEvent(e);
}

