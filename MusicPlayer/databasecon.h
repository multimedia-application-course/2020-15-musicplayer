#ifndef DATABASECON_H
#define DATABASECON_H
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QVariant>
bool createConn()
{
    QSqlDatabase db=QSqlDatabase::addDatabase("QSQLITE");//新建数据库
    db.setDatabaseName("Music.db");//数据库命名为Music.db
    if(!db.open())return false;//如果打开失败就返回
    QSqlQuery query;//
    query.exec(QString("create table AllTable (TableName varchar primary key)"));//新建一个标签存放所有歌单
    if(!query.next())
    {
        query.exec(QString("create table Local (MusicPath varchar,MusicName varchar,primary key(MusicPath,MusicName))"));//默认一个本地音乐的歌单
        query.exec(QString("insert into AllTable values('Local')"));//插入歌单标签
        query.exec(QString("create table Search (MusicPath varchar,MusicName varchar,primary key(MusicPath,MusicName))"));//用来搜索的标签
    }
    return true;
}
#endif // DATABASECON_H
