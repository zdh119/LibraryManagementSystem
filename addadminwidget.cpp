#include "addadminwidget.h"
#include "databaseoperation.h"
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
AddAdminWidget::AddAdminWidget(QWidget *parent) : QWidget(parent)
{
    //部件
    QLabel *accountLabel = new QLabel("账户：");
    accountLabel->setAlignment(Qt::AlignCenter);
    QLabel *passwordLabel = new QLabel("密码：");
    passwordLabel->setAlignment(Qt::AlignCenter);
    QLabel *confireLabel = new QLabel("确认密码：");
    confireLabel->setAlignment(Qt::AlignCenter);
    accountLineEdit = new QLineEdit;
    accountLineEdit->setPlaceholderText("请输入账户名");
    passwordLineEdit = new QLineEdit;
    passwordLineEdit->setPlaceholderText("请输入密码");
    confireLineEdit = new QLineEdit;
    confireLineEdit->setPlaceholderText("请确认密码");
    QPushButton *addAdminButton = new QPushButton("添加");
    accountLineEdit->setEchoMode(QLineEdit::Normal);
    passwordLineEdit->setEchoMode(QLineEdit::PasswordEchoOnEdit);
    confireLineEdit->setEchoMode(QLineEdit::PasswordEchoOnEdit);

    //布局
    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(accountLabel, 1, 0);
    layout->addWidget(accountLineEdit, 1, 1, 1, 2);
    layout->addWidget(passwordLabel, 2, 0);
    layout->addWidget(passwordLineEdit, 2, 1, 1, 2);
    layout->addWidget(confireLabel, 3, 0);
    layout->addWidget(confireLineEdit, 3, 1, 1, 2);
    layout->addWidget(addAdminButton, 4, 1);
    setWindowTitle("添加管理员");
    setWindowIcon(QIcon(":/add.ico"));


    connect(addAdminButton, &QPushButton::clicked, this, &AddAdminWidget::addAdminButtonClick);
}
//添加管理员
void AddAdminWidget::addAdminButtonClick(){
    QString account = accountLineEdit->text();
    QString password = passwordLineEdit->text();
    QString confirePassword = confireLineEdit->text();
    if(account==""||password==""){
        QMessageBox::information(this, tr("添加失败"), tr("管理员账号或密码为空。"));
        accountLineEdit->clear();
        passwordLineEdit->clear();
        confireLineEdit->clear();
    }
    else if(account.contains(" ")){
        QMessageBox::information(this, tr("添加失败"), tr("账户中不能有空格，请重新输入账户。"));
        accountLineEdit->clear();
    }
    else if(password!=confirePassword){
        QMessageBox::information(this, tr("添加失败"), tr("两次输入密码不同，请重新输入密码。"));
        passwordLineEdit->clear();
        confireLineEdit->clear();
    }
    else{
        if(DatabaseOperation::selectAccount(account)){
            QMessageBox::information(this, tr("添加失败"), tr("\"%1\" 账户已存在，请重新输入账户。").arg(accountLineEdit->text()));
            accountLineEdit->clear();
        }
        else{
            DatabaseOperation::addAccount(account, password, "admin");
            QMessageBox::information(this, tr("添加成功"), tr("您已成功添加管理员%1。").arg(accountLineEdit->text()));
            this->close();
        }
    }
}
