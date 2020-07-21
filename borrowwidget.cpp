#include "borrowwidget.h"
#include "databaseoperation.h"
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
BorrowWidget::BorrowWidget(QString account, QWidget *parent) : QWidget(parent)
{
    //部件
    QLabel *accountLabel = new QLabel("账户：");
    accountLabel->setAlignment(Qt::AlignCenter);
    accoutLine = new QLineEdit;
    accoutLine->setText(account);
    accoutLine->setReadOnly(true);

    QLabel *bookIDLabel = new QLabel("书的ID：");
    bookIDLabel->setAlignment(Qt::AlignCenter);
    bookIDLine = new QLineEdit;
    bookIDLine->setPlaceholderText("请输入ID");

    QLabel *borrowDateLabel = new QLabel("出借时间：");
    borrowDateLabel->setAlignment(Qt::AlignCenter);
    borrowDateLine = new QLineEdit;
    borrowDateLine->setText(QDate::currentDate().toString("yyyy-MM-dd"));
    borrowDateLine->setReadOnly(true);

    QLabel *returnDateLabel = new QLabel("归还日期：");
    returnDateLabel->setAlignment(Qt::AlignCenter);
    returnDateEdit = new QDateEdit;
    returnDateEdit->setDisplayFormat("yyyy-MM-dd");
    returnDateEdit->setMinimumDate(QDate::currentDate());
    returnDateEdit->setMaximumDate(QDate::currentDate().addDays(90));  // 归还日期不得超过3个月
    returnDateEdit->setCalendarPopup(true);

    QPushButton *confirmButton = new QPushButton("确定");
    //布局
    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->addWidget(accountLabel, 2, 0);
    gridLayout->addWidget(accoutLine, 2, 1, 1, 3);
    gridLayout->addWidget(bookIDLabel, 3, 0);
    gridLayout->addWidget(bookIDLine, 3, 1, 1, 3);
    gridLayout->addWidget(borrowDateLabel, 4, 0);
    gridLayout->addWidget(borrowDateLine, 4, 1, 1, 3);
    gridLayout->addWidget(returnDateLabel, 5, 0);
    gridLayout->addWidget(returnDateEdit, 5, 1, 1, 3);
    gridLayout->addWidget(confirmButton,6, 1, 1, 2);
    setLayout(gridLayout);

    setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(this->backgroundRole(), QBrush(QPixmap(":/交响曲.png")));
    this->setPalette(palette);

    connect(confirmButton, &QPushButton::clicked, this, &BorrowWidget::confirmButtonClick);

}
//借书确认
void BorrowWidget::confirmButtonClick(){
    if(bookIDLine->text()==""){
        QMessageBox::information(this,  tr("借阅失败"), tr("借阅图书ID为空。"));
    }
    else{
        if(DatabaseOperation::borrowBook(accoutLine->text(), bookIDLine->text().toInt(), borrowDateLine->text(), returnDateEdit->text())){
            bookIDLine->clear();
            QMessageBox::information(this, tr("借阅成功"), tr("您已成功借阅。"));
        }
        else{
            QMessageBox::information(this, tr("借阅失败"), tr("对不起，借阅失败，请重新借阅。"));
        }
    }

}
void BorrowWidget::setBorrowBookID(QString bookID){
    bookIDLine->setText(bookID);
}
