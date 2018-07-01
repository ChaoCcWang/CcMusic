#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QPoint>
#include <QMouseEvent>

#define ICON_SHHUT_DOWN_PATH  "://Data/Image/shut-down.png"

class TitleBar : public QWidget
{
    Q_OBJECT
public:
    TitleBar(QWidget *parent = 0);
    ~TitleBar();

signals:
    void showWndMax(bool isMax);


protected:
    void initUi();
    virtual void mousePressEvent(QMouseEvent* e);
    virtual void mouseReleaseEvent(QMouseEvent* e);
    virtual void mouseMoveEvent(QMouseEvent* e);

private:
    //
    QLabel*      m_pLabelIcon;
    QLabel*      m_pLabelTitle;
    QPushButton* m_pBtnMinimize;
    QPushButton* m_pBtnMaximize;
    QPushButton* m_pBtnClose;
    //
    bool         m_bMove;
    QPoint       m_movePosition;

private slots:

};

#endif // TITLEBAR_H
