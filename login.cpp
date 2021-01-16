#include "login.h"
#include "userwindow.h"
#include "adminwindow.h"
#include "databaseoperation.h"
#include <QHBoxLayout>
#include <QMessageBox>

Login::Login(QWidget *parent) :
    QWidget(parent)
{
    resize(600,400);
    registerWidget = new Register;
    accountLabel = new QLabel("账户：");
    accountLabel->setAlignment(Qt::AlignCenter);
    accountLineEdit = new QLineEdit(this);
    accountLineEdit->setPlaceholderText("请输入账户名");
    accountLineEdit->setMinimumSize(400,50);
    passwordLabel = new QLabel("密码：");
    passwordLabel->setAlignment(Qt::AlignCenter);
    passwordLineEdit = new QLineEdit(this);
    passwordLineEdit->setPlaceholderText("请输入密码");
    passwordLineEdit->setEchoMode(QLineEdit::PasswordEchoOnEdit);
    passwordLineEdit->setMinimumSize(400,50);
    //按钮
    userLoginButton = new QPushButton("用户登录");
    adminLoginButton = new QPushButton("管理员登录");
    registerButton = new QPushButton("注册");

    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->addWidget(accountLabel, 2, 0);
    gridLayout->addWidget(accountLineEdit, 2, 1, 1, 2);
    gridLayout->addWidget(passwordLabel, 3, 0);
    gridLayout->addWidget(passwordLineEdit, 3, 1, 1, 2);
    gridLayout->addWidget(registerButton, 4, 0);
    gridLayout->addWidget(userLoginButton, 4, 1);
    gridLayout->addWidget(adminLoginButton, 4, 2);
    setLayout(gridLayout);
    setWindowIcon(QIcon(":/system.ico"));
    setWindowTitle("图书管理系统登录");
    //设置背景图片
    setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(this->backgroundRole(), QBrush(QPixmap(":/绿色圆环.png")));
    this->setPalette(palette);

    connect(userLoginButton, &QPushButton::clicked, this, &Login::userLoginButtonClick);
    connect(adminLoginButton, &QPushButton::clicked, this, &Login::adminLoginButtonClick);
    connect(registerButton, &QPushButton::clicked, this, &Login::registerButtonClick);
    //点击注册返回按钮后，返回登陆界面
    connect(registerWidget,&Register::backToLoginWidget,[=](){
        registerWidget->hide();
        this->show();
    });
    //关闭注册窗口后仍显示登录界面
    connect(registerWidget,&Register::closeRegisterWidget,[=](){
        this->show();
    });
}

Login::~Login()
{

}
void Login::keyPressEvent(QKeyEvent *event){
    if(event->key()==Qt::Key_Return) {
        userLoginButtonClick();
    }
}

//用户登录
void Login::userLoginButtonClick(){
    QString account = accountLineEdit->text();
    QString password = passwordLineEdit->text();
    if(!DatabaseOperation::checkAccount(account, password, "user")){
        QMessageBox::information(this, tr("登录失败"), tr("账户不存在或密码不正确。"));
        passwordLineEdit->clear();
    }
    else{
        close();
        UserWindow *userwindow = new UserWindow(account, password);
        userwindow->show();
    }
}
//管理员登录
void Login::adminLoginButtonClick(){
    QString account = accountLineEdit->text();
    QString password = passwordLineEdit->text();
    if(!DatabaseOperation::checkAccount(account, password, "admin")){
        QMessageBox::information(this, tr("登录失败"), tr("账户不存在或密码不正确。"));
    }
    else{
        close();
        AdminWindow *adminwindow = new AdminWindow(account, password);
        adminwindow->show();
    }
}
//注册
void Login::registerButtonClick(){
    this->hide();
    registerWidget->show();
}
