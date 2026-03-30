#include "MainWindow.h"
#include "InitialWidget.h"

#include<QAbstractItemView>

MainWindow::MainWindow(QWidget* parent) 
{
	is_allowed_edited = false;
	is_sort_changed = false;
	current_sort = -2;
	initialize_window();
}


//初始化子窗口
void MainWindow::initialize_window()
{
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
	connect(updatemenu, &qa::triggered, this, &MainWindow::On_save_menu_triggered);

	//编辑栏
	qa* editmenu = menuBar->addAction("编辑");
	connect(editmenu, &qa::triggered, this,  & MainWindow::On_editmenu_triggered);

	//取消栏
	qa* cancelmenu = menuBar->addAction("取消");
	connect(cancelmenu, &qa::triggered, this, &MainWindow::On_cancelmenu_triggered);

	qa* deletemenu = menuBar->addAction("删除");

	//排序栏
	qm* sortmenu = menuBar->addMenu("排序");
	qa* sortselection1 = sortmenu->addAction("学号");
	connect(sortselection1, &qa::triggered, this, [=]() {
		table->id_sort();
		table->class_to_table(table->get_current_studentlist());
		is_sort_changed = true;
		current_sort = -1;
		});

	vector<Student> s = table->get_saved_studentlist();
	vector<CourseScore> c = s[0].getCourses();
	int n = c.size();
	vector<int> courseCount;
	for (int i = 0; i < n; i++)
		courseCount.push_back(i);
	int j = 0;
	for (auto& x : c)
	{
		qa* sortselection = new qa(QString::fromStdString(x.getCourseName()));
		sortmenu->addAction(sortselection);
		connect(sortselection, &qa::triggered, this, [=]() {
			table->customed_sort(courseCount[j]);
			table->class_to_table(table->get_current_studentlist()); 
			current_sort = j;
			is_sort_changed = true;
			});
		j++;
	}

	qa* sortselection2 = sortmenu->addAction("平均学分成绩");
	connect(sortselection2, &qa::triggered, this, [=]() {
		if (is_sort_changed)
		{
			table->default_sort();
			table->class_to_table(table->get_current_studentlist());
		}
		current_sort = -2;
		is_sort_changed = false;
		});


	qa* exportmenu = menuBar->addAction("导出");
	connect(exportmenu, &qa::triggered, this, &MainWindow::On_exportmenu_triggered);

	//返回栏
	qa* returnmenu = menuBar->addAction("返回");
	connect(returnmenu, & qa::triggered, this,  & MainWindow::On_returnmenu_triggered);


	qw* search_Widget = searchWidget();
	menuBar->setCornerWidget(search_Widget, Qt::TopRightCorner);
}


//打开并检查数据库是否为空
bool MainWindow::isLocalDatabaseEmpty(QString path)
{
	table->OpenLocalDatabase( path);
	menu_bar();
	return table->isLocalDatabaseEmpty();
}

void MainWindow::keep_sort_measure(ScoreManager* manager)
{
	if (current_sort == -1)
		manager->Sort_by_id();
	else if (current_sort == -2)
		manager->default_Sort();
	else
		manager->Sort_by_course(current_sort);
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
	connect(searchEdit, &QLineEdit::returnPressed, this,
		[=]() { On_searchedit_Changed(searchEdit->text().trimmed());});

	QAction* searchIcon = new QAction(searchEdit);
	searchIcon->setIcon(QIcon::fromTheme("edit-find"));
	searchEdit->addAction(searchIcon, QLineEdit::LeadingPosition);

	searchLayout->addWidget(searchEdit);

	return searchWidget;
}

//保存表格内容到数据库，并设置表格不可修改
void MainWindow::On_save_menu_triggered()
{
	if(is_allowed_edited)
	{
		table->clearSelection();
		table->updateStudent();
		ScoreManager* manager = new ScoreManager(table->get_saved_studentlist());
		keep_sort_measure(manager);
		table->class_to_table(manager->getStudents());
		menuBar()->clear();
		menu_bar();
	}
	is_allowed_edited = false;
}


//使用户可以双击编辑表格内容
void MainWindow::On_editmenu_triggered()
{
	is_allowed_edited = true;
	table->setEditTriggers(QAbstractItemView::DoubleClicked);
}

//取消编辑，恢复表格内容为上次保存的状态
void MainWindow::On_cancelmenu_triggered()
{
	if (is_allowed_edited)
	{
		table->clearSelection();
		ScoreManager* manager = new ScoreManager(table->get_saved_studentlist());
		keep_sort_measure(manager);
		table->class_to_table(manager->getStudents());
		table->setEditTriggers(QAbstractItemView::NoEditTriggers);
	}
	is_allowed_edited = false;
}


void MainWindow::On_exportmenu_triggered()
{
	CSV_Helper* csv = new CSV_Helper(table->get_saved_studentlist());
	QString newFilePath = QFileDialog::getSaveFileName(
		this,
		"导出为CSV",
		QDir::currentPath() + "/CSV/students.csv", // 默认路径+文件名
		"CSV文件 (*.csv)"
	);
	if (!newFilePath.isEmpty())
	{
		csv->export_to_csv(newFilePath.toStdString());
	}
	delete csv;
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


//搜索框内容改变时的槽函数，遍历表格内容并选中包含搜索关键字的行
void MainWindow::On_searchedit_Changed(const QString& key)
{
	table->clearSelection();
	if (key.isEmpty())
		return;
	int rowCount = table->rowCount();
	table->setSelectionMode(QAbstractItemView::MultiSelection);
	table->setSelectionBehavior(QAbstractItemView::SelectRows);
	for(int i = 2; i < rowCount; ++i) 
	{
		for (int j = 1; j < 4; ++j) 
		{
			QTableWidgetItem* item = table->item(i, j);
			if (item && item->text().contains(key, Qt::CaseInsensitive)) 
			{
				table->selectRow(i);   
				break;
			}
		}
	}
	table->setSelectionMode(QAbstractItemView::SingleSelection);
	table->setSelectionBehavior(QAbstractItemView::SelectItems);
	if (table->selectedItems().isEmpty()) 
	{
		QMessageBox::information(this, "  ", "未找到匹配的记录。");
	}
	table->setFocus();
}
