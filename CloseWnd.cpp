#include "CloseWnd.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

CloseWnd::CloseWnd(QWidget *parent)
    : QDialog(parent), m_state(EM_CANCLE)
{
    // 去边框
    setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint);
    setFixedSize(250, 130);
    setWindowTitle("退出程序");
    //
    QVBoxLayout* layout = new QVBoxLayout;
    layout->setSpacing(2);
    layout->setContentsMargins(50,15,50,5);
    m_pTray = new QRadioButton("隐藏至系统托盘");
    m_pQuit = new QRadioButton("退出应用程序");
    m_pTray->setChecked(true);
    layout->addWidget(m_pTray);
    layout->addWidget(m_pQuit);
    //
    QHBoxLayout* bottomLayout = new QHBoxLayout;
    QPushButton* btnOk = new QPushButton("确定");
    QPushButton* btnCancle = new QPushButton("取消");
    bottomLayout->addItem(new QSpacerItem(20,20,QSizePolicy::Expanding));
    bottomLayout->addWidget(btnOk);
    bottomLayout->addWidget(btnCancle);

    layout->addItem(new QSpacerItem(20 , 20));
    layout->addLayout(bottomLayout);
    setLayout(layout);

    connect(btnOk, &QPushButton::clicked, [&]()
    {
        m_state = m_pTray->isChecked() ? EM_TRAY : EM_QUIT;
        close();
    });
    connect(btnCancle, &QPushButton::clicked, [&]()
    {
        m_state = EM_CANCLE;
        close();
    });
}

CloseWnd::~CloseWnd()
{

}

uchar CloseWnd::GetCloseWndState()
{
    return m_state;
}

