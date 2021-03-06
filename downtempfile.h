#ifndef DOWNTEMPFILE_H
#define DOWNTEMPFILE_H

#include <QObject>
#include <QStandardPaths>
#include <QFile>
#include "down/urlOp.h"
#include <QDir>

class downTempFile : public QObject
{
    Q_OBJECT
public:
    explicit downTempFile(QObject *parent = nullptr);
    void downMp3(QString url,QString musicName,QString  mtype);
signals:
    void downFinish(QString namePath);
    void downError(int error);
private slots:
    void FinishedToSave();
    void testError(int);
    //void loadProgress(qint64 bytesSent, qint64 bytesTotal);
private:
     urlOp *Downloader;
     QString downName;
     QString downType;
     QMap<QString, QString> requestheader;

};

#endif // DOWNTEMPFILE_H
