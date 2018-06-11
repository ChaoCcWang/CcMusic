#ifndef CLOSEWND_H
#define CLOSEWND_H

#include <QDialog>
#include <QRadioButton>

class CloseWnd : public QDialog
{
    Q_OBJECT
public:
    explicit CloseWnd(QWidget *parent = 0);
    ~CloseWnd();

    enum close_state {EM_CANCLE, EM_TRAY, EM_QUIT };

public:
    uchar GetCloseWndState();

private:
    QRadioButton*  m_pTray;
    QRadioButton*  m_pQuit;

    uchar          m_state;

};

#endif // CLOSEWND_H
