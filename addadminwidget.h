#ifndef ADDADMINWIDGET_H
#define ADDADMINWIDGET_H

#include <QLineEdit>
#include <QWidget>

class AddAdminWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AddAdminWidget(QWidget *parent = 0);
public slots:
    //添加管理员按钮槽函数
    void addAdminButtonClick();
private:
    //三个单行文本框，账户，密码，确认密码
    QLineEdit *accountLineEdit;
    QLineEdit *passwordLineEdit;
    QLineEdit *confireLineEdit;

};

#endif // ADDADMINWIDGET_H
