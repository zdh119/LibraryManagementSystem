#ifndef ADMINWINDOW_H
#define ADMINWINDOW_H
#include "homepagewidget.h"
#include "bookinfowidget.h"
#include "addbookwidget.h"
#include "borrowinfowidget.h"
#include "deletebookwidget.h"
#include "mywidget.h"
#include <QMainWindow>

class AdminWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit AdminWindow(QString account, QString password,QWidget *parent = 0);
    //选项卡窗口
    QTabWidget *tabWidget;
    /*以下窗口添加到到选项卡窗口中*/
    //首页
    HomePageWidget *homePageWidget;
    //图书详细信息
    BookInfoWidget *bookInfoWidget;
    //借阅记录
    BorrowInfoWidget *borrowInfoWidget;
    //添加图书
    AddBookWidget *addBookWidget;
    //删除图书
    DeleteBookWidget *deleteBookWidget;
    //我的
    MyWidget *myWidget;
public slots:
    //切换图书详情窗口
    void changeToBookInfoWidget();
    //切换至删除图书窗口
    void changeToDeleteBookWidget();
    //注销账户槽函数
    void logoutActionClick();
    //帮助槽函数
    void helpActionClick();
    //事件过滤器——过滤回车键事件
    bool eventFilter(QObject *watched, QEvent *event);

    //添加、删除图书后，主页刷新
    void updateHomePage();
private:
    QString account;
    QString password;
};

#endif // ADMINWINDOW_H
