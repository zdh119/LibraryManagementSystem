#ifndef BORROWWIDGET_H
#define BORROWWIDGET_H

#include <QWidget>
#include <QDateEdit>
#include <QLineEdit>
#include <QKeyEvent>
#include <QPushButton>
class BorrowWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BorrowWidget(QString account, QWidget *parent = 0);

    void setBorrowBookID(QString bookID);
    void keyPressEvent(QKeyEvent *event);
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
    QPushButton *confirmButton;
};

#endif // BORROWWIDGET_H
