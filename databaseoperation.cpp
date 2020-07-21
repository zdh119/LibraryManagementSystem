#include "databaseoperation.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QMessageBox>
#include <QDebug>
#include <QVariantList>
DatabaseOperation::DatabaseOperation()
{

}
//用于QTableView显示数据
QSqlTableModel *DatabaseOperation::tableShow(QString table, QString filter, QTableView *tableView, QSqlDatabase db){
    QSqlTableModel *model = new QSqlTableModel(tableView, db);
    model->setTable(table);
    if(filter!=""){
        model->setFilter(filter);
    }
    model->select();
    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "书名");
    model->setHeaderData(2, Qt::Horizontal, "作者");
    model->setHeaderData(3, Qt::Horizontal, "出版社");
    model->setHeaderData(4, Qt::Horizontal, "出版时间");
    model->setHeaderData(5, Qt::Horizontal, "价格");
    model->setHeaderData(6, Qt::Horizontal, "ISBN");
    model->setHeaderData(7, Qt::Horizontal, "类别");
    model->setHeaderData(9, Qt::Horizontal, "是否在馆");
    model->setHeaderData(10, Qt::Horizontal, "借出日期");
    model->setHeaderData(11, Qt::Horizontal, "归还日期");
    return model;
}

//注册从数据库种查找账户
bool DatabaseOperation::selectAccount(QString account){

    QSqlDatabase db;
    if(QSqlDatabase::contains("sqlite")){
        db = QSqlDatabase::database("sqlite");
    }
    else{
        db = QSqlDatabase::addDatabase("QSQLITE", "sqlite");
    }
    db.setDatabaseName("../LibraryManagementSystem/LibraySystemDatabase.db");
    if(!db.open()){
        qDebug()<<"Error: Failed to connect database."<<db.lastError();
    }
    else{
        QSqlQuery sql_query(db);
        sql_query.prepare("select * from accountInfo where account = ?");
        sql_query.addBindValue(account);
        sql_query.exec();
        sql_query.next();
        if(sql_query.isValid()){
                return true;
        }
    }
    db.close();
    return false;
}
//登录从数据库种查找账户，并匹配密码和权限
bool DatabaseOperation::checkAccount(QString account, QString password, QString permission){
    QSqlDatabase db;
    if(QSqlDatabase::contains("sqlite")){
        db = QSqlDatabase::database("sqlite");
    }
    else{
        db = QSqlDatabase::addDatabase("QSQLITE", "sqlite");
    }
    db.setDatabaseName("../LibraryManagementSystem/LibraySystemDatabase.db");
    if(!db.open()){
        qDebug()<<"Error: Failed to connect database."<<db.lastError();
    }
    else{
        QSqlQuery sql_query(db);
        sql_query.prepare("select * from accountInfo where account = ?");
        sql_query.addBindValue(account);
        sql_query.exec();
        sql_query.next();
        if(sql_query.isValid()){
            if(sql_query.value(0).toString()==account&&sql_query.value(1)==password&&sql_query.value(2)==permission){
                db.close();
                return true;
            }
        }
    }
    db.close();
    return false;
}
//添加用户或管理员
bool DatabaseOperation::addAccount(QString account, QString password, QString permission){
    QSqlDatabase db;
    if(QSqlDatabase::contains("sqlite")){
        db = QSqlDatabase::database("sqlite");
    }
    else{
        db = QSqlDatabase::addDatabase("QSQLITE", "sqlite");
    }
    db.setDatabaseName("../LibraryManagementSystem/LibraySystemDatabase.db");
    if(!db.open()){
        qDebug()<<"Error: Failed to connect database."<<db.lastError();
    }
    else{
        QSqlQuery sql_query(db);
        sql_query.prepare("insert into accountInfo values (?,?,?,?)");
        sql_query.addBindValue(account);
        sql_query.addBindValue(password);
        sql_query.addBindValue(permission);
        sql_query.addBindValue("");
        sql_query.exec();
        if(!sql_query.execBatch()){
            qDebug()<<sql_query.lastError();
        }
        else{
            qDebug()<<"添加账户成功";
            return true;
        }
        sql_query.finish();
    }
    db.close();
    return false;
}
//修改密码
bool DatabaseOperation::changePassword(QString account, QString password){
    QSqlDatabase db;
    if(QSqlDatabase::contains("sqlite")){
        db = QSqlDatabase::database("sqlite");
    }
    else{
        db = QSqlDatabase::addDatabase("QSQLITE", "sqlite");
    }
    db.setDatabaseName("../LibraryManagementSystem/LibraySystemDatabase.db");
    if(!db.open()){
        qDebug()<<"Error: Failed to connect database."<<db.lastError();
    }
    else{
        QSqlQuery sql_query(db);
        if(!sql_query.exec("update accountInfo set password = '"+password+"' where account = '"+account+"'")){
            qDebug()<<"修改密码失败："<<sql_query.lastError();
        }
        else{
            qDebug()<<"修改密码成功";
            return true;
        }
        sql_query.finish();
    }
    db.close();
    return false;
}

//借阅书籍
bool DatabaseOperation::borrowBook(QString account, int bookID, QString borrowDate, QString returnDate){
    QString bookName;
    QString isavailable;
    QSqlDatabase db;
    if(QSqlDatabase::contains("sqlite")){
        db = QSqlDatabase::database("sqlite");
    }
    else{
        db = QSqlDatabase::addDatabase("QSQLITE", "sqlite");
    }
    db.setDatabaseName("../LibraryManagementSystem/LibraySystemDatabase.db");
    if(!db.open()){
        qDebug()<<"Error: Failed to connect database."<<db.lastError();
    }
    else{
        QSqlQuery sql_query(db);
        //从图书信息中查询对应bookID
        sql_query.prepare("select * from bookInfo where id = ?");
        sql_query.addBindValue(bookID);
        if(sql_query.exec()){
            sql_query.next();
            bookName = sql_query.record().value(1).toString();
            isavailable = sql_query.record().value(9).toString();
            qDebug()<<"查询书籍信息成功";
        }
        else{
            qDebug()<<"Error: Failed to search bookID."<<sql_query.lastError();
        }
        if(isavailable.contains("Yes")){//判断图书是否已借出
            sql_query.prepare("insert into borrowInfo values (?,?,?,?,?)");
            sql_query.addBindValue(bookID);
            sql_query.addBindValue(bookName);
            sql_query.addBindValue(account);
            sql_query.addBindValue(borrowDate);
            sql_query.addBindValue(returnDate);
            if(!sql_query.exec()){
                qDebug()<<"Error: Failed to insert into borrowInfo."<<sql_query.lastError();
            }
            else{
                qDebug()<<"添加借阅记录成功";
            }
            //修改图书是否在馆、出借时间、归还时间信息
            sql_query.prepare("update bookInfo set isavailable = 'No',borrowDate = '"+borrowDate+"',returnDate = '"+returnDate+"' where id = ?");
            sql_query.addBindValue(bookID);
            if(!sql_query.exec()){
                qDebug()<<sql_query.lastError();
            }
            else{
                qDebug()<<"修改图书借阅信息成功";
                return true;
            }
            sql_query.finish();
        }
        else{
            qDebug()<<"借阅失败";
        }
    }
    db.close();
    return false;
}

//从借阅记录表中中查询
bool DatabaseOperation::selectBorrowInfo(int bookID, QString account, QString *bookName,QString *borrowDate, QString *returnDate){
    QSqlDatabase db;
    if(QSqlDatabase::contains("sqlite")){
        db = QSqlDatabase::database("sqlite");
    }
    else{
        db = QSqlDatabase::addDatabase("QSQLITE", "sqlite");
    }
    db.setDatabaseName("../LibraryManagementSystem/LibraySystemDatabase.db");
    if(!db.open()){
        qDebug()<<"Error: Failed to connect database."<<db.lastError();
    }
    else{
        QSqlQuery sql_query(db);
        //从借阅信息中查询ID
        sql_query.prepare("select * from borrowInfo where id = ? and account = '"+account+"'");
        sql_query.addBindValue(bookID);
        sql_query.exec();
        sql_query.next();
        if(sql_query.isValid()){
            *bookName = sql_query.record().value(1).toString();
            qDebug()<<sql_query.record().value(1).toString();
            *borrowDate = sql_query.record().value(3).toString();
            *returnDate = sql_query.record().value(4).toString();
            qDebug()<<"查询借阅记录成功";
            return true;
        }
        else{
            qDebug()<<"Error: Failed to search bookID."<<sql_query.lastError();
            return false;
        }
    }
    db.close();
    return false;
}
//还书
bool DatabaseOperation::returnBook(QString account, int bookID){
    QString accountSearchResult;
    bool isDeleteSucc = false;//删除借阅记录成功标志
    bool isUpdateSucc = false;//更新书籍信息成功标志
    QSqlDatabase db;
    if(QSqlDatabase::contains("sqlite")){
        db = QSqlDatabase::database("sqlite");
    }
    else{
        db = QSqlDatabase::addDatabase("QSQLITE", "sqlite");
    }
    db.setDatabaseName("../LibraryManagementSystem/LibraySystemDatabase.db");
    if(!db.open()){
        qDebug()<<"Error: Failed to connect database."<<db.lastError();
    }
    else{
        QSqlQuery sql_query(db);
        //从借阅记录中查询bookID
        sql_query.prepare("select * from borrowInfo where id = ?");
        sql_query.addBindValue(bookID);
        if(sql_query.exec()){
            sql_query.next();
            accountSearchResult = sql_query.record().value(2).toString();
            qDebug()<<"查询记录成功";
        }
        else{
            qDebug()<<"Error: Failed to search bookID."<<sql_query.lastError();
        }
        if(accountSearchResult==account){//匹配对应booID中的账户
            //删除借阅记录
            sql_query.prepare("delete from borrowInfo where id = ?");
            sql_query.addBindValue(bookID);
            if(!sql_query.exec()){
                qDebug()<<"Error: Failed to delete into borrowInfo."<<sql_query.lastError();
                isDeleteSucc = false;
            }
            else{
                qDebug()<<"删除借阅记录成功";
                isDeleteSucc = true;
            }
            //修改图书信息
            sql_query.prepare("update bookInfo set isavailable = 'Yes',borrowDate = '',returnDate = '' where id = ?");
            sql_query.addBindValue(bookID);
            if(!sql_query.exec()){
                qDebug()<<sql_query.lastError();
                isUpdateSucc = false;
            }
            else{
                qDebug()<<"修改书籍信息成功";
                isUpdateSucc = true;
            }
            //判断删除借阅记录和修改图书信息是否全部成功
            if(!isDeleteSucc||!isUpdateSucc){

                qDebug()<<"还书失败";
            }
            else{
                return true;
            }
        }
        sql_query.finish();
    }
    db.close();
    return false;
}
//添加图书
bool DatabaseOperation::addBook(QString bookName,
                                QString authorName,
                                QString pressName,
                                QString publishDate,
                                double price,
                                QString ISBN,
                                QString categeory,
                                QString outline,
                                int totalNumber){
    QSqlDatabase db;
    if(QSqlDatabase::contains("sqlite")){
        db = QSqlDatabase::database("sqlite");
    }
    else{
        db = QSqlDatabase::addDatabase("QSQLITE", "sqlite");
    }
    db.setDatabaseName("../LibraryManagementSystem/LibraySystemDatabase.db");
    if(!db.open()){
        qDebug()<<"Error: Failed to connect database."<<db.lastError();
    }
    else{
        QSqlQuery sql_query(db);
        sql_query.exec("select * from bookInfo");
        sql_query.last();
        int currentNumber = sql_query.record().value(0).toInt();
        sql_query.finish();
        QVariantList IDList;
        QVariantList bookNameList;
        QVariantList authorNameList;
        QVariantList pressNameList;
        QVariantList publishDateList;
        QVariantList priceList;
        QVariantList ISBNList;
        QVariantList categeoryList;
        QVariantList outlineList;
        QVariantList isavailableList;
        QVariantList borrowDateList;
        QVariantList returnDateList;
        sql_query.prepare("insert into bookInfo values(?,?,?,?,?,?,?,?,?,?,?,?)");
        for(int i=0;i<totalNumber;i++){
            IDList.append(currentNumber+i+1);
            bookNameList.append(bookName);
            authorNameList.append(authorName);
            pressNameList.append(pressName);
            publishDateList.append(publishDate);
            priceList.append(price);
            ISBNList.append(ISBN);
            categeoryList.append(categeory);
            outlineList.append(outline);
            isavailableList.append("Yes");
            borrowDateList.append("");
            returnDateList.append("");
        }
        sql_query.addBindValue(IDList);
        sql_query.addBindValue(bookNameList);
        sql_query.addBindValue(authorNameList);
        sql_query.addBindValue(pressNameList);
        sql_query.addBindValue(publishDateList);
        sql_query.addBindValue(priceList);
        sql_query.addBindValue(ISBNList);
        sql_query.addBindValue(categeoryList);
        sql_query.addBindValue(outlineList);
        sql_query.addBindValue(isavailableList);
        sql_query.addBindValue(borrowDateList);
        sql_query.addBindValue(returnDateList);
        if(!sql_query.execBatch()){
            qDebug()<<sql_query.lastError();
        }
        else{
            qDebug()<<"插入记录成功";
            return true;
        }
        sql_query.finish();
    }
    db.close();
    return false;
}
