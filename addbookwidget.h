#ifndef ADDBOOKWIDGET_H
#define ADDBOOKWIDGET_H

#include <QLineEdit>
#include <QTextEdit>
#include <QWidget>

class AddBookWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AddBookWidget(QWidget *parent = 0);

signals:
    void finishAddBook();
public slots:
    //管理员添加书籍
    void addBookButtonClick();
private:
    QLineEdit *bookNameLine;
    QLineEdit *authorNameLine;
    QLineEdit *pressNameLine;
    QLineEdit *ISBNLine;
    QLineEdit *publishDateLine;
    QLineEdit *categoryLine;
    QLineEdit *priceLine;
    QLineEdit *totalNumberLine;
    QTextEdit *outlineText;

};

#endif // ADDBOOKWIDGET_H
