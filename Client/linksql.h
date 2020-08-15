#ifndef LINKSQL_H
#define LINKSQL_H
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>

class LinkSql
{
  public:
    LinkSql()
    {
        db = QSqlDatabase::addDatabase("QMYSQL");
        db.setHostName("127.0.0.1");
        db.setPort(3306);
        db.setDatabaseName("im");
        db.setUserName("root");
        db.setPassword("123456");
        db.open();
        if(!db.open())
        {
            MyToast *myToast = new MyToast(QString::fromLocal8Bit("数据库连接失败"),1000,this);
            myToast->show();
            return;
        }
    }
    ~LinkSql();
private:
    QSqlDatabase db;
};

#endif // LINKSQL_H
