#include "urlop.h"


urlOp::urlOp(QObject *parent) : QObject(parent)
{
    manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),
                this, SLOT(replyFinished(QNetworkReply*)));

}


void urlOp::setOp(const QMap<QString, QString>& requestheader, const QByteArray& opdata,  QUrl url)
{
    QNetworkRequest request;

    request.setUrl(url);
    QSslConfiguration config;
    config.setPeerVerifyMode(QSslSocket::VerifyNone);
    config.setProtocol(QSsl::TlsV1SslV3);//设置ssl
    request.setSslConfiguration(config);

    QMap<QString, QString>::const_iterator iterator;
    for(iterator = requestheader.begin(); iterator != requestheader.end(); iterator++)
        request.setRawHeader(iterator.key().toLocal8Bit(), iterator.value().toLocal8Bit());


    if(opdata == "")
      reply = manager->get(request);
    else
      reply = manager->post(request, opdata);
 //   connect(reply, SIGNAL(downloadProgress(qint64 ,qint64)), this, SLOT(loadProgress(qint64 ,qint64)));


}


QByteArray  urlOp::getResponseCookieByKey(const QByteArray& key)
{
    return responseCookie[key];
}
QByteArray urlOp::getUrlData()
{
    return urlData;
}
int urlOp::getResponseStatus()
{
    return status;
}
QString urlOp::getErrorString()
{
    return errString;
}


void urlOp::replyFinished(QNetworkReply *reply)
{
    status = reply->error();
    //qDebug() << status;
    errString = reply->errorString();
    //qDebug()<<errString;
    if((reply != NULL) && (reply->error() == QNetworkReply::NoError))
    {
        urlData = reply->readAll();
        QList<QByteArray> responseRawHeader = reply->rawHeaderList();
        QByteArray tmp;
        for(int i = 0; i <responseRawHeader.count(); i++)
        {
            tmp = responseRawHeader.at(i);
            responseCookie.insert(tmp, reply->rawHeader(tmp));
            //qDebug() << tmp  << ":" <<reply->rawHeader(tmp);
        }
        //qDebug() << "getFinished";

        emit this->getFinished();
    }
    else
        emit this->error((int)reply->error());


    reply->close();
}
