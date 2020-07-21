#include "addbookwidget.h"
#include "databaseoperation.h"
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
AddBookWidget::AddBookWidget(QWidget *parent) : QWidget(parent)
{
    //部件
    QLabel *titleLabel = new QLabel;
    titleLabel->setText("添加图书");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setMinimumSize(300, 50);
    QLabel *bookNameLabel = new QLabel("图书名称：");
    bookNameLine = new QLineEdit();
    QLabel *authorNameLabel = new QLabel("作者：");
    authorNameLine = new QLineEdit();
    QLabel *pressNameLabel = new QLabel("出版社：");
    pressNameLine = new QLineEdit();
    QLabel *ISBNLabel = new QLabel("ISBN：");
    ISBNLine = new QLineEdit();
    QLabel *publishDateLabel = new QLabel("出版日期：");
    publishDateLine = new QLineEdit();
    QLabel *categoryLabel = new QLabel("类别：");
    categoryLine = new QLineEdit();
    QLabel *priceLabel = new QLabel("价格：");
    priceLine = new QLineEdit();
    QValidator *priceValidator=new QDoubleValidator;
    priceLine->setValidator(priceValidator);
    priceLine->setPlaceholderText("限制为数字");
    QLabel *totalNumberLabel = new QLabel("数量");
    totalNumberLine = new QLineEdit;
    QValidator *numberValidator=new QIntValidator(1,1000,this);
    totalNumberLine->setValidator(numberValidator);
    totalNumberLine->setPlaceholderText("限制为[1,1000)之间的整数");
    QLabel *outlineLabel = new QLabel("概要：");
    outlineText = new QTextEdit();

    QPushButton *addBookButton = new QPushButton("添加");

    //布局
    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->addWidget(titleLabel, 0,0,1,8,Qt::AlignCenter);
    gridLayout->addWidget(bookNameLabel,1,0);
    gridLayout->addWidget(bookNameLine,1,2);
    gridLayout->addWidget(authorNameLabel,1,4);
    gridLayout->addWidget(authorNameLine,1,6);
    gridLayout->addWidget(pressNameLabel,2,0);
    gridLayout->addWidget(pressNameLine,2,2);
    gridLayout->addWidget(ISBNLabel,2,4);
    gridLayout->addWidget(ISBNLine,2,6);
    gridLayout->addWidget(publishDateLabel,3,0);
    gridLayout->addWidget(publishDateLine,3,2);
    gridLayout->addWidget(priceLabel,3,4);
    gridLayout->addWidget(priceLine,3,6);
    gridLayout->addWidget(categoryLabel,4,0);
    gridLayout->addWidget(categoryLine,4,2);
    gridLayout->addWidget(totalNumberLabel,5,0);
    gridLayout->addWidget(totalNumberLine,5,2);
    gridLayout->addWidget(outlineLabel,6,0,1,2);
    gridLayout->addWidget(outlineText,7,0,1,8);
    gridLayout->addWidget(addBookButton, 8, 2, 1, 4);
    setLayout(gridLayout);

    setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(this->backgroundRole(), QBrush(QPixmap(":/粉碎.png")));
    this->setPalette(palette);
    connect(addBookButton, &QPushButton::clicked, this, &AddBookWidget::addBookButtonClick);
}
//添加图书
void AddBookWidget::addBookButtonClick(){
    if(totalNumberLine->text().toInt()==0||bookNameLine->text()==""){
        QMessageBox::information(this, tr("添加失败"), tr("添加图书为空。"));
    }
    else if(DatabaseOperation::addBook(bookNameLine->text(), authorNameLine->text(), pressNameLine->text(), publishDateLine->text(), priceLine->text().toDouble(), ISBNLine->text(), categoryLine->text(), outlineText->toPlainText(), totalNumberLine->text().toInt())){
        bookNameLine->clear();
        authorNameLine->clear();
        pressNameLine->clear();
        ISBNLine->clear();
        publishDateLine->clear();
        categoryLine->clear();
        priceLine->clear();
        totalNumberLine->clear();
        outlineText->clear();
        emit finishAddBook();
        QMessageBox::information(this, tr("添加成功"), tr("已成功添加图书。"));

    }
    else{
        QMessageBox::information(this, tr("添加失败"), tr("请重新添加。"));
    }

}
