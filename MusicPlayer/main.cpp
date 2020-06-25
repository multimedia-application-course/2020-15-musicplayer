#include "playershow.h"
#include <QApplication>
#include "databasecon.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    if(!createConn())return 0;//连接数据库
    PlayerShow w;
    w.show();
    return a.exec();
}
