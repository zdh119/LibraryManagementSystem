#ifndef DATABASEOPERATION_H
#define DATABASEOPERATION_H
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QTableView>
class DatabaseOperation
{
public:
    DatabaseOperation();
    //检索账户，如果数据库中有该账户，则返回true
    static bool selectAccount(QString account);
    //登录验证账户、密码、权限
    static bool checkAccount(QString account, QString password, QString permission);
    //添加账户
    static bool addAccount(QString account, QString password, QString permission);
    //更改密码
    static bool changePassword(QString account, QString password);
    //用户借书
    static bool borrowBook(QString account, int bookID, QString borrowDate, QString returnDate);
    //检索借书记录
    static bool selectBorrowInfo(int bookID, QString account, QString *bookName, QString *borrowDate, QString *returnDate);
    //用户还书
    static bool returnBook(QString account, int bookID);
    //管理员添加书籍
    static bool addBook(QString bookName, QString authorName, QString pressName, QString publishDate, double price, QString ISBN, QString categeory, QString outline, int totalNumber);
    //在表格中显示图书信息数据
    static QSqlTableModel* tableShow(QString table, QString filter, QTableView *tableView, QSqlDatabase db);
};

#endif // DATABASEOPERATION_H
