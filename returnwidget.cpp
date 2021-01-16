#include "returnwidget.h"
#include "databaseoperation.h"

#include <QGridLayout>
#include <QMessageBox>
#include <QDebug>
ReturnWidget::ReturnWidget(QString account, QWidget *parent) : QWidget(parent)
{

    //部件
    QLabel *accountLabel = new QLabel("账户：");
    accountLabel->setAlignment(Qt::AlignCenter);
    accountLine = new QLineEdit;
    accountLine->setText(account);
    accountLine->setReadOnly(true);

    QLabel *bookIDLabel = new QLabel("书的ID：");
    bookIDLabel->setAlignment(Qt::AlignCenter);
    bookIDLine = new QLineEdit;
    bookIDLine->setPlaceholderText("请输入归还书籍的ID");
    QPushButton *returnButton = new QPushButton("归还");
    bookNameLabel = new QLabel("书名：");
    bookNameLabel->setAlignment(Qt::AlignCenter);
    bookNameLabel->hide();
    bookNameLine = new QLineEdit;
    bookNameLine->hide();

    borrowDateLabel = new QLabel("出借时间：");
    borrowDateLabel->setAlignment(Qt::AlignCenter);
    borrowDateLabel->hide();
    borrowDateLine = new QLineEdit;
    borrowDateLine->hide();

    returnDateLabel = new QLabel("归还日期：");
    returnDateLabel->setAlignment(Qt::AlignCenter);
    returnDateLabel->hide();
    returnDateLine = new QLineEdit;
    returnDateLine->hide();

    confirmButton = new QPushButton("确认");
    confirmButton->hide();
    //布局
    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->addWidget(accountLabel, 2, 0);
    gridLayout->addWidget(accountLine, 2, 1, 1, 3);
    gridLayout->addWidget(bookIDLabel, 3, 0);
    gridLayout->addWidget(bookIDLine, 3, 1, 1, 3);
    gridLayout->addWidget(returnButton, 4, 1, 1, 2);
    gridLayout->addWidget(bookNameLabel, 5, 0);
    gridLayout->addWidget(bookNameLine, 5, 1, 1, 3);
    gridLayout->addWidget(borrowDateLabel, 6, 0);
    gridLayout->addWidget(borrowDateLine, 6, 1, 1, 3);
    gridLayout->addWidget(returnDateLabel, 7, 0);
    gridLayout->addWidget(returnDateLine, 7, 1, 1, 3);
    gridLayout->addWidget(confirmButton, 8, 1, 1, 2);
    this->setLayout(gridLayout);

    setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(this->backgroundRole(), QBrush(QPixmap(":/花纹古典底纹素材.png")));
    this->setPalette(palette);

    connect(returnButton, &QPushButton::clicked, this, &ReturnWidget::returnButtonClick);
    connect(confirmButton, &QPushButton::clicked, this, &ReturnWidget::confirmButtonClick);

    //设置tab顺序
    this->setTabOrder(this->bookIDLine, this->returnButton);
    this->setTabOrder(this->returnButton, this->confirmButton);
    this->setTabOrder(this->confirmButton, this->bookIDLine);
}
void ReturnWidget::keyPressEvent(QKeyEvent *event){
    if(event->key()==Qt::Key_Return) {
        if(this->returnButton->hasFocus()){
            returnButtonClick();
        } else if(this->confirmButton->hasFocus()){
            confirmButtonClick();
        }
    }
}

//还书按钮槽函数
void ReturnWidget::returnButtonClick(){
    if(bookIDLine->text()==""){
        QMessageBox::information(this,  tr("还书失败"), tr("归还书籍的ID为空。"));
    }
    else{
        QString *bookName = new QString;
        QString *returnDate= new QString;
        QString *borrowDate= new QString;
        if(DatabaseOperation::selectBorrowInfo(bookIDLine->text().toInt(), accountLine->text(), bookName, borrowDate, returnDate)){
            bookNameLabel->setVisible(true);
            bookNameLine->setVisible(true);
            bookNameLine->setText(*bookName);
            bookNameLine->setReadOnly(true);
            borrowDateLabel->setVisible(true);
            borrowDateLine->setVisible(true);
            borrowDateLine->setText(*borrowDate);
            borrowDateLine->setReadOnly(true);
            returnDateLabel->setVisible(true);
            returnDateLine->setVisible(true);
            returnDateLine->setText(*returnDate);
            returnDateLine->setReadOnly(true);
            confirmButton->setVisible(true);
        }
        else{
            QMessageBox::information(this, tr("还书失败"), tr("您没有借阅ID为%1的书。").arg(bookIDLine->text()));
        }
    }
}
//确认还书按钮槽函数
void ReturnWidget::confirmButtonClick(){
    if(DatabaseOperation::returnBook(accountLine->text(), bookIDLine->text().toInt())){
        QMessageBox::information(this, tr("还书成功"), tr("您成功已归还。"));
        bookNameLabel->hide();
        bookIDLine->clear();
        bookNameLine->hide();
        borrowDateLabel->hide();
        borrowDateLine->hide();
        returnDateLabel->hide();
        returnDateLine->hide();
        confirmButton->hide();
    }
    else{
        QMessageBox::information(this, tr("还书失败"), tr("请重新还书。"));
    }
}
