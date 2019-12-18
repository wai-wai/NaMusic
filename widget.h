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


QT_BEGIN_NAMESPACE
namespace Ui {
    class Widget;
}
QT_END_NAMESPACE

class searchM;

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
private slots:
    void on_search_clicked();
    void getRData(QByteArray);
    void getDownLink(QByteArray);
#if 0
    void on_pushButton_clicked();
#endif
    void clickDownBtn();
    void playMp3();
    void setClipboard (QString url);

private:
    Ui::Widget *ui;
    searchM    findM;
    QByteArray musicData;
    QTableWidget *tabW;
    QString     curMusicName;
    QString     playUrl;
    int          maxLen;
    int          BtnNum;

    /* 播放mp3 */

};
#endif // WIDGET_H
