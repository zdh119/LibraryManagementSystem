#ifndef MYWIDGET_H
#define MYWIDGET_H
#include "login.h"
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
class MyWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MyWidget(Login::Model Model, QString account, QString password,QWidget *parent = 0);
    //账户密码框
    QLineEdit *accountLineEdit;
    QLineEdit *passwordLineEdit;

public slots:
    //修改密码按钮槽函数
    void changePasswordButtonClick();
    //提交按钮槽函数
    void submitButtonClick();
    //管理员模式下，添加管理员账户
    void addAdminAccountButtonClick();
private:
    QString account;
    QString password;
    QLabel *accountLabel;
    QLabel *passwordLabel;
    QLabel *changePasswordLabel;
    QLineEdit *changePasswordLineEdit;
    QPushButton *changePasswordButton;
    QPushButton *submitButton;
    QPushButton *addAdminAccountButton;

};

#endif // MYWIDGET_H
