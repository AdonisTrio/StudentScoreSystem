#include "Table.h"

#include <QInputDialog>

Table::Table(QWidget* parent)
{
	DB = nullptr;
	manager = nullptr;
	isRefreshing = false;
	
	//设置表格不可修改，按下菜单栏的编辑按钮后再设置为可修改
	setEditTriggers(QAbstractItemView::NoEditTriggers);
}


void Table::class_to_table(vector<Student> students)
{
	isRefreshing = true;

	vector<CourseScore> courses = students[0].getCourses();
	
	//设置行列数
	int courseCount = courses.size();
	int rowCount = students.size()+2;
	this->setRowCount(rowCount);
	int columnCount = 7 + 2 * courseCount;
	this->setColumnCount(columnCount);
	
	//设置表头
	this->setItem(0, 0, fixed_StrItem("排名"));
	this->setSpan(0, 0, 2, 1);
	this->setItem(0, 1, fixed_StrItem("学号"));
	this->setSpan(0, 1, 2, 1);
	this->setItem(0, 2, fixed_StrItem("姓名"));
	this->setSpan(0, 2, 2, 1);
	this->setItem(0, 3, fixed_StrItem("院系"));
	this->setSpan(0, 3, 2, 1);
	this->setItem(0, columnCount - 3, fixed_StrItem("GPA"));
	this->setSpan(0, columnCount - 3, 2, 1);
	this->setItem(0, columnCount - 2, fixed_StrItem("平均学分成绩"));
	this->setSpan(0, columnCount - 2, 2, 1);
	this->setItem(0, columnCount - 1, fixed_StrItem("总学分"));
	this->setSpan(0, columnCount - 1, 2, 1);

	for (int i = 0; i < courseCount; i++)
	{
		string s = courses[i].getCourseName() + ' ' + courses[i].doubleToString(courses[i].getCredit(), 1) + "学分";
		this->setItem(0, 4 + 2 * i, StrItem(s));
		this->setSpan(0, 4 + 2 * i, 1, 2);
		this->setItem(1, 4 + 2 * i, fixed_StrItem("得分"));
		this->setItem(1, 4 + 2 * i + 1, fixed_StrItem("绩点"));
	}
	vector<Student>::iterator it = students.begin();
	int row = 2;
	while (it != students.end())
	{
		this->setItem(row, 0, fixed_NumItem(row-1));
		this->setItem(row, 1, fixed_NumItem(it->getId()));
		this->setItem(row, 2, StrItem(it->getName()));
		this->setItem(row, 3, StrItem(it->getDepartment()));
		this->setItem(row, columnCount - 3, fixed_StrItem(it->doubleToString(it->getGPA(),1)));
		this->setItem(row, columnCount - 2, fixed_StrItem(it->doubleToString(it->getAverageScore(),1)));
		this->setItem(row, columnCount - 1, fixed_StrItem(it->doubleToString(it->getTotalCredit(),1)));
		vector<CourseScore>::iterator course = it->getCourses().begin();
		int column = 4;
		while (course != it->getCourses().end())
		{
			this->setItem(row, column, StrItem(course->doubleToString(course->getScore(),1)));
			this->setItem(row, column + 1, fixed_StrItem(course->doubleToString(course->getCreditPoint(), 1)));
			course++;
			column += 2;
		}
		it++;
		row++;
	}
	isRefreshing = false;
}

//导入csv时初始化表格
void Table::initialize_table()
{
	manager = new ScoreManager(DB->get_All_Students());
	manager->default_Sort();
	class_to_table(manager->getStudents());
	connect(this, &QTableWidget::itemChanged, this, &Table::On_cell_changed);
}

QTableWidgetItem* Table::StrItem(string x)
{
	qtwi* item = new qtwi(QString::fromStdString(x));
	item->setTextAlignment(Qt::AlignCenter);
	return item;
}

QTableWidgetItem* Table::fixed_StrItem(string x)
{
	qtwi* item = new qtwi(QString::fromStdString(x));
	item->setTextAlignment(Qt::AlignCenter);
	item->setFlags(item->flags() & ~Qt::ItemIsEditable);
	return item;
}


QTableWidgetItem* Table::fixed_NumItem(int x)
{
	qtwi* item = new qtwi(QString::number(x));
	item->setTextAlignment(Qt::AlignCenter);
	item->setFlags(item->flags() & ~Qt::ItemIsEditable);
	return item;
}

void Table::On_cell_changed(QTableWidgetItem* item)
{
	if (isRefreshing)
		return;
	try
	{
		if (item->text().trimmed().isEmpty())
			throw("内容不能为空或只有空格！");

		QRegularExpression re("^[\\p{Han}\\p{L}\\s0-9 .]+$");
		re.setPatternOptions(QRegularExpression::UseUnicodePropertiesOption);
		if (!re.match(item->text()).hasMatch())
			throw("只能输入汉字、英文字母、数字和空格！");
	}catch(const char* msg)
	{
		QMessageBox::warning(nullptr, "输入错误", msg);
		return;
	}

	int row = item->row();
	int col = item->column();
	if (row == 0)
	{
		int i = (col - 4) / 2;
		vector<string> s = DB->split(item->text().toStdString(), ' ');
		if (s.size() < 2) 
		{
			QMessageBox::warning(nullptr, "输入错误", "请按「课程名 学分」格式输入");
			return;
		}
		for(auto& x: manager->getStudents())
		{
			x.getCourses()[i].setCourseName(s[0]);
			x.updateCourseCredit(i, stod(s[1]));
		}
	}
	else 
	{
		Student *s = &manager->getStudents()[row - 2];
		QRegularExpression re1("^[\u4e00-\u9fa5a-zA-Z ]+$");
		QRegularExpression re2("^\\d+(\\.\\d+)?$");
		string string0;
		try
		{
			switch (col)
			{
			case 2:
				string0 = s->getName();
				if (!re1.match(item->text()).hasMatch())
					throw("姓名只能为汉字、英文字母和空格！");
				s->setName(item->text().toStdString());
				break;
			case 3:
				string0 = s->getDepartment();
				if (!re1.match(item->text()).hasMatch())
					throw("院系只能为汉字、英文字母和空格！");
				s->setDepartment(item->text().toStdString());
				break;
			default:
				int i = (col - 4) / 2;
				string0 = s->getCourses()[i].doubleToString(s->getCourses()[i].getScore(), 1);
				if (!re2.match(item->text()).hasMatch())
					throw( "课程成绩只能为数字！");
				bool ok;
				double score = item->text().toDouble(&ok);
				if (!ok)
					throw("请输入有效的数字！");
				s->updateCourseScore(i, item->text().toDouble());
				string0 = s->getCourses()[i].doubleToString(s->getCourses()[i].getScore(),1);
				item->setText(QString::fromStdString(string0));
			}
		}
		catch(const char* msg)
		{
			QMessageBox::warning(nullptr, "输入错误", msg);
			item->setText(QString::fromStdString(string0));
			return;
		}
	}
}

void Table::addEmptyStudent()
{
	vector<Student> students = manager->getStudents(); // 检查现有学生,以便获取课程模板
	if (students.empty()) 
	{
		QMessageBox::warning(nullptr, "", "当前没有学生，无法添加！请先导入数据。");
		return;
	}

	/*
	1                // 默认显示的数字
    1                // 允许的最小值
    9999999           // 允许的最大值
    1                // 步长（点击上下箭头时每次增减的值）
	*/
	bool ok;
	int newId = QInputDialog::getInt(nullptr, "", "请输入学号:", 1, 1, 9999999, 1, &ok); // 弹出对话框输入学号
	if (!ok) return;

	for (int i = 0; i < students.size(); i++)
	{
		if (students[i].getId() == newId)
		{
			QMessageBox::warning(nullptr, "", "学号已存在！");
			return;
		}
	}

	vector<CourseScore> Courses = students[0].getCourses();

	Student newStudent("","", newId);
	for (auto& c : Courses) 
	{
		CourseScore cs(c.getCourseName(), 0.0, c.getCredit(), 0.0);
		newStudent.updateCourses(cs);
	}
	newStudent.calculateTotalCredit();
	newStudent.calculateAverageScore();
	newStudent.calculateGPA();

	DB->addStudent(newStudent);

	reset();
	default_sort();
	class_to_table(manager->getStudents());

	vector<Student> newList = manager->getStudents();

	int targetRow;
	for (int i = 0; i < (int)newList.size(); i++) {
		if (newList[i].getId() == newId) {
			// 表格前两行是表头，数据行从第2行开始
			targetRow = i + 2;
			break;
		}
	}

	if (targetRow >= 2) {
		// 获取该行第2列（学号列）的单元格，用来定位
		QTableWidgetItem* cell = this->item(targetRow, 1);
		if (cell != nullptr) {
			this->scrollToItem(cell, QAbstractItemView::PositionAtBottom); // 滚动到底部
			this->selectRow(targetRow); // 高亮整行
		}
	}
}

void Table::deleteSelectedStudent()
{
	
	/*
	获取当前表格中所有被选中的单元格（QTableWidgetItem 对象指针）
	QList 是 Qt 提供的动态数组，类似于 C++ 标准库的 vector，用于存储多个元素
	QTableWidgetItem* 是指向 QTableWidgetItem 对象的指针，每个这样的对象代表表格中的一个单元格
	selectedItems() 是 QTableWidget 的成员函数，返回当前被选中的单元格指针列表
	如果没有任何单元格被选中，返回的列表为空
	*/
	QList<QTableWidgetItem*> selected = this->selectedItems();

	if (selected.isEmpty()) 
	{
		QMessageBox::warning(nullptr, "", "请先选中要删除的学生行！");
		return;
	}

	int row = selected.first()->row();
	if (row < 2) 
	{   // 前两行是表头
		QMessageBox::warning(nullptr, "", "不能删除表头！");
		return;
	}

	// 获取该行对应的学生对象
	vector<Student> students = manager->getStudents();
	Student& stu = students[row - 2];
	int id = stu.getId();
	QString name = QString::fromStdString(stu.getName());

	if (students.size() == 1) 
	{
		QMessageBox::warning(nullptr, "", "至少需要保留一个学生！");
		return;
	}

	int ret = QMessageBox::question(nullptr, "",
		QString("确定要删除学生 %1（学号 %2）吗？").arg(name).arg(id),
		QMessageBox::Yes | QMessageBox::No);
	if (ret != QMessageBox::Yes) return;

	DB->deleteStudent(id);

	reset();
	default_sort();
	class_to_table(manager->getStudents());
}
