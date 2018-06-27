#ifndef CCMUSIC_H
#define CCMUSIC_H

#include <QWidget>
#include <QVariant>
#include "MusicPlaylist.h"

namespace Ui {
class CcMusic;
}

// 显示歌词
// 添加文件夹显示
// 资源丢失问题


#define WND_ICON_PATH "://Data/Image/wnd-icon.png"

class TitleBar;
class BottomBar;
class PlayerBody;
class QMediaPlayer;
class QMediaPlaylist;
class CloseWnd;
class QSystemTrayIcon;
class CcMusic : public QWidget
{
    Q_OBJECT

public:
    explicit CcMusic(QWidget *parent = 0);
    ~CcMusic();

public:
    void SetCurrentMusic(int index);
    bool Play();
    void Pause();
    void Stop();
    void OnClose();
    QMediaPlayer*   GetPlayer();
    MusicPlaylist*  GetPlayList();
    QMediaPlaylist* GetMediaPlaylist();
    uchar           GetPlayMode();
    void            SetPlayMode(uchar mode);

    void ConnectPlayerSignal();
    void disConnectPlayerSignal();
    void keyPressEvent(QKeyEvent *e);

public slots:
    void setVolume(int v);
    void playPreSong();
    void playNextSong();

protected:
    void initUi();
    void initMusic();
    void closeEvent(QCloseEvent* e);
    void SetQssStyle();

private:
    Ui::CcMusic *ui;
    TitleBar*        m_pTitleBar;
    BottomBar*       m_pBottomBar;
    PlayerBody*      m_pPlayerBody;
    QMediaPlayer*    m_pPlayer;
    QMediaPlaylist*  m_pMediaPlaylist;
    CloseWnd*        m_pCloseWnd;
    QSystemTrayIcon* m_pSysTray;
};

#endif // CCMUSIC_H
