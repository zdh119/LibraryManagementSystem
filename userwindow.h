#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "homepagewidget.h"
#include "bookinfowidget.h"
#include "borrowwidget.h"
#include "returnwidget.h"
#include "borrowinfowidget.h"
#include "mywidget.h"
#include <QMainWindow>

class UserWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit UserWindow(QString account, QString password, QWidget *parent = 0);
    //选项卡窗口
    QTabWidget *tabWidget;
    //以下窗口添加到到选项卡窗口中
    //主页
    HomePageWidget *homePageWidget;
    //图书详细信息
    BookInfoWidget *bookInfoWidget;
    //借阅图书
    BorrowWidget *borrowWidget;
    //归还图书
    ReturnWidget *returnWidget;
    //借阅记录
    BorrowInfoWidget *borrowInfoWidget;
    //我的
    MyWidget *myWidget;

public slots:
    //切换至图书详情窗口
    void changeToBookInfoWidget();
    //切换至借阅窗口
    void changeToBorrowWidget();
    //注销账户槽函数
    void logoutActionClick();
    //帮助槽函数
    void helpActionClick();
    //事件过滤器——用于过滤回车键事件
    bool eventFilter(QObject *watched, QEvent *event);


private:
    QString account;
    QString password;
};

#endif // MAINWINDOW_H
