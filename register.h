#ifndef REGISTER_H
#define REGISTER_H

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>
#include <QKeyEvent>
class Register : public QWidget
{
    Q_OBJECT
public:
    explicit Register(QWidget *parent = 0);
    //重写关闭事件，当关闭注册窗口仍显示登录窗口
    void closeEvent(QCloseEvent *);
    void keyPressEvent(QKeyEvent *event);
signals:
    //返回登录窗口信号
    void backToLoginWidget();
    //关闭注册窗口信号
    void closeRegisterWidget();
protected slots:
    //注册账户
    void registerButtonClick();
    //返回登录窗口
    void backToLoginButtonClick();
private:
    QLineEdit *accountLineEdit;
    QLineEdit *passwordLineEdit;
    QLineEdit *confireLineEdit;
    QPushButton *registerButton;
    QPushButton *backToLoginButton;
};

#endif // REGISTER_H
