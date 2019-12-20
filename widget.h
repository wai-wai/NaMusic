#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLayout>
#include <QPushButton>
#include <QLabel>


#include "searchm.h"
#include "rwsqlite.h"

#include <QByteArray>
#include <QTableWidget>
#include <QHeaderView>


#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>

#include <QMessageBox>
#include <QComboBox>
#include <QFileDialog>
#include <QModelIndex>
#include <QPoint>
#include <QTime>
#include <QClipboard>
#include <QPixmap>
#include <QPalette>
#include <QBrush>

#include <QMediaPlayer>
#include "downtempfile.h"
#include <QMetaType>
#include <QFileInfo>

QT_BEGIN_NAMESPACE
namespace Ui {
    class Widget;
}
QT_END_NAMESPACE

class searchM;
class downTempFile;
class rwSqlite;

const QString defSavePath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/music/";
const QString configFile = "conf.ini";
const QString musicDbFile = "db.db";
const QString mDb = defSavePath + musicDbFile;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    void findhMusic(QString);

private:
    void showTab(QJsonArray, int row);
    void playMp3ByUrl(QString);
    void stopMp3();
    void playMp3();
private slots:
    void on_search_clicked();
    void getRData(QByteArray);
    void getDownLink(QByteArray);
    void downError(int);
#if 0
    void on_pushButton_clicked();
#endif
    void clickDownBtn();
    void playMusic();
    void setClipboard (QString url);

private:
    Ui::Widget *ui;
    searchM    findM;
    downTempFile    downM;
    QByteArray musicData;
    QTableWidget *tabW;
    QString      currentMd5;
    QString     curMusicName;
    QString     curSonger;
    QString     curMType;
    QString     curDownUrl;
    bool        playEnable;
    QString     playUrl;
    int          maxLen;
    int          BtnNum;

    /* 播放mp3 */
    QMediaPlayer    *mPlayer;

    rwSqlite     *musicDb;
};
#endif // WIDGET_H
