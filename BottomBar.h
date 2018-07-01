#ifndef BOTTOMBAR_H
#define BOTTOMBAR_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QSlider>
#include "MusicPlaylist.h"

#define ICON_PALY_PATH       "://Data/Image/play.png"
#define ICON_PAUSE_PATH      "://Data/Image/pause.png"
#define ICON_STOP_PATH       "://Data/Image/stop.png"
#define ICON_NEXT_PATH       "://Data/Image/next.png"
#define ICON_PRE_PATH        "://Data/Image/pre.png"

enum { EM_LIST_RECYCLE, EM_ONE_RECYCLE, EM_RANDOM, EM_MODE_COUNT};

class ProcessSlider;
class VolumeSlider;
class BottomBar : public QWidget
{
    Q_OBJECT
public:
    explicit BottomBar(QWidget *parent = 0);
    ~BottomBar();

public:

    QPushButton* GetNextButton() {return m_pBtnNext;}
    QPushButton* GetPreButton() {return m_pBtnPre;}
    QPushButton* GetPlayButton() {return m_pBtnPlay;}
    void         SetPlayMode(uchar mode);

signals:
    void SignalPositionChange(int);

public slots:
    void HidePlayList(bool hide);
    void OnPositionChange(qint64 position);
    void OnDurationChanged(qint64 duration);

protected:
    void initUi();
    void showEvent(QShowEvent *e);
    void keyPressEvent(QKeyEvent* e);

public:
    MusicPlaylist* m_pPlayListWnd;
    uchar          m_ucPlayMode;

private:
    QPushButton*         m_pBtnPre;
    QPushButton*         m_pBtnPlay;
    QPushButton*         m_pBtnStop;
    QPushButton*         m_pBtnNext;
    QPushButton*         m_pBtnSonglist;
    QPushButton*         m_pBtnVolume;
    QPushButton*         m_pBtnPlayMode;

    QLabel*              m_LabelTime;
    ProcessSlider*       m_pSliderProcess;
    VolumeSlider*        m_pSliderVolume;
    QMap<uchar, QString> m_mapModeIcon;

private slots:
    void OnPlayBtnClick();
    void OnStopBtnClick();
    void OnBtnPlayListClick(bool isOpen);
    void OnPlayListDoubleClick();
    void SwitchMode();
};

#endif // BOTTOMBAR_H
