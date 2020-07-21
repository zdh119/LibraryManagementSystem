#include "deletebookwidget.h"

#include <QLabel>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>
#include <QMessageBox>
#include <QGridLayout>
DeleteBookWidget::DeleteBookWidget(QWidget *parent) : QWidget(parent)
{
    //部件
    QLabel *bookIDLabel = new QLabel("ID：");
    bookIDLabel->setAlignment(Qt::AlignCenter);
    bookIDLine = new QLineEdit;
    QLabel *bookNameLabel = new QLabel("书名：");
    bookNameLabel->setAlignment(Qt::AlignCenter);
    bookNameLine = new QLineEdit;

    QPushButton *searchBookButton = new QPushButton("查询");
    deleteBookButton = new QPushButton("删除");
    deleteBookButton->hide();
    //布局
    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->addWidget(bookIDLabel, 1, 0);
    gridLayout->addWidget(bookIDLine, 1, 1, 1, 2);
    gridLayout->addWidget(bookNameLabel, 2, 0);
    gridLayout->addWidget(bookNameLine, 2, 1, 1, 2);
    gridLayout->addWidget(searchBookButton, 3, 1);
    gridLayout->addWidget(deleteBookButton, 4, 1);

    setLayout(gridLayout);

    setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(this->backgroundRole(), QBrush(QPixmap(":/粉碎.png")));
    this->setPalette(palette);

    connect(searchBookButton, &QPushButton::clicked, this, &DeleteBookWidget::searchBook);
    connect(deleteBookButton, &QPushButton::clicked, this, &DeleteBookWidget::deleteBook);
}
//查询要删除的书籍
void DeleteBookWidget::searchBook(){
    if(bookIDLine->text()==""||bookNameLine->text()==""){
        QMessageBox::information(this, tr("查询失败"), tr("ID或书名为空"));
        deleteBookButton->hide();
    }
    else{
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
            sql_query.prepare("select * from bookInfo where id = ? and bookName = '"+bookNameLine->text()+"'");
            sql_query.addBindValue(bookIDLine->text().toInt());
            sql_query.exec();
            if(!sql_query.next()){
                QMessageBox::information(this, tr("查询失败"), tr("没有《%1》图书").arg(bookNameLine->text()));
                bookIDLine->clear();
                bookNameLine->clear();
            }
            else {
                sql_query.prepare("select * from bookInfo where id = ? and bookName = '"+bookNameLine->text()+"' and isavailable = 'Yes'");
                sql_query.addBindValue(bookIDLine->text().toInt());
                sql_query.exec();
                if(!sql_query.next()){
                    QMessageBox::information(this, tr("查询成功"), tr("《%1》图书已借出，不可删除").arg(bookNameLine->text()));
                    bookIDLine->clear();
                    bookNameLine->clear();
                }
                else{
                    deleteBookButton->setVisible(true);
                }
            }
        }
    }
}
//删除书籍
void DeleteBookWidget::deleteBook(){
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
        sql_query.prepare("delete from bookInfo where cast(id as varchar)= '"+bookIDLine->text()+"'");
        if(!sql_query.exec()){
            QMessageBox::information(this, tr("删除失败"), tr("《%1》图书未成功删除").arg(bookNameLine->text()));
        }
        else {
            emit finishDeleteBook();
            QMessageBox::information(this, tr("删除成功"), tr("《%1》图书已成功删除").arg(bookNameLine->text()));
            bookIDLine->clear();
            bookNameLine->clear();
            deleteBookButton->hide();
        }
    }
    db.close();
}
