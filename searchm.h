#ifndef SEARCHM_H
#define SEARCHM_H

#include <QObject>
#include <QUrl>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QSslConfiguration>
#include <QSslSocket>
#include <QSsl>
#include <QEventLoop>
#include <QByteArray>

const QString url0= "https://api.imjad.cn/cloudmusic/?type=search&search_type=1&limit=100&s=";
const QString url1 = "&offset=0";/* offset每次获取30个 可用于计算总共多少 */

const QString url2 = "https://api.imjad.cn/cloudmusic/?type=detail&id=";

const QString url3 = "https://api.imjad.cn/cloudmusic/?type=song&id=168277&br=128000";



class searchM : public QObject
{
    Q_OBJECT
public:
    explicit searchM(QObject *parent = nullptr);

    void findMusic(QString name);
    void findDownLink(QString id, QString Quality);
private:

    void setUrl(QString url);
    void getJson(QByteArray *json);

signals:
    void getRData(QByteArray);
    void getDownLink(QByteArray);
private:
    QString musicName;
    QUrl    musicUrl;
};

#endif // SEARCHM_H
