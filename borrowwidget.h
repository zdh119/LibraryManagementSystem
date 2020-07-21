#ifndef BORROWWIDGET_H
#define BORROWWIDGET_H

#include <QWidget>
#include <QDateEdit>
#include <QLineEdit>
class BorrowWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BorrowWidget(QString account, QWidget *parent = 0);

    void setBorrowBookID(QString bookID);
signals:

public slots:
    //确认借书
    void confirmButtonClick();

private:
    QLineEdit *accoutLine;
    QLineEdit *bookIDLine;
    //当前借书时间
    QLineEdit *borrowDateLine;
    //归还时间
    QDateEdit *returnDateEdit;

};

#endif // BORROWWIDGET_H
