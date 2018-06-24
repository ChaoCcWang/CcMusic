#ifndef BOTTOMBAR_H
#define BOTTOMBAR_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QSlider>
#include "MusicPlaylist.h"

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
    uchar GetPlayMode() {return m_ucPlayMode;}
    QPushButton* GetNextButton() {return m_pBtnNext;}
    QPushButton* GetPreButton() {return m_pBtnPre;}
    QPushButton* GetPlayButton() {return m_pBtnPlay;}

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

    uchar                m_ucPlayMode;
    QMap<uchar, QString> m_mapModeIcon;

private slots:
    void OnPlayBtnClick();
    void OnStopBtnClick();
    void OnBtnPlayListClick(bool isOpen);
    void OnPlayListDoubleClick();
    void SwitchMode();
};

#endif // BOTTOMBAR_H
