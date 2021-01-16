#include "register.h"
#include "databaseoperation.h"
#include <QGridLayout>
#include <QMessageBox>
Register::Register(QWidget *parent) : QWidget(parent)
{
    resize(600,400);
    //部件
    QLabel *accountLabel = new QLabel("账号:");
    accountLabel->setAlignment(Qt::AlignCenter);
    QLabel *passwordLabel = new QLabel("密码:");
    passwordLabel->setAlignment(Qt::AlignCenter);
    QLabel *confireLabel = new QLabel("确认密码:");
    confireLabel->setAlignment(Qt::AlignCenter);
    registerButton = new QPushButton("注册");
    backToLoginButton = new QPushButton("返回");
    //设置快捷键
    backToLoginButton->setShortcut(tr("ctrl+left"));

    accountLineEdit = new QLineEdit;
    accountLineEdit->setPlaceholderText("请输入账户，不能有空格");
    accountLineEdit->setMinimumSize(400,50);

    passwordLineEdit = new QLineEdit;
    passwordLineEdit->setPlaceholderText("请输入密码");
    passwordLineEdit->setMinimumSize(400,50);

    confireLineEdit = new QLineEdit;
    confireLineEdit->setPlaceholderText("请确认密码");
    confireLineEdit->setMinimumSize(400,50);

    accountLineEdit->setEchoMode(QLineEdit::Normal);
    passwordLineEdit->setEchoMode(QLineEdit::PasswordEchoOnEdit);
    confireLineEdit->setEchoMode(QLineEdit::PasswordEchoOnEdit);
    //布局
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(accountLabel, 1, 0);
    layout->addWidget(accountLineEdit, 1, 1);
    layout->addWidget(passwordLabel, 2, 0);
    layout->addWidget(passwordLineEdit, 2, 1);
    layout->addWidget(confireLabel, 3, 0);
    layout->addWidget(confireLineEdit, 3, 1);
    layout->addWidget(backToLoginButton, 4, 0);
    layout->addWidget(registerButton, 4, 1);
    setLayout(layout);

    setWindowTitle("图书管理系统注册");
    setWindowIcon(QIcon(":/register.ico"));

    setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(this->backgroundRole(), QBrush(QPixmap(":/绿色圆环.png")));
    this->setPalette(palette);

    connect(registerButton, &QPushButton::clicked, this, &Register::registerButtonClick);
    connect(backToLoginButton, &QPushButton::clicked, this, &Register::backToLoginButtonClick);
}
void Register::keyPressEvent(QKeyEvent *event){
    if(event->key()== Qt::Key_Return){
        registerButtonClick();
    }
}

//向登录窗口发送关闭注册窗口信号
void Register::closeEvent(QCloseEvent *){
    emit closeRegisterWidget();
}
//向登录窗口发送返回登录窗口信号
void Register::backToLoginButtonClick(){
    emit backToLoginWidget();
}
//注册账户
void Register::registerButtonClick(){
    QString account = accountLineEdit->text();
    QString password = passwordLineEdit->text();
    QString confirePassword = confireLineEdit->text();
    if(account==""||password==""){
        QMessageBox::information(this, tr("注册失败"), tr("注册账号或密码为空。"));
        accountLineEdit->clear();
        passwordLineEdit->clear();
        confireLineEdit->clear();
    }
    else if(account.contains(" ")){
        QMessageBox::information(this, tr("注册失败"), tr("账户中不能有空格。"));
        accountLineEdit->clear();
        passwordLineEdit->clear();
        confireLineEdit->clear();
    }
    else if(password!=confirePassword){
        QMessageBox::information(this, tr("注册失败"), tr("两次输入密码不同，请重新输入密码。"));
        passwordLineEdit->clear();
        confireLineEdit->clear();
    }
    else{
        if(DatabaseOperation::selectAccount(account)){
            QMessageBox::information(this, tr("注册失败"), tr("\"%1\" 账户已存在。").arg(accountLineEdit->text()));
        }
        else{
            DatabaseOperation::addAccount(account, password, "user");
            QMessageBox::information(this, tr("注册成功"), tr("请登录。"));
            emit this->backToLoginWidget();
        }
    }
}
