#ifndef BORROWINFOWIDGET_H
#define BORROWINFOWIDGET_H
#include "login.h"
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QTableView>
#include <QWidget>
#include <QKeyEvent>
class BorrowInfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BorrowInfoWidget(QWidget *parent = 0);
    QTableView *borrowInfoTableView;
    //键盘事件
    void keyPressEvent(QKeyEvent *event);
signals:

public slots:
    //搜索借阅记录
    void searchButtonClick();

private:
    QLineEdit *searchLineEdit;
    QPushButton *searchButton;
    QLabel *searchModelLabel;
    QRadioButton *searchByBookName;
    QRadioButton *searchByID;
    QRadioButton *searchByUserAccount;//管理员界面有，用户界面无
    QCheckBox *searchByFuzzy;//模糊搜索
    int searchModel;
    Login::Model Model;//账户模式
};

#endif // BORROWINFOWIDGET_H
