#ifndef URLOP_H
#define URLOP_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QMap>
#include <QProgressBar>

class urlOp : public QObject
{
    Q_OBJECT

public:
    explicit urlOp(QObject *parent = 0);
    void setOp(const QMap<QString, QString>& requestheader, const QByteArray& postdata, QUrl url);
    QByteArray getUrlData();
    QByteArray getResponseCookieByKey(const QByteArray& key);
    int getResponseStatus();
    QString getErrorString();
  QNetworkReply *reply;

signals:
    void getFinished();
    void error(int );


private slots:
    void replyFinished(QNetworkReply *reply);


private:
    QNetworkAccessManager* manager;
    QByteArray urlData;
    QString  errString;
    int status;
    QMap<QByteArray, QByteArray>  responseCookie;

};


#endif // URLOP_H
