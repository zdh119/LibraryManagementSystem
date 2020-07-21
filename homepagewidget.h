#ifndef HOMEPAGEWIDGET_H
#define HOMEPAGEWIDGET_H
#include <QWidget>
#include <QTableView>
#include <QSqlDatabase>
#include <QPushButton>
#include <QRadioButton>
#include <QCheckBox>
#include <QButtonGroup>
#include <QLineEdit>
#include <QSqlTableModel>
#include <QKeyEvent>
class HomePageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HomePageWidget(QWidget *parent = 0);
    QTableView *homePageTableView;
    QSqlTableModel *model;
    //存储当前选中的列号
    QModelIndex checkedIndex;

    //键盘事件
    void keyPressEvent(QKeyEvent *event);
    //刷新界面
    void updateHomePageTableView();
signals:
    void tableClick();
public slots:
    void searchButtonClick();
    void sortButtonClick();
    void homePageTableViewClick(QModelIndex index);
private:
    //搜索
    QRadioButton *searchByBookName;
    QRadioButton *searchByAuthor;
    QRadioButton *searchByID;
    QRadioButton *searchByCategory;
    QCheckBox *searchByFuzzy;
    QButtonGroup *searchGroup;
    int searchModel;
    //排序模式相关
    QRadioButton *sortByPrice;
    QRadioButton *sortByID;
    QButtonGroup *sortGroup;

    QLineEdit *searchLineEdit;
    QPushButton *searchButton;
    QPushButton *sortButton;
    QSqlDatabase db;
};

#endif // HOMEPAGEWIDGET_H
