#include "bookinfowidget.h"
#include "borrowwidget.h"
#include "databaseoperation.h"
#include <QGridLayout>
#include <QSqlTableModel>
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QMessageBox>
#include <QHeaderView>
BookInfoWidget::BookInfoWidget(Login::Model Model,QWidget *parent) : QWidget(parent)
{

    //删除图书窗口
    deleteBookWidget = new DeleteBookWidget;
    //部件
    QLabel *titleLabel = new QLabel;
    titleLabel->setText("图书信息");
    titleLabel->setAlignment(Qt::AlignCenter);

    QLabel *bookNameLabel = new QLabel("图书名称：");
    bookNameLine = new QLineEdit();
    bookNameLine->setReadOnly(true);//书名只读
    QLabel *authorNameLabel = new QLabel("作者：");
    authorNameLine = new QLineEdit();
    QLabel *presskNameLabel = new QLabel("出版社：");
    presskNameLine = new QLineEdit();
    QLabel *ISBNLabel = new QLabel("ISBN：");
    ISBNLine = new QLineEdit();
    QLabel *publishDateLabel = new QLabel("出版日期：");
    publishDateLine = new QLineEdit();
    QLabel *categoryLabel = new QLabel("类别：");
    categoryLine = new QLineEdit();
    QLabel *priceLabel = new QLabel("价格：");
    priceLine = new QLineEdit();
    QLabel *outlineLabel = new QLabel("概要：");
    outlineText = new QTextEdit();
    QLabel *availableNumberLabel = new QLabel("在馆数：");
    availableNumberLine = new QLineEdit();
    QLabel *totalNumberLabel = new QLabel("总数");
    totalNumberLine = new QLineEdit;
    //在馆数和总数不可修改，只有当主动删除或添加书籍的时候才能改变
    availableNumberLine->setReadOnly(true);
    totalNumberLine->setReadOnly(true);

    bookInfoTableView = new QTableView(this);
    bookInfoTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    QPushButton *borrowBookButton = new QPushButton("借阅");
    QPushButton *deleteBookButton = new QPushButton("删除");
    QPushButton *modifyBookButton = new QPushButton("修改");

    //布局
    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->addWidget(titleLabel, 0,0,1,8,Qt::AlignCenter);
    gridLayout->addWidget(bookNameLabel,1,0);
    gridLayout->addWidget(bookNameLine,1,2);
    gridLayout->addWidget(authorNameLabel,1,4);
    gridLayout->addWidget(authorNameLine,1,6);
    gridLayout->addWidget(presskNameLabel,2,0);
    gridLayout->addWidget(presskNameLine,2,2);
    gridLayout->addWidget(ISBNLabel,2,4);
    gridLayout->addWidget(ISBNLine,2,6);
    gridLayout->addWidget(publishDateLabel,3,0);
    gridLayout->addWidget(publishDateLine,3,2);
    gridLayout->addWidget(priceLabel,3,4);
    gridLayout->addWidget(priceLine,3,6);
    gridLayout->addWidget(categoryLabel,4,0);
    gridLayout->addWidget(categoryLine,4,2);
    gridLayout->addWidget(outlineLabel,5,0,1,2);
    gridLayout->addWidget(outlineText,6,0,1,8);
    gridLayout->addWidget(availableNumberLabel,7,0);
    gridLayout->addWidget(availableNumberLine,7,2);
    gridLayout->addWidget(totalNumberLabel,7,4);
    gridLayout->addWidget(totalNumberLine,7,6);
    gridLayout->addWidget(bookInfoTableView,8,0,1,8);
    //根据账户模式选择显示内容
    if(Model==Login::User){
        //用户模式下显示借阅按钮
        gridLayout->addWidget(borrowBookButton, 9, 0, 1, 8,Qt::AlignCenter);
        //用户模式下，只读
        authorNameLine->setReadOnly(true);
        presskNameLine->setReadOnly(true);
        ISBNLine->setReadOnly(true);
        publishDateLine->setReadOnly(true);
        priceLine->setReadOnly(true);
        categoryLine->setReadOnly(true);
        outlineText->setReadOnly(true);
    }
    else{
        //管理员模式下，显示修改、删除按钮
        gridLayout->addWidget(modifyBookButton, 9, 0, 1, 4);
        gridLayout->addWidget(deleteBookButton, 9, 4, 1, 4);
    }

    setLayout(gridLayout);

    setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(this->backgroundRole(), QBrush(QPixmap(":/细方格平铺素材.png")));
    this->setPalette(palette);

    connect(borrowBookButton, &QPushButton::clicked, this, &BookInfoWidget::borrowBookButtonClick);
    connect(modifyBookButton, &QPushButton::clicked, this, &BookInfoWidget::modifyBookButtonClick);
    connect(deleteBookButton, &QPushButton::clicked, this, &BookInfoWidget::deleteBookButtonClick);
    connect(bookInfoTableView, &QTableView::doubleClicked, this, &BookInfoWidget::bookInfoTableViewClick);
    //设置tab顺序
    this->setTabOrder(borrowBookButton,borrowBookButton);
}
void BookInfoWidget::keyPressEvent(QKeyEvent *event){
    if(event->key()==Qt::Key_Return) {
        borrowBookButtonClick();
    }
}

//显示图书详细信息
void BookInfoWidget::setBookInfo(QString bookName){
    //查询数据库该图书的信息，并显示在bookInfoView中
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
        QSqlTableModel *model = DatabaseOperation::tableShow("bookInfo", "bookName = '"+bookName+"'",bookInfoTableView, db);
        bookInfoTableView->setModel(model);
        bookNameLine->setText(model->record(0).value(1).toString());
        authorNameLine->setText(model->record(0).value(2).toString());
        presskNameLine->setText(model->record(0).value(3).toString());
        publishDateLine->setText(model->record(0).value(4).toString());
        priceLine->setText(model->record(0).value(5).toString());
        ISBNLine->setText(model->record(0).value(6).toString());
        categoryLine->setText(model->record(0).value(7).toString());
        outlineText->setText(model->record(0).value(8).toString());
        //总数
        totalNumberLine->setText(QString::number(model->rowCount()));
        //在馆数
        int availableNumber=0;
        for(int i=0;i<model->rowCount();i++){
            if(model->record(i).value(9).toString()=="Yes")
                availableNumber++;
        }
        availableNumberLine->setText(QString::number(availableNumber));

        //隐藏作者，出版社，出版日期，价格，ISBN，类别，概要
        bookInfoTableView->hideColumn(2);
        bookInfoTableView->hideColumn(3);
        bookInfoTableView->hideColumn(4);
        bookInfoTableView->hideColumn(5);
        bookInfoTableView->hideColumn(6);
        bookInfoTableView->hideColumn(7);
        bookInfoTableView->hideColumn(8);
        bookInfoTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        bookInfoTableView->setModel(model);
    }
    db.close();
}
//发送借书信号给用户窗口
void BookInfoWidget::borrowBookButtonClick(){
    emit borrowBookButtonClickSignal();
}
//删除图书
void BookInfoWidget::deleteBookButtonClick(){
    emit deleteBookButtonClickSignal();
}
//修改图书信息
void BookInfoWidget::modifyBookButtonClick(){
    if(bookNameLine->isModified()||authorNameLine->isModified()||presskNameLine->isModified()||ISBNLine->isModified()||publishDateLine->isModified()||categoryLine->isModified()||priceLine->isModified()||outlineText->overwriteMode()){
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
            sql_query.prepare("update bookInfo set authorName = ?,presskName = ?, ISBN = ?, publishDate = ?, category = ?, price = ?, outline = ? where bookName = ?");
            sql_query.addBindValue(authorNameLine->text());
            sql_query.addBindValue(presskNameLine->text());
            sql_query.addBindValue(ISBNLine->text());
            sql_query.addBindValue(publishDateLine->text());
            sql_query.addBindValue(categoryLine->text());
            sql_query.addBindValue(priceLine->text().toDouble());
            sql_query.addBindValue(outlineText->toPlainText());
            sql_query.addBindValue(bookNameLine->text());
            if(sql_query.exec()){
                QMessageBox::information(this, tr("修改成功"), tr("你已成功修改所有《%1》的信息").arg(bookNameLine->text()));
            }
            else{
                QMessageBox::information(this, tr("修改失败"), tr("你未能修改《%1》的信息").arg(bookNameLine->text()));
            }
        }
        db.close();
    }
    else{
        QMessageBox::information(this, tr("修改失败"),tr("您未做任何修改"));
    }
}

void BookInfoWidget::bookInfoTableViewClick(QModelIndex index){
    checkedIndex = index;
    emit tableClick();
}
