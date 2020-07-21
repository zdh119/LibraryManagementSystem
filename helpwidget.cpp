#include "helpwidget.h"

#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QTextEdit>

HelpWidget::HelpWidget(QWidget *parent) : QWidget(parent)
{
    resize(600,400);
    QLabel *titleLabel = new QLabel("说明");
    QTextEdit *helpText = new QTextEdit;
    QString paragraph1 = "1.本软件实现了图书管理系统的基本功能——检索图书、查看图书信息、借阅书籍、归还书籍、查询借阅记录，修改个人信息功能；以及管理员特有的添加图书、删除图书功能。";
    QString paragraph2 = "2.搜索提供了对书名、ID、类别的精准搜索和模糊搜索；排序提供了对ID、图书价格的排序功能。当双击首页的图书名时，会跳转到图书详细信息界面（默认情况下，图书详细信息界面为空）。";
    QString paragraph3 = "3.不支持修改账户名（账户名是每个账户的唯一标识）。";
    QString paragraph4 = "4.在首页中双击书名，会自动跳转到所点击的图书详细信息界面。";
    QString paragraph5 = "5.在图书详细信息界面，双击表中图书的ID，会自动跳转到借阅界面。";
    QString paragraph = paragraph1 + '\n' + paragraph2 +'\n' + paragraph3 + '\n' + paragraph4 + '\n'+ paragraph5 + '\n';
    helpText->setText(paragraph);
    helpText->setReadOnly(true);
    QVBoxLayout *VBLayout = new QVBoxLayout;
    VBLayout->addWidget(titleLabel);
    VBLayout->addWidget(helpText);
    setLayout(VBLayout);
    setWindowTitle("帮助");
    setWindowIcon(QIcon(":/help.ico"));
}
