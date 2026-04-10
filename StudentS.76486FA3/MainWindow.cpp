#include "MainWindow.h"
#include "InitialWidget.h"
#include "LoginWidget.h"

#include<QAbstractItemView>
#include <QInputDialog>

MainWindow::MainWindow(QWidget* parent) 
{
	is_allowed_edited = false;
	is_sort_changed = false;
	current_sort = -2;
	isTeacher = false;
	initialize_window();
}


//初始化子窗口
void MainWindow::initialize_window()
{
	setWindowTitle("NJUST学生成绩管理系统");
	//设置close时调用析构函数
	setAttribute(Qt::WA_DeleteOnClose);
	QScreen* myDesktop = QGuiApplication::primaryScreen();
	QRect myWholeDesktop = myDesktop->geometry();
	QRect myAvailableDesktop = myDesktop->availableGeometry();
	resize(myWholeDesktop.width() * 0.85, myWholeDesktop.height() * 0.85);
	move((myWholeDesktop.width() - width()) / 2, (myWholeDesktop.height() - height()) * 0.8 / 2);
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

	//添加栏
	qa* addmenu = menuBar->addAction("添加");
	connect(addmenu, &qa::triggered, this, [=]() 
	{ 
		table->addEmptyStudent(); 
	});
	
	//删除栏
	qa* deletemenu = menuBar->addAction("删除");
	connect(deletemenu, &qa::triggered, this, [=]() 
	{ 
		table->deleteSelectedStudent(); 
	});

	//排序栏
	qm* sortmenu = menuBar->addMenu("排序");

	qa* sortselection1 = sortmenu->addAction("学号");
	connect(sortselection1, &qa::triggered, this, [=]() 
	{
		is_sort_changed = true;
		current_sort = -1;
		keep_sort_measure();
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
		connect(sortselection, &qa::triggered, this, [=]() 
		{
			current_sort = j;
			is_sort_changed = true;
			keep_sort_measure();
		});
		j++;
	}

	qa* sortselection2 = sortmenu->addAction("平均学分成绩");
	connect(sortselection2, &qa::triggered, this, [=]() 
	{
		if (is_sort_changed)
		{
			current_sort = -2;
			is_sort_changed = false;
			keep_sort_measure();
		}
	});


	//筛选栏
	qm* queryMenu = menuBar->addMenu("筛选");

	qa* byName = queryMenu->addAction("按姓名筛查");
	connect(byName, &qa::triggered, this, [=]() 
		{
		bool ok;
		/*
		QLineEdit::Normal是Qt中QLineEdit类的一个枚举值，用于设置输入框的显示模式。
		它是最常用的普通模式，表示：用户输入的字符原样显示（不隐藏、不做特殊转换）
		*/
		QString name = QInputDialog::getText(this, "NJUST学生成绩管理系统", "请输入姓名:", QLineEdit::Normal, "", &ok); 
		if (ok && !name.isEmpty()) 
		{
			table->filter_by_name(name.toStdString());
			keep_sort_measure();
		}
		});

	qa* byDept = queryMenu->addAction("按院系筛查");
	connect(byDept, &qa::triggered, this, [=]() 
		{
		bool ok;
		QString dept = QInputDialog::getText(this, "NJUST学生成绩管理系统", "请输入院系名称:", QLineEdit::Normal, "", &ok);
		if (ok && !dept.isEmpty()) 
		{
			table->filter_by_dep(dept.toStdString());
			keep_sort_measure();
		}
		});

	qa* byCourseMin = queryMenu->addAction("按指定课程成绩（≥）");
	connect(byCourseMin, &qa::triggered, this, [=]()
		{
			vector<Student> students = table->get_current_studentlist();
			vector<CourseScore> courses = students[0].getCourses();
			QStringList courseNames;
			for (int i = 0; i < courses.size(); ++i)
			{
				courseNames << QString::fromStdString(courses[i].getCourseName());
			}
			bool ok;
			/*
			0			//默认选中第几项（0 表示第一项）
			false		//是否允许用户手动输入
			*/
			QString course = QInputDialog::getItem(this, "选择课程", "课程:", courseNames, 0, false, &ok);
			if (!ok) return;
			double minScore = QInputDialog::getDouble(this, "", "请输入最低成绩（≥）:", 0, 0, 100, 1, &ok);
			if (ok)
			{
				table->filter_by_CourseScoreMin(course.toStdString(), minScore);
				keep_sort_measure();
			}
		});

	qa* byCourseMax = queryMenu->addAction("按指定课程成绩（≤）");
	connect(byCourseMax, &qa::triggered, this, [=]()
		{
			vector<Student> students = table->get_current_studentlist();
			vector<CourseScore> courses = students[0].getCourses();
			QStringList courseNames;
			for (int i = 0; i < courses.size(); ++i)
			{
				courseNames << QString::fromStdString(courses[i].getCourseName());
			}
			bool ok;
			QString course = QInputDialog::getItem(this, "选择课程", "课程:", courseNames, 0, false, &ok);
			if (!ok) return;
			double maxScore = QInputDialog::getDouble(this, "", "请输入最高成绩（≤）:", 100, 0, 100, 1, &ok);
			if (ok)
			{
				table->filter_by_CourseScoreMax(course.toStdString(), maxScore);
				keep_sort_measure();
			}
		});

	qa* byExcel = queryMenu->addAction("按课程全部优秀");
	connect(byExcel, &qa::triggered, this, [=]()
		{
			vector<Student> students = table->get_current_studentlist();
			
			table->filter_by_Excel();
			keep_sort_measure();
		});

	qa* byMiddle = queryMenu->addAction("按课程全部良好");
	connect(byMiddle, &qa::triggered, this, [=]()
		{
			vector<Student> students = table->get_current_studentlist();
			table->filter_by_Middle();
			keep_sort_measure();
		});

	qa* byPass = queryMenu->addAction("按课程全部及格");
	connect(byPass, &qa::triggered, this, [=]()
		{
			vector<Student> students = table->get_current_studentlist();
			table->filter_by_Pass();
			keep_sort_measure();
		});

	qa* byFail = queryMenu->addAction("按任一课程不及格");
	connect(byFail, &qa::triggered, this, [=]()
		{
			vector<Student> students = table->get_current_studentlist();
			table->filter_by_Fail();
			keep_sort_measure();
		});



	qa* clear = queryMenu->addAction("显示全部");
	connect(clear, &qa::triggered, this, [=]() 
		{
		table->clearFilter();
		keep_sort_measure();
		});

	//导出栏
	qa* exportmenu = menuBar->addAction("导出");
	connect(exportmenu, &qa::triggered, this, &MainWindow::On_exportmenu_triggered);

	//返回栏
	qa* returnmenu = menuBar->addAction("返回");
	connect(returnmenu, & qa::triggered, this,  & MainWindow::On_returnmenu_triggered);

	updatemenu->setEnabled(isTeacher);
	editmenu->setEnabled(isTeacher);
	cancelmenu->setEnabled(isTeacher);
	addmenu->setEnabled(isTeacher);
	deletemenu->setEnabled(isTeacher);

	qw* search_Widget = searchWidget();
	menuBar->setCornerWidget(search_Widget, Qt::TopRightCorner);
}



//根据当前排序方式对学生列表进行排序并显示，以保持表格内容的排序状态不变
void MainWindow::keep_sort_measure()
{
	if (current_sort == -1)
		table->id_sort();
	else if (current_sort == -2)
		table->default_sort();
	else
		table->customed_sort(current_sort);
	table->class_to_table(table->get_current_studentlist());
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
		keep_sort_measure();
		if(table->course_changed())
		{
			menuBar()->clear();
			menu_bar();
			table->set_course_changed();
		}
		table->setEditTriggers(QAbstractItemView::NoEditTriggers);
		is_allowed_edited = false;
	}
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
		table->reset();
		keep_sort_measure();
		table->setEditTriggers(QAbstractItemView::NoEditTriggers);
		is_allowed_edited = false;
	}
}


//导出当前表格内容到CSV文件，弹出文件保存对话框让用户选择保存位置和文件名，且导出后排序与用户当前看到的表格内容一致
void MainWindow::On_exportmenu_triggered()
{
	CSV_Helper* csv = new CSV_Helper(table->get_current_studentlist());
	QString newFilePath;
	while(true)
	{
		newFilePath = QFileDialog::getSaveFileName(
			this,
			"导出为CSV",
			QDir::currentPath() + "/CSV/students.csv", // 默认路径+文件名
			"CSV文件 (*.csv)"
		);
		if (newFilePath.isEmpty())
			return;
		QFileInfo info(newFilePath);
		if (info.exists() && info.isFile()) {
			// 已存在：提示并重新循环选择
			QMessageBox::warning(
				this, "路径已存在",
				"文件已存在，请输入其他文件名或选择其他路径！"
			);
			continue;
		}
		csv->export_to_csv(newFilePath.toStdString());
		break;
	}
	delete csv;
}

//返回父窗口并关闭当前窗口
void MainWindow::On_returnmenu_triggered()
{
	if(isTeacher==false)
	{
		if (ParentLoginWidget) {
			ParentLoginWidget->show();
			ParentLoginWidget->raise();
			ParentLoginWidget->activateWindow();
		}
	}
	else
	{
		if (ParentWidget) {
			ParentWidget->show();
			ParentWidget->raise();
			ParentWidget->activateWindow();
		}
	}
	table->closing();
	close();
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
