#include "MainWindow.h"
#include "InitialWidget.h"

MainWindow::MainWindow(QWidget* parent) 
{
	initialize_window();
}


//初始化子窗口
void MainWindow::initialize_window()
{
	menu_bar();

	//创建表格并设置为中心窗口
	table = new Table;
	this->setCentralWidget(table);

}


//创建菜单栏
void MainWindow::menu_bar()
{
	qmb* menuBar = this->menuBar();

	//保存栏
	qa* updatemenu = menuBar->addAction("保存");

	//编辑栏
	qa* editmenu = menuBar->addAction("编辑");
	connect(editmenu, &qa::triggered, this, &MainWindow::On_editmenu_triggered);

	qa* deletemenu = menuBar->addAction("删除");

	qm* sortmenu = menuBar->addMenu("排序");


	qa* exportmenu = menuBar->addAction("导出");

	//返回栏
	qa* returnmenu = menuBar->addAction("返回");
	connect(returnmenu, &qa::triggered, this, &MainWindow::On_returnmenu_triggered);


	qw* search_Widget = searchWidget();
	menuBar->setCornerWidget(search_Widget, Qt::TopRightCorner);
}


//打开并检查数据库是否为空
bool MainWindow::isLocalDatabaseEmpty(QString path)
{
	table->OpenLocalDatabase( path);
	return table->isLocalDatabaseEmpty();
}


//创建搜索框
QWidget*  MainWindow::searchWidget()
{
	QWidget* searchWidget = new QWidget(this);
	QHBoxLayout* searchLayout = new QHBoxLayout(searchWidget);
	searchLayout->setContentsMargins(0, 4, 6, 0);  
	searchLayout->setAlignment(Qt::AlignCenter);  

	QLineEdit* searchEdit = new QLineEdit(this);
	searchEdit->setPlaceholderText("搜索...");
	searchEdit->setFixedWidth(220);   
	searchEdit->setMinimumHeight(26);  

	QAction* searchIcon = new QAction(searchEdit);
	searchIcon->setIcon(QIcon::fromTheme("edit-find"));
	searchEdit->addAction(searchIcon, QLineEdit::LeadingPosition);

	searchLayout->addWidget(searchEdit);

	return searchWidget;
}


void MainWindow::On_editmenu_triggered()
{
	table->setEditTriggers(QAbstractItemView::DoubleClicked);
}


//返回父窗口并关闭当前窗口
void MainWindow::On_returnmenu_triggered()
{
	
	if (ParentWidget) {
		ParentWidget->show();
		ParentWidget->raise();
		ParentWidget->activateWindow();
	}
	this->close();
}