#include "TitleBar.h"
#include "CcMusic.h"
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QApplication>


#define ICON_MIN_BTN_PATH  "://Data/Image/min-btn.png"
#define ICON_MAX_BTN_PATH  "://Data/Image/max-btn.png"
#define ICON_NORMAL_PATH  "://Data/Image/normal.png"

extern CcMusic* g_pMusic;
TitleBar::TitleBar(QWidget *parent)
    : QWidget(parent), m_bMove(false)
{
    // 去边框
    setWindowFlags(Qt::Dialog|Qt::WindowMinMaxButtonsHint|Qt::FramelessWindowHint);
    //
    initUi();
    //
}

TitleBar::~TitleBar()
{

}

void TitleBar::initUi()
{
    QHBoxLayout* layout = new QHBoxLayout;
    layout->setMargin(0);
    layout->setContentsMargins(10, 5, 10, 0);
    m_pLabelIcon = new QLabel;
    m_pLabelTitle = new QLabel("CcMusic 播放器");
    m_pLabelTitle->setObjectName("WindowTitle");
    m_pBtnMinimize = new QPushButton();
    m_pBtnMaximize = new QPushButton();
    m_pBtnClose = new QPushButton();

    QPixmap wndIcon = QPixmap(WND_ICON_PATH);
    m_pLabelIcon->setPixmap(wndIcon.scaled(18, 18));
    m_pBtnMinimize->setToolTip("最小化");
    m_pBtnMaximize->setToolTip("最大化");
    m_pBtnMaximize->setCheckable(true);
    m_pBtnClose->setToolTip("关闭");
    m_pBtnMaximize->setIcon(QIcon(ICON_MAX_BTN_PATH));
    m_pBtnMinimize->setIcon(QIcon(ICON_MIN_BTN_PATH));
    m_pBtnClose->setIcon(QIcon(ICON_SHHUT_DOWN_PATH));
    //
    m_pBtnMaximize->setFocusPolicy(Qt::NoFocus);
    m_pBtnMinimize->setFocusPolicy(Qt::NoFocus);
    m_pBtnClose->setFocusPolicy(Qt::NoFocus);
    //
    layout->addWidget(m_pLabelIcon);
    layout->addWidget(m_pLabelTitle);
    layout->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Expanding));
    layout->addWidget(m_pBtnMinimize);
    layout->addWidget(m_pBtnMaximize);
    layout->addWidget(m_pBtnClose);

    setLayout(layout);
    //
    connect(m_pBtnMinimize, &QPushButton::clicked, [](){g_pMusic->showMinimized();});
    connect(m_pBtnMaximize, &QPushButton::clicked, [&](bool isChecked)
    {
        m_pBtnMaximize->setIcon(QIcon(isChecked ? ICON_NORMAL_PATH : ICON_MAX_BTN_PATH));
        isChecked ? g_pMusic->showMaximized() : g_pMusic->showNormal();
        emit showWndMax(isChecked);
    });
    connect(m_pBtnClose, &QPushButton::clicked, [](){g_pMusic->OnClose();});
}

//
void TitleBar::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
    {
        m_bMove = true;
        m_movePosition = e->globalPos() - pos();
    }
    return QWidget::mousePressEvent(e);
}

//
void TitleBar::mouseReleaseEvent(QMouseEvent* e)
{
    if(e->button() == Qt::LeftButton)
    {
        m_bMove = false;
    }
    return QWidget::mouseReleaseEvent(e);
}

//
void TitleBar::mouseMoveEvent(QMouseEvent* e)
{
    if(m_bMove && g_pMusic)
    {
        QRect rect = g_pMusic->geometry();
        QPoint point = mapToGlobal(e->globalPos()- m_movePosition);
        g_pMusic->move(point);
        g_pMusic->GetPlayList()->setGeometry(QRect(g_pMusic->mapToGlobal(QPoint(rect.width(),0)), QSize(300, rect.height())));
        m_movePosition = e->globalPos() - pos();
    }
    return QWidget::mouseMoveEvent(e);
}
