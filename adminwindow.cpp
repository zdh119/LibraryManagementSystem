#include "adminwindow.h"
#include "login.h"
#include "addbookwidget.h"
#include "helpwidget.h"
#include <QAction>
#include <QMenuBar>
#include <QDebug>

AdminWindow::AdminWindow(QString account, QString password, QWidget *parent) : QMainWindow(parent)
{

    this->account = account;
    this->password = password;
    resize(800,600);
    //窗口部件
    tabWidget = new QTabWidget(this);
    homePageWidget = new HomePageWidget;
    homePageWidget->installEventFilter(this);
    bookInfoWidget = new BookInfoWidget(Login::Admin);
    borrowInfoWidget = new BorrowInfoWidget;
    borrowInfoWidget->installEventFilter(this);
    addBookWidget = new AddBookWidget;
    deleteBookWidget = new DeleteBookWidget;
    myWidget = new MyWidget(Login::Admin, account, password);
    tabWidget->addTab(homePageWidget, "首页");
    tabWidget->addTab(bookInfoWidget, "图书详细信息");
    tabWidget->addTab(borrowInfoWidget, "借阅记录");
    tabWidget->addTab(addBookWidget, "添加图书");
    tabWidget->addTab(deleteBookWidget, "删除图书");
    tabWidget->addTab(myWidget, "我的");
    this->setCentralWidget(tabWidget);
    QAction *helpAction = menuBar()->addAction("帮助");
    helpAction->setIcon(QIcon(":/help.ico"));
    QAction *logoutAction = menuBar()->addAction("注销");
    logoutAction->setIcon(QIcon(":/logout.ico"));

    setWindowTitle(account+" 管理员界面");
    setWindowIcon(QIcon(":/admin.ico"));

    connect(helpAction, &QAction::triggered, this, &AdminWindow::helpActionClick);
    connect(logoutAction, &QAction::triggered, this, &AdminWindow::logoutActionClick);
    connect(homePageWidget, &HomePageWidget::tableClick, this, &AdminWindow::changeToBookInfoWidget);
    connect(bookInfoWidget, &BookInfoWidget::deleteBookButtonClickSignal, this, &AdminWindow::changeToDeleteBookWidget);
    connect(addBookWidget, &AddBookWidget::finishAddBook, this, &AdminWindow::updateHomePage);
    connect(deleteBookWidget, &DeleteBookWidget::finishDeleteBook, this, &AdminWindow::updateHomePage);
}
//事件过滤器
bool AdminWindow::eventFilter(QObject *watched, QEvent *event){
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
void AdminWindow::helpActionClick(){
    HelpWidget *helpWidget = new HelpWidget;
    helpWidget->show();
}
//注销账户
void AdminWindow::logoutActionClick(){
    close();
    Login *login = new Login;
    login->show();
}

//响应首页点击书名信号，切换到图书详情窗口
void AdminWindow::changeToBookInfoWidget(){
    if(homePageWidget->checkedIndex.column()==1){
        QString bookName = homePageWidget->homePageTableView->model()->data(homePageWidget->checkedIndex).toString();
        bookInfoWidget->setBookInfo(bookName);
        tabWidget->setCurrentIndex(1);
    }
}
//切换至删除图书窗口
void AdminWindow::changeToDeleteBookWidget(){
    tabWidget->setCurrentIndex(4);
}

//数据改变时，对主页进行刷新
void AdminWindow::updateHomePage(){
    homePageWidget->updateHomePageTableView();
}
