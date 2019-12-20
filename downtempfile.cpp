#include "downtempfile.h"

downTempFile::downTempFile(QObject *parent) : QObject(parent)
{
    Downloader = new urlOp(this);
    connect(Downloader, SIGNAL(getFinished()),this, SLOT(FinishedToSave()));
    connect(Downloader, SIGNAL(error(int)), this, SLOT(testError(int)));
#if 0
    requestheader.insert("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8");
    requestheader.insert("Accept-Encoding", "gzip, deflate, br");
    requestheader.insert("Accept-Language", "zh-CN,zh;q=0.9");
    requestheader.insert("Connection", "keep-alive");
    requestheader.insert("Host", "qd.myapp.com");
    requestheader.insert("Referer", "https://im.qq.com/pcqq/");
    requestheader.insert("Upgrade-Insecure-Requests", "1");
    requestheader.insert("User-Agent", "Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/69.0.3497.100 Mobile Safari/537.36");
#endif
     //connect(Downloader->reply,SIGNAL(downloadProgress(qint64,qint64)),this,SLOT(loadProgress(qint64,qint64)));
}

void downTempFile::downMp3(QString url,QString musicName, QString  mtype){
    Downloader->setOp(requestheader, "",QUrl(url));
    downName = musicName;
    downType = mtype;
}

void downTempFile::FinishedToSave(){
    QString filename = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/music";
    QDir dir(filename);
    if(!dir.exists()){
        dir.mkdir(filename);
    }
    filename += ("/" + downName + "." + downType);
    QFile file(filename);
    if(!file.open(QIODevice::Append))
    {
        qDebug() << QString("%1 Open error").arg(filename);
        return;
    }

    file.write(Downloader->getUrlData());
    file.close();
    emit downFinish(filename);
}

void downTempFile::testError(int error){
    emit downError(error);
}

#if 0
void downTempFile::loadProgress(qint64 bytesSent, qint64 bytesTotal){
    qDebug() << "loaded" << bytesSent/1024 << "KB of" << bytesTotal/1024 <<"KB";
}
#endif
