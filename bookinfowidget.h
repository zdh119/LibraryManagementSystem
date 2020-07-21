#ifndef BOOKINFOWIDGET_H
#define BOOKINFOWIDGET_H
#include "login.h"
#include "databaseoperation.h"
#include "deletebookwidget.h"
#include <QTableView>
#include <QWidget>
#include <QSqlDatabase>
#include <QTextEdit>

class BookInfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BookInfoWidget(Login::Model Model, QWidget *parent = 0);
    //显示图图书详细信息
    void setBookInfo(QString bookName);

    QTableView *bookInfoTableView;
    //管理员删除图书窗口
    DeleteBookWidget *deleteBookWidget;

    //存储当前选中的列号
    QModelIndex checkedIndex;

signals:
    //借书信号
    void borrowBookButtonClickSignal();
    //删除图书信号
    void deleteBookButtonClickSignal();
    //点击tableView信号
    void tableClick();
public slots:
    //发给借书信号用户窗口
    void borrowBookButtonClick();  
    void bookInfoTableViewClick(QModelIndex index);

    //管理员删除书籍，修改书籍信息
    void deleteBookButtonClick();
    void modifyBookButtonClick();
private:

    QSqlDatabase db;
    QLineEdit *bookNameLine;
    QLineEdit *authorNameLine;
    QLineEdit *presskNameLine;
    QLineEdit *ISBNLine;
    QLineEdit *publishDateLine;
    QLineEdit *categoryLine;
    QLineEdit *priceLine;
    QTextEdit *outlineText;
    QLineEdit *availableNumberLine;
    QLineEdit *totalNumberLine;
};

#endif // BOOKINFOWIDGET_H
