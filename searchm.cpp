#include "searchm.h"

searchM::searchM(QObject *parent) : QObject(parent)
{

}

void searchM::findMusic(QString name, int pos){
    /* http请求 获取json */
/*
 * https://api.imjad.cn/cloudmusic/?type=search&search_type=1&limit=1024&s=水木年华&offset=0
*/
    QString url = url0 + QUrl::toPercentEncoding(name) + "&offset=" + QString::number(pos);

    setUrl(url);
    QByteArray json;

    getJson(&json);
    /* 通知界面 显示列表 */
    emit getRData(json);
}


void searchM::setUrl(QString url){
    musicUrl.setUrl(url);
}

void searchM::getJson(QByteArray *json){
    QNetworkAccessManager mMan;

    QNetworkRequest request;
    request.setUrl(musicUrl);

    QSslConfiguration config;
    config.setPeerVerifyMode(QSslSocket::VerifyNone);
    config.setProtocol(QSsl::TlsV1SslV3);//设置ssl

    request.setSslConfiguration(config);

    QNetworkReply *reply = mMan.get(request);

    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);

    loop.exec();
    QByteArray data =  reply->readAll();
    json->append(data);
}

/*
    传递 id  音质 然后getJson处理
*/

void searchM::findDownLink(QString id, QString Quality){
    QString url = QString("https://api.imjad.cn/cloudmusic/?type=song&id=%1&br=%2").arg(id).arg(Quality);
    setUrl(url);

    QByteArray json;
    getJson(&json);

    emit getDownLink(json);
}
