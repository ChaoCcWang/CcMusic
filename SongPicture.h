#ifndef SONGPICTURE_H
#define SONGPICTURE_H

#include <QWidget>
#include <QImage>

class SongPicture : public QWidget
{
    Q_OBJECT
public:
    explicit SongPicture(QWidget *parent = 0);
    ~SongPicture();

public:
    void UpdateImage(const QImage& img);

protected:
    void paintEvent(QPaintEvent* e);

private:
    QImage  m_musicImage;
};

#endif // SONGPICTURE_H
