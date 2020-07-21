#include "borrowinfowidget.h"

#include <QHBoxLayout>
#include <QSqlQueryModel>
#include <QDebug>
#include <QSqlError>
#include <QHeaderView>
#include <QAction>
BorrowInfoWidget::BorrowInfoWidget( QWidget *parent) : QWidget(parent)
{

    borrowInfoTableView = new QTableView;
    //搜索相关
    searchLineEdit = new QLineEdit;
    QAction *searchAction = new QAction;
    searchAction->setIcon(QIcon(":/search.ico"));
    searchLineEdit->addAction(searchAction,QLineEdit::LeadingPosition);
    searchLineEdit->setPlaceholderText("请输入书名或ID或类别进行搜索——默认情况下，执行内容为空的模糊搜索");
    searchButton = new QPushButton("搜索");
    searchModelLabel = new QLabel("搜索模式");
    searchByBookName = new QRadioButton("书名");
    searchModel = 0;
    searchByID = new QRadioButton("ID");
    searchByUserAccount = new QRadioButton("用户");
    searchByUserAccount->setChecked(true);
    searchByFuzzy = new QCheckBox("模糊");
    searchByFuzzy->setChecked(true);

    //布局
    QVBoxLayout *VBLayout = new QVBoxLayout;
    QHBoxLayout *searchLayout = new QHBoxLayout;
    QHBoxLayout *searchModelLayout = new QHBoxLayout;
    searchLayout->addWidget(searchLineEdit);
    searchLayout->addWidget(searchButton);
    searchModelLayout->addWidget(searchModelLabel);
    searchModelLayout->addWidget(searchByBookName);
    searchModelLayout->addWidget(searchByID);
    searchModelLayout->addWidget(searchByUserAccount);
    searchModelLayout->addWidget(searchByFuzzy);

    VBLayout->addLayout(searchLayout);
    VBLayout->addLayout(searchModelLayout);
    VBLayout->addWidget(borrowInfoTableView);
    borrowInfoTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    setLayout(VBLayout);

    setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(this->backgroundRole(), QBrush(QPixmap(":/布质花纹.jpg")));
    this->setPalette(palette);
    //权限模式
    this->Model = Model;

    connect(searchButton, &QPushButton::clicked, this, &BorrowInfoWidget::searchButtonClick);
}
//搜索借阅记录
void BorrowInfoWidget::searchButtonClick(){
    //获取搜索内容和模式
    QString  searchContent = searchLineEdit->text();
    if(!searchByFuzzy->isChecked()){//精确搜索
        if(searchByBookName->isChecked()) searchModel = 0;
        if(searchByID->isChecked()) searchModel = 1;
        if(searchByUserAccount->isChecked()) searchModel = 2;
    }
    else{//模糊搜索
        if(searchByBookName->isChecked()) searchModel = 3;
        if(searchByID->isChecked()) searchModel = 4;
        if(searchByUserAccount->isChecked()) searchModel = 5;
    }
    QSqlDatabase db;
    if(QSqlDatabase::contains("sqlite")){
        db = QSqlDatabase::database("sqlite");
    }
    else{
        db = QSqlDatabase::addDatabase("QSQLITE", "sqlite");
    }
    db.setDatabaseName("../LibraryManagementSystem/LibraySystemDatabase.db");
    if(!db.open()){
        qDebug()<<"Error: Failed to connect database."<<db.lastError();
    }
    else{
        QSqlQueryModel *model = new QSqlQueryModel(borrowInfoTableView);
        switch(searchModel){
            //精确搜索
            case 0: model->setQuery("select * from borrowInfo where bookName = '"+searchContent+"'" , db);break;
            case 1: model->setQuery("select * from borrowInfo where cast(id as varchar) = '"+searchContent+"'", db); break;
            case 2: model->setQuery("select * from borrowInfo where account = '"+searchContent+"'" , db);break;
            //模糊搜索
            case 3: model->setQuery("select * from borrowInfo where bookName like '%"+searchContent+"%'" , db);break;
            case 4: model->setQuery("select * from borrowInfo where cast(id as varchar) like '%"+searchContent+"%'" , db);break;
            case 5: model->setQuery("select * from borrowInfo where account like '%"+searchContent+"%'" , db);break;
        }
        model->setHeaderData(0, Qt::Horizontal, "ID");
        model->setHeaderData(1, Qt::Horizontal, "书名");
        model->setHeaderData(2, Qt::Horizontal, "账户");
        model->setHeaderData(3, Qt::Horizontal, "出借时间");
        model->setHeaderData(4, Qt::Horizontal, "归还时间");

        borrowInfoTableView->setModel(model);
    }
    db.close();
}
void BorrowInfoWidget::keyPressEvent(QKeyEvent *event){
    if(event->key()==Qt::Key_Return){
        searchButtonClick();
    }
}
