#ifndef MUSICPLAYLIST_H
#define MUSICPLAYLIST_H

#include <QWidget>
#include <QRect>
#include <QPushButton>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMouseEvent>
#include <QPoint>
#include <QMediaPlaylist>

class MusicPlaylist : public QWidget
{
    Q_OBJECT
public:
    explicit MusicPlaylist(QMediaPlaylist* playList, QWidget *parent = 0);
    ~MusicPlaylist();

public:
    void WriteConf();

signals:
    void playMusic();

protected:
    void initUi();
    void ReadConf();
    void AddPlayList(QString filePath);


private:
    QListWidget* m_pPlayList;
    QPushButton* m_pBtnAdd;
    QPushButton* m_pBtnDel;

    QMediaPlaylist* m_pMediaPlaylist;

    QList<QListWidgetItem*> m_arListWidgetItem;
    QList<QString>          m_arSongList;
    QString                 m_lastDir;

private slots:
    void OnBtnAddClick();
    void OnBtnDelClick();
    void OnListItemDoubleClick(QListWidgetItem * item);
};

#endif // MUSICPLAYLIST_H
