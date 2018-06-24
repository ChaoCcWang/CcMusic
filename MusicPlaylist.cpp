#include "MusicPlaylist.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QFileInfo>
#include <QFile>
#include <QDomDocument>
#include <QAction>
#include <QLineEdit>
#include "CcMusic.h"

#define CONFIG_PATH "../Data/Config/CcMusic.xml"
#define ICON_MINUS_PATH  "://Data/Image/minus.png"
#define ICON_PLUS_PATH  "://Data/Image/plus.png"
#define ICON_AUDIO_PATH  "://Data/Image/audio.png"
#define ICON_VIDEO_PATH  "://Data/Image/video.png"

extern CcMusic* g_pMusic;
MusicPlaylist::MusicPlaylist(QMediaPlaylist *playList, QWidget *parent)
    : QWidget(parent), m_pMediaPlaylist(playList), m_lastDir("/")
{
    // 去边框
    setWindowFlags(Qt::Dialog|Qt::WindowMinMaxButtonsHint|Qt::FramelessWindowHint);
    //
    initUi();
    //
    initMenu();
    //
    ReadConf();
}

MusicPlaylist::~MusicPlaylist()
{

}

void MusicPlaylist::initUi()
{
    //
    m_pBtnAdd = new QPushButton();
    m_pBtnAdd->setIcon(QIcon(ICON_PLUS_PATH));
    m_pBtnDel = new QPushButton();
    m_pBtnDel->setIcon(QIcon(ICON_MINUS_PATH));
    m_pPlayList = new QListWidget;
    //
    QHBoxLayout* toplayout = new QHBoxLayout;
    toplayout->setMargin(0);
    toplayout->addItem(new QSpacerItem(20, 20, QSizePolicy::Expanding));
    toplayout->addWidget(m_pBtnAdd);
    toplayout->addWidget(m_pBtnDel);

    QHBoxLayout* bottomLayout = new QHBoxLayout;
    bottomLayout->setMargin(0);
    bottomLayout->addWidget(m_pPlayList);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addLayout(toplayout);
    layout->addLayout(bottomLayout);

    setLayout(layout);

    connect(m_pBtnAdd, &QPushButton::clicked, this, &MusicPlaylist::OnBtnAddClick);
    connect(m_pBtnDel, &QPushButton::clicked, this, &MusicPlaylist::OnBtnDelClick);
    connect(m_pPlayList, &QListWidget::itemDoubleClicked, this, &MusicPlaylist::OnListItemDoubleClick);
}

void MusicPlaylist::ReadConf()
{
    QFile xml(CONFIG_PATH);
    QDomDocument doc;
    if(!xml.open(QIODevice::ReadOnly)) return;
    if(doc.setContent(&xml))
    {
        xml.close();
        QDomNodeList listNode = doc.elementsByTagName("Song");
        for(int i = 0; i < listNode.size(); i++)
        {
            QDomElement ele = listNode.item(i).toElement();
            AddPlayList(ele.text());
        }
    }
}

void MusicPlaylist::AddPlayList(QString filePath)
{
    if(filePath.isEmpty()) return;
    filePath.replace("\\", "/");
    if(m_arSongList.contains(filePath)) return;

    QString fileName = QFileInfo(filePath).fileName();
    QListWidgetItem* item = new QListWidgetItem(fileName);
    m_pPlayList->addItem(item);
    //
    QIcon icon(filePath.contains(".mp3") ? ICON_AUDIO_PATH : ICON_VIDEO_PATH);
    item->setIcon(icon);
    //
    m_arListWidgetItem.append(item);
    m_arSongList.append(filePath);
    //
    QUrl url;
    if(filePath.startsWith("http://"))
    {
        url.setUrl(filePath);
    }
    else
    {
        url = QUrl::fromLocalFile(filePath);
    }

    QMediaContent mediaContent(url);
    m_pMediaPlaylist->insertMedia(m_arListWidgetItem.size() - 1, mediaContent);
    if(m_arListWidgetItem.size() == 1)
    {
        g_pMusic->SetCurrentMusic(0);
    }
}

void MusicPlaylist::initMenu()
{
    m_pMenu = new QMenu(this);

    QAction* addLocalFileAction = new QAction("添加文件", this);
    QAction* addLocalDirAction = new QAction("添加文件夹", this);
    QAction* addNetFileAction = new QAction("添加网络文件", this);
    m_pMenu->addAction(addLocalFileAction);
    m_pMenu->addAction(addLocalDirAction);
    m_pMenu->addAction(addNetFileAction);

    connect(addLocalFileAction, &QAction::triggered, this, &MusicPlaylist::OnAddFile);
    connect(addLocalDirAction, &QAction::triggered, this, &MusicPlaylist::OnAddDir);
    connect(addNetFileAction, &QAction::triggered, this, &MusicPlaylist::OnAddNetFile);
}

void MusicPlaylist::OnBtnAddClick()
{
    m_pMenu->exec(QCursor::pos());

}

void MusicPlaylist::OnBtnDelClick()
{
    QListWidgetItem* item = m_pPlayList->currentItem();
    int index = m_arListWidgetItem.indexOf(item);
    m_pMediaPlaylist->removeMedia(index);
    m_arListWidgetItem.removeAt(index);
    m_arSongList.removeAt(index);
    m_pPlayList->takeItem(index);
}

void MusicPlaylist::OnListItemDoubleClick(QListWidgetItem *item)
{
    int index = m_arListWidgetItem.indexOf(item);
    g_pMusic->SetCurrentMusic(index);

    emit playMusic();
}

// 添加文件
void MusicPlaylist::OnAddFile()
{
    QString filePath = QFileDialog::getOpenFileName(this, "选取文件", m_lastDir, "音频//视频 (*.mp3 *.mp4 *.avi *.rmvb)");
    if(!filePath.isEmpty())
    {
        m_lastDir = QFileInfo(filePath).absoluteDir().absolutePath();
        AddPlayList(filePath);
    }
}

// 添加文件夹
void MusicPlaylist::OnAddDir()
{
    QString dirPath = QFileDialog::getExistingDirectory(this, "选取文件夹", "/");
    if(!dirPath.isEmpty())
    {
        QFileInfoList infoList = QDir(dirPath).entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
        for(QFileInfoList::iterator it = infoList.begin(); it != infoList.end(); it++)
        {
            if(it->suffix() == "mp3" ||
                it->suffix() == "mp4" ||
                it->suffix() == "avi" ||
                it->suffix() == "rmvb" ||
                it->suffix() == "flv")
            {
                AddPlayList(it->absoluteFilePath());
            }
        }
    }
}

// 添加网络文件
void MusicPlaylist::OnAddNetFile()
{
    QDialog* wgt = new QDialog;
    wgt->setWindowFlags(Qt::WindowMinMaxButtonsHint|Qt::FramelessWindowHint);
    QHBoxLayout* hlayout = new QHBoxLayout();
    QLineEdit* lineEdit = new QLineEdit();
    QPushButton* sureBtn = new QPushButton("确定");
    QPushButton* cancelBtn = new QPushButton("取消");
    hlayout->addItem(new QSpacerItem(20, 20, QSizePolicy::Expanding));
    hlayout->addWidget(sureBtn);
    hlayout->addWidget(cancelBtn);
    QVBoxLayout* vLayout = new QVBoxLayout();
    vLayout->addWidget(lineEdit);
    vLayout->addLayout(hlayout);
    wgt->setLayout(vLayout);

    //
    wgt->setGeometry(geometry().x(), geometry().y() + 35, geometry().width(), 60);
    connect(sureBtn, &QPushButton::clicked, [=]()
    {
        QString filePath = lineEdit->text();
        if(!filePath.isEmpty())
        {
            AddPlayList(filePath);
        }
        wgt->close();
    });
    connect(cancelBtn, &QPushButton::clicked, [&]()
    {
        wgt->close();
    });
    wgt->exec();
    delete cancelBtn;
    delete sureBtn;
    delete lineEdit;
    delete hlayout;
    delete vLayout;
    delete wgt;
}

// 回写配置文件
void MusicPlaylist::WriteConf()
{
    QFile xml(CONFIG_PATH);
    QDomDocument doc;
    if(!xml.open(QIODevice::ReadOnly)) return;
    if(doc.setContent(&xml))
    {
        xml.close();
        QDomElement root = doc.documentElement();
        QDomNode p_n = root.firstChild();
        while (!p_n.isNull())
        {
            QDomElement p_e = p_n.toElement();
            if(p_e.tagName() == "PlayList")
            {
                root.removeChild(p_n);
            }
            p_n = p_n.nextSibling();
        }
        QDomElement playListEle = doc.createElement("PlayList");
        root.appendChild(playListEle);
        for(QList<QString>::iterator it = m_arSongList.begin(); it != m_arSongList.end(); it++)
        {
            QDomElement songEle = doc.createElement("Song");
            QDomText node = doc.createTextNode(*it);
            songEle.appendChild(node);
            playListEle.appendChild(songEle);
        }
        if(!xml.open(QIODevice::WriteOnly)) return;
        QTextStream out(&xml);
        out.setCodec("UTF-8");
        doc.save(out, 4, QDomNode::EncodingFromTextStream);
        xml.close();
    }
}
