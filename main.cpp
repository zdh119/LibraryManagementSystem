#include "login.h"
#include "userwindow.h"
#include "bookinfowidget.h"
#include "adminwindow.h"
#include "homepagewidget.h"
#include <QApplication>
#include <QFile>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile file(":/style.qss");
    file.open(QFile::ReadOnly);
    if(!file.isOpen()){
        qDebug()<<"file failed open";
    }
    else{
        QString styleSheet = QString::fromLatin1(file.readAll());
        qApp->setStyleSheet(styleSheet);
        Login *w = new Login;
        w->show();
    }

    return a.exec();
}
