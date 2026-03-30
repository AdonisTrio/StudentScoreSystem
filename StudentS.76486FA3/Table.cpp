#include "Table.h"

Table::Table(QWidget* parent)
{
	DB = nullptr;
	manager = nullptr;
	isRefreshing = false;
}

void Table::OpenLocalDatabase(QString path)
{
	DB = new DatabaseHelper;
	DB->OpenLocalDatabase(path);
	if (!isLocalDatabaseEmpty())
		class_to_table(manager->getStudents());
	connect(this, &QTableWidget::itemChanged, this, &Table::On_cell_changed);

	//设置表格不可修改，按下菜单栏的编辑按钮后再设置为可修改
	this->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

bool Table::isLocalDatabaseEmpty()
{
	manager =  new ScoreManager(DB->get_All_Students());
	if (manager->getStudents().size() == 0)
		return true;
	return false;
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
		this->setItem(row, columnCount - 3, fixed_NumItem(it->getGPA()));
		this->setItem(row, columnCount - 2, fixed_NumItem(it->getAverageScore()));
		this->setItem(row, columnCount - 1, fixed_NumItem(it->getTotalCredit()));
		vector<CourseScore>::iterator course = it->getCourses().begin();
		int column = 4;
		while (course != it->getCourses().end())
		{
			this->setItem(row, column, NumItem(course->getScore()));
			this->setItem(row, column + 1, fixed_NumItem(course->getCreditPoint()));
			course++;
			column += 2;
		}
		it++;
		row++;
	}
	isRefreshing = false;
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

template<typename T>
inline QTableWidgetItem* Table::NumItem(T x)
{
	qtwi* item = new qtwi(QString::number(x));
	item->setTextAlignment(Qt::AlignCenter);
	return item;
}

template<typename T>
inline QTableWidgetItem* Table::fixed_NumItem(T x)
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
		switch (col)
		{
		case 2:
			s->setName(item->text().toStdString());
			break;
		case 3:
			s->setDepartment(item->text().toStdString());
			break;
		default:
			int i = (col - 4) / 2;
			s->updateCourseScore(i, item->text().toDouble());
		}
	}
	manager->default_Sort();
	class_to_table(manager->getStudents());
	clearSelection();
}