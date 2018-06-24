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
#include <QMenu>

class MusicPlaylist : public QWidget
{
    Q_OBJECT
public:
    explicit MusicPlaylist(QMediaPlaylist* playList, QWidget *parent = 0);
    ~MusicPlaylist();

public:
    void WriteConf();
    int GetSongCount() {return m_arSongList.size();}

signals:
    void playMusic();

protected:
    void initUi();
    void ReadConf();
    void AddPlayList(QString filePath);
    void initMenu();

private slots:
    void OnBtnAddClick();
    void OnBtnDelClick();
    void OnListItemDoubleClick(QListWidgetItem * item);
    void OnAddFile();
    void OnAddDir();
    void OnAddNetFile();

private:
    QListWidget* m_pPlayList;
    QPushButton* m_pBtnAdd;
    QPushButton* m_pBtnDel;

    QMediaPlaylist* m_pMediaPlaylist;

    QList<QListWidgetItem*> m_arListWidgetItem;
    QList<QString>          m_arSongList;
    QString                 m_lastDir;

    QMenu*                  m_pMenu;
};

#endif // MUSICPLAYLIST_H
