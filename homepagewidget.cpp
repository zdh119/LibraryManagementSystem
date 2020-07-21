#include "homepagewidget.h"
#include "databaseoperation.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QSqlTableModel>
#include <QDebug>
#include <QSqlError>
#include <QSqlRecord>
#include <QHeaderView>
#include <QMessageBox>
#include <QAction>
HomePageWidget::HomePageWidget(QWidget *parent) : QWidget(parent)
{

    homePageTableView = new QTableView(this);
    homePageTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //刷新
    QPushButton *updateButton = new QPushButton("刷新");
    //搜索相关
    searchLineEdit = new QLineEdit;
    QAction *searchAction = new QAction;
    searchAction->setIcon(QIcon(":/search.ico"));
    searchLineEdit->addAction(searchAction,QLineEdit::LeadingPosition);
    searchLineEdit->setPlaceholderText("请输入书名或ID或类别进行搜索——默认情况下，执行内容为空的模糊搜索");
    searchButton = new QPushButton("搜索");
    searchGroup = new QButtonGroup(this);
    QLabel *searchModelLabel = new QLabel("搜索模式");
    searchByBookName = new QRadioButton("书名");
    searchByBookName->setChecked(true);
    searchModel = 0;
    searchByAuthor = new QRadioButton("作者");
    searchByID = new QRadioButton("ID");
    searchByCategory = new QRadioButton("题材");
    searchByFuzzy = new QCheckBox("模糊");
    searchByFuzzy->setChecked(true);
    searchGroup->addButton(searchByBookName);
    searchGroup->addButton(searchByAuthor);
    searchGroup->addButton(searchByID);
    searchGroup->addButton(searchByCategory);
    //排序相关
    sortGroup = new QButtonGroup(this);
    sortButton = new QPushButton("排序");
    sortByPrice = new QRadioButton("价格");
    sortByID = new QRadioButton("ID");
    sortGroup->addButton(sortByPrice);
    sortGroup->addButton(sortByID);

    //布局
    QVBoxLayout *VBLayout = new QVBoxLayout;
    QHBoxLayout *searchLayout = new QHBoxLayout;
    QHBoxLayout *searchModelLayout = new QHBoxLayout;
    QHBoxLayout *sortModelLayout = new QHBoxLayout;
    searchLayout->addWidget(searchLineEdit);
    searchLayout->addWidget(searchButton);
    searchLayout->addWidget(updateButton);
    searchModelLayout->addWidget(searchModelLabel);
    searchModelLayout->addWidget(searchByBookName);
    searchModelLayout->addWidget(searchByAuthor);
    searchModelLayout->addWidget(searchByID);
    searchModelLayout->addWidget(searchByCategory);
    searchModelLayout->addWidget(searchByFuzzy);

    sortModelLayout->addWidget(sortByPrice);
    sortModelLayout->addWidget(sortByID);
    sortModelLayout->addWidget(sortButton);
    sortModelLayout->setAlignment(Qt::AlignRight);
    VBLayout->addLayout(searchLayout);
    VBLayout->addLayout(searchModelLayout);
    VBLayout->addLayout(sortModelLayout);
    VBLayout->addWidget(homePageTableView);

    this->setLayout(VBLayout);

    setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(this->backgroundRole(), QBrush(QPixmap(":/粉碎.png")));

    this->setPalette(palette);
    //显示数据库中所有书籍
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
        model = DatabaseOperation::tableShow("bookInfo", "", homePageTableView, db);
        homePageTableView->setModel(model);
        //隐藏id，概要，借出时间，归还时间
        homePageTableView->hideColumn(8);
        homePageTableView->hideColumn(10);
        homePageTableView->hideColumn(11);
    }
    homePageTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    connect(searchButton, &QPushButton::clicked, this, &HomePageWidget::searchButtonClick);
    connect(sortButton, &QPushButton::clicked, this, &HomePageWidget::sortButtonClick);
    connect(homePageTableView, &QTableView::doubleClicked, this, &HomePageWidget::homePageTableViewClick);
    connect(updateButton, &QPushButton::clicked, this, &HomePageWidget::updateHomePageTableView);
}
//搜索按钮槽函数
void HomePageWidget::searchButtonClick(){
    //获取搜索内容和模式
    QString  searchContent = searchLineEdit->text();
    if(!searchByFuzzy->isChecked()){//精确搜索
        if(searchByBookName->isChecked()) searchModel = 0;
        if(searchByAuthor->isChecked()) searchModel = 1;
        if(searchByID->isChecked()) searchModel = 2;
        if(searchByCategory->isChecked()) searchModel = 3;
    }
    else{
        if(searchByBookName->isChecked()) searchModel = 4;
        if(searchByAuthor->isChecked()) searchModel = 5;
        if(searchByID->isChecked()) searchModel = 6;
        if(searchByCategory->isChecked()) searchModel = 7;
    }
    if(!db.isOpen()){
        if(QSqlDatabase::contains("sqlite")){
            db = QSqlDatabase::database("sqlite");
        }
        else{
            db = QSqlDatabase::addDatabase("QSQLITE", "sqlite");
        }
    }
    db.setDatabaseName("../LibraryManagementSystem/LibraySystemDatabase.db");
    if(!db.open()){
        qDebug()<<"Error: Failed to connect database."<<db.lastError();
    }
    else{
        model = new QSqlTableModel(homePageTableView, db);
        model->setTable("bookInfo");
        switch(searchModel){
            //精确搜索
            case 0: model->setFilter("bookName = '"+searchContent+"'");break;
            case 1: model->setFilter("authorName = '"+searchContent+"'");break;
            case 2: model->setFilter("cast(id as varchar)= '"+searchContent+"'");break;
            case 3: model->setFilter("category = '"+searchContent+"'");break;
            //模糊搜索
            case 4: model->setFilter("bookName like '%"+searchContent+"%'");break;
            case 5: model->setFilter("authorName like '%"+searchContent+"%'");break;
            case 6: model->setFilter("cast(id as varcahr) like '%"+searchContent+"%'");break;
            case 7: model->setFilter("category like '%"+searchContent+"%'");break;
        }
        model->select();
        if(model->record().isEmpty()){
            QMessageBox::information(this, tr("搜素失败"), tr("系统中没有符合搜索内容的书籍"));
        }
        model->setHeaderData(0, Qt::Horizontal, tr("ID"));
        model->setHeaderData(1, Qt::Horizontal, tr("书名"));
        model->setHeaderData(2, Qt::Horizontal, tr("作者"));
        model->setHeaderData(3, Qt::Horizontal, tr("出版社"));
        model->setHeaderData(4, Qt::Horizontal, tr("出版时间"));
        model->setHeaderData(5, Qt::Horizontal, tr("价格"));
        model->setHeaderData(6, Qt::Horizontal, tr("ISBN"));
        model->setHeaderData(7, Qt::Horizontal, tr("类别"));

        homePageTableView->setModel(model);
        homePageTableView->hideColumn(8);
        homePageTableView->hideColumn(9);
        homePageTableView->hideColumn(10);
        homePageTableView->hideColumn(11);
    }

}
void HomePageWidget::keyPressEvent(QKeyEvent *event){
    if(event->key()==Qt::Key_Return){
        searchButtonClick();
    }
}

void HomePageWidget::sortButtonClick(){
    if(sortByID->isChecked()){
        model->sort(0, Qt::AscendingOrder);
    }

    else if(sortByPrice->isChecked()){
        model->sort(5, Qt::AscendingOrder);
    }
    homePageTableView->setModel(model);
    homePageTableView->hideColumn(8);
    homePageTableView->hideColumn(10);
    homePageTableView->hideColumn(11);
}
//点击table双击信号槽函数,只对书名响应
void HomePageWidget::homePageTableViewClick(QModelIndex index){
    checkedIndex = index;
    emit tableClick();
}
void HomePageWidget::updateHomePageTableView(){
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
        model = DatabaseOperation::tableShow("bookInfo", "", homePageTableView, db);
        homePageTableView->setModel(model);
        //隐藏id，概要，借出时间，归还时间
        homePageTableView->hideColumn(8);
        homePageTableView->hideColumn(10);
        homePageTableView->hideColumn(11);
    }
}
