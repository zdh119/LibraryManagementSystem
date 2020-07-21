#ifndef LOGIN_H
#define LOGIN_H
#include "register.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>

class Login : public QWidget
{
    Q_OBJECT

public:
    //权限
    enum Model{User, Admin};
    Q_ENUM(Model)
    Login(QWidget *parent = 0);
    ~Login();
    //注册窗口
    Register *registerWidget;
protected slots:
    //用户登录
    void userLoginButtonClick();
    //管理员登录
    void adminLoginButtonClick();
    //注册
    void registerButtonClick();

private:
    QLabel *accountLabel;
    QLabel *passwordLabel;
    QLineEdit *accountLineEdit;
    QLineEdit *passwordLineEdit;
    QPushButton *userLoginButton;
    QPushButton *adminLoginButton;
    QPushButton *registerButton;
};

#endif // LOGIN_H
