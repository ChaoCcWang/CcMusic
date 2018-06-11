#include "CcMusic.h"
#include <QApplication>
#include <QProcess>
#include <QList>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // 加载LAV
    QProcess p;
    QString exePath = QCoreApplication::applicationDirPath();
    exePath.replace("\\", "/");
    //
    QList<QStringList> listParam;
    listParam << (QStringList() << "/c" << exePath + "../../../LAV/install_audio.bat");
    listParam << (QStringList() << "/c" << exePath + "../../../LAV/install_splitter.bat");
    listParam << (QStringList() << "/c" << exePath + "../../../LAV/install_video.bat");
    for(QList<QStringList>::iterator it = listParam.begin(); it != listParam.end(); it++)
    {
        QFile f(QString("%1_%2").arg((*it)[1]).arg(".log"));
        if(!f.exists())
        {
            p.start("cmd.exe", *it);
            if(p.waitForStarted() && f.open(QIODevice::ReadWrite))
            {
                p.waitForFinished();
                f.write(p.readAllStandardOutput());
                f.close();
            }
        }
    }
    //
    CcMusic w;
    w.show();

    return a.exec();
}
