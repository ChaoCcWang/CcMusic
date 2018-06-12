#include "MusicPlaylist.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QFileInfo>
#include <QFile>
#include <QDomDocument>
#include "CcMusic.h"

#define CONFIG_PATH "../Data/Config/CcMusic.xml"
#define ICON_MINUS_PATH  "://Data/Image/minus.png"
#define ICON_PLUS_PATH  "://Data/Image/plus.png"

extern CcMusic* g_pMusic;
MusicPlaylist::MusicPlaylist(QMediaPlaylist *playList, QWidget *parent)
    : QWidget(parent), m_pMediaPlaylist(playList), m_lastDir("/")
{
    // 去边框
    setWindowFlags(Qt::Dialog|Qt::WindowMinMaxButtonsHint|Qt::FramelessWindowHint);
    //
    initUi();
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
    m_pPlayList->setViewMode(QListView::IconMode);
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
    m_arListWidgetItem.append(item);
    m_arSongList.append(filePath);
    //
    m_pMediaPlaylist->insertMedia(m_arListWidgetItem.size() - 1, QMediaContent(QUrl::fromLocalFile(filePath)));
    if(m_arListWidgetItem.size() == 1)
    {
        g_pMusic->SetCurrentMusic(0);
    }
}

void MusicPlaylist::OnBtnAddClick()
{
    QString filePath = QFileDialog::getOpenFileName(this, "选取音乐文件", m_lastDir, "MP3 (*.mp3 *.mp4)");
    m_lastDir = QFileInfo(filePath).absoluteDir().absolutePath();
    AddPlayList(filePath);
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
