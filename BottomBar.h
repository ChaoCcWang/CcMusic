#ifndef BOTTOMBAR_H
#define BOTTOMBAR_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QSlider>
#include "MusicPlaylist.h"

class ProcessSlider;
class BottomBar : public QWidget
{
    Q_OBJECT
public:
    explicit BottomBar(QWidget *parent = 0);
    ~BottomBar();



signals:
    void SignalPositionChange(int);

public slots:
    void HidePlayList(bool hide);
    void OnPositionChange(qint64 position);
    void OnDurationChanged(qint64 duration);

protected:
    void initUi();
    void showEvent(QShowEvent *e);

public:
    MusicPlaylist* m_pPlayListWnd;

private:
    QPushButton* m_pBtnPre;
    QPushButton* m_pBtnPlay;
    QPushButton* m_pBtnStop;
    QPushButton* m_pBtnNext;
    QPushButton* m_pBtnSonglist;
    QPushButton* m_pBtnVolume;

    QLabel*      m_LabelTime;
    ProcessSlider*     m_pSliderProcess;
    QSlider*     m_pSliderVolume;

private slots:
    void OnPlayBtnClick();
    void OnStopBtnClick();
    void OnBtnPlayListClick(bool isOpen);
    void OnPlayListDoubleClick();
};

#endif // BOTTOMBAR_H
