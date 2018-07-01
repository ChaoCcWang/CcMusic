#ifndef PLAYERBODY_H
#define PLAYERBODY_H

#include <QWidget>
#include <QImage>
#include <QMap>
#include <QMutexLocker>

class QLabel;
class QTimer;
class LyricWgt;
class SongPicture;
class QListWidgetItem;
class PlayerBody : public QWidget
{
    Q_OBJECT
public:
    explicit PlayerBody(QWidget *parent = 0);
    ~PlayerBody();

public:
    void     ShowMusicInfo(const QStringList& info, const QImage& img);
    void     UpdatVolume();
    QTimer*  GetTimer() {return m_pTimer;}

public slots:
    void     OnPositionChange();

protected:
    quint64  Str2IntTime(const QString& strTime);
    void     LoadLyric();
    void     InitUi();

private:
    QLabel*        m_pTitleLabel;
    SongPicture*   m_pImageWgt;
    QLabel*        m_pVolumnLabel;
    LyricWgt*      m_pLrcWgt;
    QMap<quint64, QListWidgetItem*> m_mapLstWgtItem;

    QImage         m_musicImage;
    QString        m_strMusicTitle;
    QMap<QString, QMap<int, QString> > m_mapLyric;
    QTimer*        m_pTimer;

    QMutex         m_mutex;
};

#endif // PLAYERBODY_H
