#include "mywidget.h"
#include "databaseoperation.h"
#include "addadminwidget.h"
#include <QHBoxLayout>
#include <QMessageBox>
MyWidget::MyWidget(Login::Model Model, QString account, QString password, QWidget *parent) : QWidget(parent)
{

    this->account = account;
    this->password = password;
    accountLabel = new QLabel("账户：");
    accountLabel->setAlignment(Qt::AlignCenter);
    passwordLabel = new QLabel("密码：");
    passwordLabel->setAlignment(Qt::AlignCenter);
    changePasswordLabel = new QLabel("密码修改为：");
    changePasswordLabel->setAlignment(Qt::AlignCenter);
    changePasswordLabel->hide();
    //账户显示框不可编辑
    accountLineEdit = new QLineEdit(account);
    accountLineEdit->setReadOnly(true);
    //当前密码显示框
    passwordLineEdit = new QLineEdit;
    passwordLineEdit->setPlaceholderText("请输入密码");
    //修改密码框默认情况下隐藏
    changePasswordLineEdit = new QLineEdit;
    changePasswordLineEdit->hide();
    //修改密码按钮
    changePasswordButton = new QPushButton("修改密码");
    submitButton = new QPushButton("提交");
    submitButton->hide();
    addAdminAccountButton = new QPushButton("添加管理员账户");
    //用户界面无添加管理员账户按钮
    if(Model==Login::User){
        addAdminAccountButton->hide();
    }

    //布局
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(accountLabel, 1, 0);
    layout->addWidget(accountLineEdit, 1, 1, 1, 2);
    layout->addWidget(passwordLabel, 2, 0);
    layout->addWidget(passwordLineEdit, 2, 1, 1, 2);
    layout->addWidget(changePasswordButton, 3, 1);
    layout->addWidget(changePasswordLabel, 4, 0);
    layout->addWidget(changePasswordLineEdit, 4, 1, 1, 2);
    layout->addWidget(submitButton, 5, 1);
    layout->addWidget(addAdminAccountButton, 6, 1);
    setLayout(layout);

    setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(this->backgroundRole(), QBrush(QPixmap(":/布.花.png")));
    this->setPalette(palette);


    connect(changePasswordButton, &QPushButton::clicked, this, &MyWidget::changePasswordButtonClick);
    connect(submitButton, &QPushButton::clicked, this, &MyWidget::submitButtonClick);
    connect(addAdminAccountButton, &QPushButton::clicked, this, &MyWidget::addAdminAccountButtonClick);
}
//修改密码按钮槽函数，如果当前密码正确，则实现——当点击修改密码按钮时，显示修改密码框和提交按钮
void MyWidget::changePasswordButtonClick(){
    if(passwordLineEdit->text()==password){
        changePasswordLabel->setVisible(true);
        changePasswordLineEdit->setVisible(true);
        submitButton->setVisible(true);
    }
    else{
        QMessageBox::information(this, tr("修改失败"), tr("输入的当前密码不正确。"));
    }
}
//提交修改密码
void MyWidget::submitButtonClick(){
    if(!DatabaseOperation::changePassword(accountLineEdit->text(), changePasswordLineEdit->text())){
        QMessageBox::information(this, tr("修改失败"), tr("修改密码失败。"));
    }
    else{
        QMessageBox::information(this, tr("修改成功"), tr("修改密码成功。"));
        password = changePasswordLineEdit->text();
        passwordLineEdit->clear();
        changePasswordLineEdit->clear();
        changePasswordLabel->setVisible(false);
        changePasswordLineEdit->setVisible(false);
        submitButton->setVisible(false);
    }
}
//添加管理员账户
void MyWidget::addAdminAccountButtonClick(){
    AddAdminWidget *addAdminWidget = new AddAdminWidget;
    addAdminWidget->show();
}
