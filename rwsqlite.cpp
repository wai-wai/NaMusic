#include "rwsqlite.h"

rwSqlite::rwSqlite(QString dbFilePath, QObject *parent) :  QObject(parent),dbPath(dbFilePath)
{
    dataBase = QSqlDatabase::addDatabase("QSQLITE");
    dataBase.setDatabaseName(dbPath);

    if(!dataBase.open()){
        qDebug()<<"open db Error: "<<dataBase.lastError();
        return;
    }
    QSqlQuery sql;
    /* 判断一下是否为空 */
    QString findTable = "select count(*)  from sqlite_master where type='table' and name = 'musicList'";
    //
    /* 创建表格 */
    if(!sql.exec(findTable)){
        qDebug()<<"findTable error:"<< sql.lastError().text();
        return;
    }
    int count = 0;
    while(sql.next()){
       count = sql.value(0).toInt();
       break;
    }
    if(count == 0){
        QString creatTab = "CREATE TABLE musicList(ID INTEGER  PRIMARY KEY, NAME TEXT NOT NULL,  MD5 TEXT NOT NULL)";
        if(!sql.exec(creatTab)){
            qDebug()<<"creatTab error: "<< sql.lastError().text();
            return;
        }
    }
}

bool  rwSqlite::findMd5(QString md5, QString name){
    bool ret = false;
    do{
        if(md5.isEmpty() | name.isEmpty()){
            break;
        }
        QString findInfo = QString("select count(*)  from musicList where MD5='%1' and NAME = '%2'").arg(md5).arg(name);
        QSqlQuery sql;
        if(!sql.exec(findInfo)){
            qDebug()<<"findMd5 error:"<< sql.lastError().text();
            ret = false;
            break;
        }
        int count = 0;
        while(sql.next()){
            count =  sql.value(0).toInt();
            break;
        }
        if(count == 0){
            break;
        }
        ret = true;

    }while(0);
    return ret;
}

void  rwSqlite::addMd5(QString md5, QString name ){
    QString insMd5 = QString("INSERT INTO musicList(ID,NAME,MD5) VALUES(NULL,'%1','%2')").arg(name).arg(md5);
    QSqlQuery sql;
    if(!sql.exec(insMd5)){
        qDebug()<<"insMd5 error:"<< sql.lastError().text();
        return;
    }
}

bool  rwSqlite::delMusicByName(QString name){
    bool ret = false;
    do{
        if(name.isEmpty()) break;
        QString delMusic = QString("DELETE FROM musicList WHERE NAME = '%1'").arg(name);
        QSqlQuery sql;
        if(!sql.exec(delMusic)){
         qDebug()<<"delMusicByName error:"<< sql.lastError().text();
                break;
        }
        ret = true;
    }while(0);
    return ret;
}

bool  rwSqlite::delmusicByMd5(QString md5){
    bool ret = false;
    do{
        if(md5.isEmpty()) break;
        QString delMusic = QString("DELETE FROM musicList WHERE MD5 = '%1'").arg(md5);
        QSqlQuery sql;
        if(!sql.exec(delMusic)){
         qDebug()<<"delmusicByMd5 error:"<< sql.lastError().text();
                break;
        }
        ret = true;
    }while(0);
    return ret;
}

bool  rwSqlite::clear(){

    bool ret = false;
    do{
        QString clearTab = "delete from musicList";
        QSqlQuery sql;
        if(!sql.exec(clearTab)){
         qDebug()<<"clearTab error:"<< sql.lastError().text();
                break;
        }
        ret = true;
    }while(0);
    return ret;
}
