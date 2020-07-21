#ifndef DELETEBOOKWIDGET_H
#define DELETEBOOKWIDGET_H

#include <QLineEdit>
#include <QPushButton>
#include <QSqlDatabase>
#include <QWidget>

class DeleteBookWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DeleteBookWidget(QWidget *parent = 0);

signals:
    void finishDeleteBook();
public slots:
    //查询要删除的书籍
    void searchBook();
    //删除书籍
    void deleteBook();

private:
    QSqlDatabase db;
    QLineEdit *bookIDLine;
    QLineEdit *bookNameLine;
    QPushButton *deleteBookButton;
};

#endif // DELETEBOOKWIDGET_H
