#ifndef RETURNWIDGET_H
#define RETURNWIDGET_H
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>
#include <QKeyEvent>
class ReturnWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ReturnWidget(QString account, QWidget *parent = 0);

    void keyPressEvent(QKeyEvent *event);

public slots:
    //还书
    void returnButtonClick();
    //确认
    void confirmButtonClick();
private:
    QLabel *bookNameLabel;
    QLabel *borrowDateLabel;
    QLabel *returnDateLabel;
    QPushButton *confirmButton;
    QLineEdit *accountLine;
    QLineEdit *bookIDLine;
    QLineEdit *bookNameLine;
    QLineEdit *borrowDateLine;
    QLineEdit *returnDateLine;
    QPushButton *returnButton;
};

#endif // RETURNWIDGET_H
