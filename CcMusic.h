#ifndef CCMUSIC_H
#define CCMUSIC_H

#include <QWidget>
#include <QVariant>
#include <QMap>
#include "MusicPlaylist.h"

namespace Ui {
class CcMusic;
}

// 拖拽添加歌曲
// 资源丢失问题
// 右键打开文件问题
// 当前歌曲背景色添加


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
    bool Play();
    void Pause();
    void Stop();
    void playPreSong();
    void playNextSong();

protected:
    void initUi();
    void initMusic();
    void closeEvent(QCloseEvent* e);
    void SetQssStyle();
    //void dragEnterEvent(QDragEnterEvent *e);
    //void dropEvent(QDropEvent *e);


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
