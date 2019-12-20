#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    connect(&findM, &searchM::getRData, this, &Widget::getRData);
    connect(&findM, &searchM::getDownLink, this, &Widget::getDownLink);

    connect(ui->musicName, &QLineEdit::returnPressed, this, &Widget::on_search_clicked);

    connect(&downM,&downTempFile::downFinish, this, &Widget::playMp3ByUrl);
    connect(&downM,&downTempFile::downError, this, &Widget::downError);

    QHBoxLayout *hLayout = new QHBoxLayout();

    tabW = new QTableWidget();
    hLayout->addWidget(tabW);
    ui->musicList->setLayout(hLayout);

    tabW->hide();
    //ui->pushButton->hide();
    setWindowIcon(QIcon(":/new/prefix1/Music.ico"));
    setWindowTitle("音乐搜索. by smaller V1.1");

    this->setAttribute(Qt::WA_StyledBackground);
    this->setStyleSheet("QWidget#Widget{border-image: url(:/new/prefix1/back.jpg);}");

    /* 播放器 */
    //mPlayer = new QMediaPlayer(this);

    /* 初始化 数据库 */
    musicDb = new rwSqlite(mDb,this);
    curCount = 0;
    /*    ui->playBtn->hide();
    ui->nextBtn->hide();
    ui->lastBtn->hide();
*/
    ui->nextWidget->hide();
    mPlayer = nullptr;
}

Widget::~Widget()
{
    delete musicDb;
    delete ui;
}

void Widget::on_search_clicked()
{
    QString error;
    do{
        if(ui->musicName->text().isEmpty()){
            error = QString("请输入歌曲名字或演唱者");
            break;
        }
        if(ui->musicName->text().length() == 0 ){
            error = QString("请输入歌曲名字或演唱者");
            break;
        }

        /* 调用搜索歌曲 */
        curPage = 1;
        findhMusic(ui->musicName->text(),0);

    }while(0);

}


void Widget::findhMusic(QString name, int pos){
    //qDebug()<<QSslSocket::sslLibraryBuildVersionString();
    findM.findMusic(name,pos);
}

void Widget::getRData(QByteArray data){
    musicData.clear();
    musicData = data;
    /* 1. 解析json code： 200是获取成功 */
    /* 2. 继续解析json result 判断songCount 数量  建立列表使用*/
    /* 3. 解析详细的内容  歌曲名字 演唱者 id 音质 */
    do{

        if(musicData.isEmpty()) break;
        /* 解析json 这里只解析第一步跟第二步 */
        /* 第一步解析出返回200 获取成功 */
        QJsonParseError jsonError;
        QJsonDocument jsonDoc(QJsonDocument::fromJson(musicData, &jsonError));
        if(jsonError.error != QJsonParseError::NoError ){
            qDebug()<<"json error 1:"<< jsonError.errorString();
            break;
        }

        QJsonObject rootObj = jsonDoc.object();

        if( !rootObj.contains("code") ){
            break;
        }
        if(200 != rootObj.value("code").toInt()){
            QMessageBox::information(this,"错误提示1:","获取失败!\n");
            break;
        }
        /* 解析出获取到的数量 */
        if(!rootObj.contains("result")){
            QMessageBox::information(this,"错误提示2:","获取失败!\n");
            break;
        }
        QJsonValue value = rootObj.value("result");
        if(!value.isObject()){
            QMessageBox::information(this,"错误提示3:","获取失败!\n");
            break;
        }
        /* 解析result */
        QJsonObject songObj = value.toObject();
        if(!songObj.contains("songCount")){
            QMessageBox::information(this,"错误提示4:","获取失败!\n");
            break;
        }
        if(songObj.value("songCount").toInt() == 0){
            QMessageBox::information(this,"提示1:","未搜索到音乐!\n");
            break;
        }
        curCount = songObj.value("songCount").toInt();
        /* 默认一页100 */
        if(!songObj.contains("songs")){
            QMessageBox::information(this,"错误提示5:","获取失败!\n");
            break;
        }
        /* song list */
        QJsonValue songArray = songObj.value("songs");
        if(!songArray.isArray()){
            QMessageBox::information(this,"错误提示6:","获取失败!\n");
            break;
        }
        QJsonArray song = songArray.toArray();

        showTab(song, song.size());
    }while(0);
}


void Widget::showTab(QJsonArray data,int row){
    tabW->clear();
    if(row == 0) return;
    /* 5列 */
    QStringList headList;
    headList<<"曲名"\
           <<"演唱"\
          <<"时间"\
         <<"音质 | 大小"\
        <<"试听"\
       <<"下载" \
      <<"id";
    maxLen = headList.length();
    tabW->setColumnCount(headList.length());
    tabW->setRowCount(row);
    /* 隐藏id列 */
    tabW->setColumnHidden(headList.length()-1, true);
    /* 禁止编辑 */
    //tabW->setEditTriggers(QAbstractItemView::NoEditTriggers);
    QHeaderView* headerView = tabW->horizontalHeader();
    /*设置表格是否充满，即行末不留空*/
    headerView->setSectionResizeMode(0,QHeaderView::Stretch);
    headerView->setSectionResizeMode(1,QHeaderView::Stretch);
    headerView->setSectionResizeMode(2,QHeaderView::Stretch);
    headerView->setSectionResizeMode(3,QHeaderView::Stretch);
    headerView->setSectionResizeMode(4,QHeaderView::Stretch);
    headerView->setSectionResizeMode(5,QHeaderView::Stretch);
    tabW->setHorizontalHeaderLabels(headList);

    /* 解析json */
    /* 隐藏 垂直分割线*/
    tabW->setShowGrid(false);
    tabW->setStyleSheet("QTableWidget::Item{border:0px solid rgb(205, 201, 201); border-bottom:1px solid rgb(205, 201, 201);selection-background-color:lightblue}");
    /* 行交替显示颜色 */
    //tabW->setAlternatingRowColors(true);
    //tabW->setStyleSheet("alternate-background-color:rgb(205, 201, 201);background: white; color:rgb(205, 201, 201)");

    //tabW->setColumnWidth(0,300);

    tabW->resize(ui->musicList->size());

    /* 此处解析songs的列表 */
    /* 解析后交给tabW去显示 */

    QJsonArray songer;
    for(int i=0; i< data.size();i++){
        /* 曲名 */
        QTableWidgetItem *tabIteam = new QTableWidgetItem();
        tabIteam->setFlags(Qt::NoItemFlags);
        tabIteam->setText(data.at(i).toObject().value("name").toString());
        tabW->setItem(i,0,tabIteam);

        /* ar是数组 songer 还需要单独解析 */
        songer = data.at(i).toObject().value("ar").toArray();
        if(0 != songer.count()){
            QTableWidgetItem *songerIteam = new QTableWidgetItem();
            songerIteam->setFlags(Qt::NoItemFlags);
            songerIteam->setText(songer.at(0).toObject().value("name").toString());
            tabW->setItem(i,1,songerIteam);
        }

        /* 提取时常 */
        QTableWidgetItem *dtIteam = new QTableWidgetItem();
        dtIteam->setFlags(Qt::NoItemFlags);
        QString timeStr;
        QTime time = QTime::fromMSecsSinceStartOfDay(data.at(i).toObject().value("dt").toInt());
        timeStr = time.toString("mm:ss");

        dtIteam->setText(timeStr);
        tabW->setItem(i,2,dtIteam);

        /* 音乐品质也需要列出来  h  m  l */
        QStringList list;
        QString info;
        float sizeM = 0;
        if(data.at(i).toObject().value("h").toObject().value("br").toInt() != 0){
            info.clear();
            sizeM = float(data.at(i).toObject().value("h").toObject().value("size").toInt()/1024);
            sizeM /= 1024;
            info = QString::number(data.at(i).toObject().value("h").toObject().value("br").toInt()) +" | " + \
                   QString::number(sizeM);
            list<< info;
        }

        if(data.at(i).toObject().value("m").toObject().value("br").toInt() != 0){
            info.clear();
            sizeM = float(data.at(i).toObject().value("m").toObject().value("size").toInt()/1024);
            sizeM /= 1024;
            info = QString::number(data.at(i).toObject().value("m").toObject().value("br").toInt()) +" | " + \
                   QString::number(sizeM);
            list<< info;

        }
        if(data.at(i).toObject().value("l").toObject().value("br").toInt() != 0){
            info.clear();
            sizeM = float(data.at(i).toObject().value("m").toObject().value("size").toInt()/1024);
            sizeM /= 1024;
            info = QString::number(data.at(i).toObject().value("m").toObject().value("br").toInt()) +" | " + \
                   QString::number(sizeM);
            list<< info;
        }


        QComboBox *pMusic = new QComboBox();
        pMusic->addItems(list);
        tabW->setCellWidget(i,3,pMusic);

        QPushButton *playBtn = new QPushButton("播放");
        connect(playBtn, &QPushButton::clicked, this, &Widget::playMusic);
        tabW->setCellWidget(i, 4, playBtn);

        QPushButton *donwBtn = new QPushButton("复制下载链接");
        connect(donwBtn, &QPushButton::clicked, this, &Widget::clickDownBtn);
        tabW->setCellWidget(i, maxLen-2, donwBtn);

        QTableWidgetItem *idIteam = new QTableWidgetItem();
        idIteam->setFlags(Qt::NoItemFlags);
        idIteam->setText(QString::number(data.at(i).toObject().value("id").toInt()));
        tabW->setItem(i,headList.length()-1,idIteam);
    }

    tabW->show();
    ui->curPage->setText(QString::number(curPage));
    ui->countPage->setText(QString::number( sumPos(curCount)));

    ui->nextWidget->show();
}

void Widget::clickDownBtn(){
    QPushButton *senderObj = qobject_cast<QPushButton*>(sender());
    if(senderObj == nullptr) return;
    QModelIndex index = tabW->indexAt(QPoint(senderObj->frameGeometry().x(),senderObj->frameGeometry().y()));

    /* 通过索引 获取对应的 */
    //tabW->indexWidget(index);
    //tabW->cellWidget()
    QComboBox *curBox = qobject_cast<QComboBox* > (tabW->cellWidget(index.row(),3));
    /* 取出音质 */
    QString  pStr = curBox->currentText().trimmed().simplified().split("|").at(0);
    /* 取出 id */
    QString idStr = tabW->item(index.row(),maxLen-1)->text();
    curMusicName = tabW->item(index.row(),0)->text();

    /* 取出 songer = curSonger */
    curSonger = tabW->item(index.row(),1)->text();
    /* 将音质 id设置过去 直接获取下载链接 */
    BtnNum = 1;
    findM.findDownLink(idStr,pStr.trimmed().simplified());

}

void Widget::playMusic(){
    static bool ok = false;

    QPushButton *senderObj = qobject_cast<QPushButton*>(sender());
    if(senderObj == nullptr) return;
    QModelIndex index = tabW->indexAt(QPoint(senderObj->frameGeometry().x(),senderObj->frameGeometry().y()));

    /* 播放按钮改 暂停 */
    if(ok == false){
        QComboBox *curBox = qobject_cast<QComboBox* > (tabW->cellWidget(index.row(),3));
        /* 取出最低音质试听 */
        QString pStr = curBox->itemText(curBox->count() -1 ).trimmed().simplified().split("|").at(0);

        /* 取出 id */
        QString idStr = tabW->item(index.row(),maxLen-1)->text();

        curMusicName = tabW->item(index.row(),0)->text();

        /* 取出 songer */
        curSonger = tabW->item(index.row(),1)->text();

        BtnNum = 0;
        findM.findDownLink(idStr,pStr.trimmed().simplified());
        senderObj->setText("暂停");
        ok = true;
    }else{
        stopMp3();
        senderObj->setText("播放");

        ok = false;
    }
}

void Widget::getDownLink(QByteArray data){
    /* 解析json */
    musicData.clear();
    musicData = data;
    curDownUrl.clear();
    do{
        /*
         * 判断返回值 200
         * data 是jsonArray  单独处理
        */
        if(musicData.isEmpty()) break;
        currentMd5.clear();
        QJsonParseError jsonError;
        QJsonDocument rootDoc(QJsonDocument::fromJson(musicData,&jsonError));
        if(jsonError.error != QJsonParseError::NoError ){
            qDebug()<<"getDownLink json error 1:"<< jsonError.errorString();
            break;
        }

        QJsonObject rootObj = rootDoc.object();

        if(!rootObj.contains("code")){
            break;
        }

        if(200 != rootObj.value("code").toInt()){
            QMessageBox::information(this,"getDownLink,错误提示1:","获取失败!\n");
            break;
        }

        if(!rootObj.contains("data")){
            QMessageBox::information(this,"getDownLink,错误提示2:","获取失败!\n");
            break;
        }

        if(!rootObj.value("data").isArray()){
            QMessageBox::information(this,"getDownLink,错误提示3:","获取失败!\n");
            break;
        }
        QJsonArray rootArray = rootObj.value("data").toArray();

        if(rootArray.count() == 0){
            QMessageBox::information(this,"getDownLink,提示4:","获取失败!\n");
            break;
        }
        curDownUrl = rootArray.at(0).toObject().value("url").toString();
        curMType = rootArray.at(0).toObject().value("type").toString();
        currentMd5 = rootArray.at(0).toObject().value("md5").toString();

        curDownUrl += ("?filename=" + QUrl::toPercentEncoding(curMusicName) + "&downloadtype=" + curMType);

        if(BtnNum == 1){
            setClipboard(curDownUrl);
        }else if(BtnNum == 0){
            /* 启动播放器试听 */
            /* 播放rul */
            /* 需要添加md5判断以后在进行下载 */
            if(musicDb->findMd5(currentMd5, curMusicName + "-" + curSonger)){
                playEnable = true;
                playMp3();
                break;
            }
            downM.downMp3(curDownUrl, curMusicName + "-" + curSonger, curMType);
        }
        /* 发送给下载线程 */
    }while(0);
}

void Widget::playMp3ByUrl(QString ){
    /* 下载成功 添加md5 跟 musicname */
    musicDb->addMd5(currentMd5, curMusicName + "-" + curSonger);
    playEnable = true;
    playMp3();
}

void Widget::downError(int){
    playEnable = false;
}


void Widget::stopMp3(){
    /* stop player */
    mPlayer->stop();
    delete  mPlayer;
    mPlayer = nullptr;
    playEnable = false;
    ui->playBtn->setText("播放");
}

void Widget::playMp3(){

    if(!playEnable){
        return;
    }
    /* 文件存在就播放 仅判断文件名 */
    QString URL= defSavePath + curMusicName + "-" + curSonger + "." + curMType;
    QFileInfo  fInfo(URL);
    if(!fInfo.isFile()){
        musicDb->delMusicByName(curMusicName + "-" + curSonger);
        downM.downMp3(curDownUrl, curMusicName + "-" + curSonger, curMType);
        return;
    }

    mPlayer = new QMediaPlayer();
    /* player */
    mPlayer->setMedia(QUrl::fromLocalFile(URL));
    mPlayer->play();
    ui->playBtn->setText("暂停");
}

void Widget::setClipboard (QString url){
    QClipboard *clicpboard = QApplication::clipboard();
    clicpboard->setText(url);
}

int Widget::sumPos(int count){
    int ret = 0;

    if(count%100){
        ret = (int)(count/100) +1;
    }else{
        ret = (int)(count/100);
    }
    return ret;
}


/* 下一页 */
void Widget::on_nextBtn_clicked()
{
    ui->nextBtn->setEnabled(false);
    /*
     *   ui->playBtn->hide();
        ui->nextBtn->hide();
        ui->lastBtn->hide();
    */
    do {
        int countPage = sumPos(curCount);
        if(curPage == countPage){
            break;
        }

        if(ui->musicName->text().isEmpty()){
            break;
        }
        if(ui->musicName->text().length() == 0 ){
            break;
        }
        curPage++;
        /* 调用搜索歌曲 */
        findhMusic(ui->musicName->text(),(int)((curPage-1)*100));
    } while (0);
    ui->nextBtn->setEnabled(true);
}

/*上一页 */
void Widget::on_lastBtn_clicked()
{
    ui->lastBtn->setEnabled(false);
    /*
     *   ui->playBtn->hide();
        ui->nextBtn->hide();
        ui->lastBtn->hide();
    */
    do{
        if(curPage == 1){
            break;
        }
        if(ui->musicName->text().isEmpty()){
            break;
        }
        if(ui->musicName->text().length() == 0 ){
            break;
        }
        curPage--;
        /* 调用搜索歌曲 */
        findhMusic(ui->musicName->text(),(curPage == 1)?(0):((curPage-1)*100));

    }while(0);
    ui->lastBtn->setEnabled(true);
}

/* 播放暂停公用 */
void Widget::on_playBtn_clicked()
{
    if(mPlayer == nullptr) return;
    /* stop player */
    mPlayer->stop();
    delete  mPlayer;
    mPlayer = nullptr;
    playEnable = false;
    ui->playBtn->setText("播放");
}
