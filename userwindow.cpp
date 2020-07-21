#include "userwindow.h"
#include "login.h"
#include "borrowwidget.h"
#include "returnwidget.h"
#include "helpwidget.h"
#include <QAction>
#include <QMenuBar>
UserWindow::UserWindow(QString account, QString password, QWidget *parent) : QMainWindow(parent)
{
    this->account = password;
    this->password = password;
    resize(800,600);
    //窗口部件
    tabWidget = new QTabWidget(this);
    homePageWidget = new HomePageWidget;
    homePageWidget->installEventFilter(this);
    bookInfoWidget = new BookInfoWidget(Login::User);
    borrowWidget = new BorrowWidget(account);
    returnWidget = new ReturnWidget(account);
    borrowInfoWidget = new BorrowInfoWidget;
    borrowInfoWidget->installEventFilter(this);
    myWidget = new MyWidget(Login::User,account, password);
    tabWidget->addTab(homePageWidget, "首页");
    tabWidget->addTab(bookInfoWidget, "图书详细信息");
    tabWidget->addTab(borrowWidget, "借书");
    tabWidget->addTab(returnWidget, "还书");
    tabWidget->addTab(borrowInfoWidget, "借阅记录");
    tabWidget->addTab(myWidget, "我的");
    this->setCentralWidget(tabWidget);

    QAction *helpAction = menuBar()->addAction("帮助");
    helpAction->setIcon(QIcon(":/help.ico"));
    QAction *logoutAction = menuBar()->addAction("注销");
    logoutAction->setIcon(QIcon(":/logout.ico"));
    logoutAction->setIconText("注销");

    setWindowTitle(account + " 用户界面");
    setWindowIcon(QIcon(":/widget.ico"));
    connect(helpAction, &QAction::triggered, this, &UserWindow::helpActionClick);
    connect(logoutAction, &QAction::triggered, this, &UserWindow::logoutActionClick);
    connect(homePageWidget, &HomePageWidget::tableClick, this, &UserWindow::changeToBookInfoWidget);
    connect(bookInfoWidget, &BookInfoWidget::borrowBookButtonClickSignal, this, &UserWindow::changeToBorrowWidget);
    connect(bookInfoWidget, &BookInfoWidget::tableClick, this, &UserWindow::changeToBorrowWidget);
}

//事件过滤器
bool UserWindow::eventFilter(QObject *watched, QEvent *event){
    if(watched==homePageWidget){
        if(event->type()==QEvent::KeyPress){
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            homePageWidget->keyPressEvent(keyEvent);
        }
    }
    else if(watched==borrowInfoWidget){
        if(event->type()==QEvent::KeyPress){
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            borrowInfoWidget->keyPressEvent(keyEvent);
        }
    }
    return QMainWindow::eventFilter(watched, event);
}

//帮助
void UserWindow::helpActionClick(){
    HelpWidget *helpWidget = new HelpWidget;
    helpWidget->show();
}
//注销账户
void UserWindow::logoutActionClick(){
    close();
    Login *login = new Login;
    login->show();
}

//响应首页点击书名信号，切换到图书详情窗口
void UserWindow::changeToBookInfoWidget(){
    if(homePageWidget->checkedIndex.column()==1){
        QString bookName = homePageWidget->homePageTableView->model()->data(homePageWidget->checkedIndex).toString();
        bookInfoWidget->setBookInfo(bookName);
        tabWidget->setCurrentIndex(1);
    }
}
//点击图书详细信息的窗口的借阅按钮，切换至借阅窗口
void UserWindow::changeToBorrowWidget(){
    if(bookInfoWidget->checkedIndex.isValid()&&bookInfoWidget->checkedIndex.column()==0){
        QString bookID = bookInfoWidget->bookInfoTableView->model()->data(bookInfoWidget->checkedIndex).toString();
        borrowWidget->setBorrowBookID(bookID);
    }
    tabWidget->setCurrentIndex(2);
}

