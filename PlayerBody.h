#ifndef PLAYERBODY_H
#define PLAYERBODY_H

#include <QWidget>
#include <QImage>

class QLabel;
class PlayerBody : public QWidget
{
    Q_OBJECT
public:
    explicit PlayerBody(QWidget *parent = 0);
    ~PlayerBody();

public:
    void     ShowMusicInfo(const QStringList& info, const QImage& img);

protected:
    void     paintEvent(QPaintEvent* e);

private:
    QImage    m_musicImage;
    QString   m_strMusicTitle;
};

#endif // PLAYERBODY_H
