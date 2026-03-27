#include "Table.h"

Table::Table(QWidget* parent) 
{
	
}

void Table::OpenLocalDatabase(QString path)
{
	DB = new DatabaseHelper;
	DB->OpenLocalDatabase(path);
	if (!isLocalDatabaseEmpty())
	{
		vector<Student> students = DB->get_All_Students();
		class_to_table(students);
	}
}

bool Table::isLocalDatabaseEmpty()
{
	vector<Student> students = DB->get_All_Students();
	if (students.size() == 0)
		return true;
	return false;
}

void Table::class_to_table(vector<Student> students)
{
	//设置表格不可修改，按下菜单栏的编辑按钮后再设置为可修改
	this->setEditTriggers(QAbstractItemView::NoEditTriggers);

	vector<CourseScore> courses = students[0].getCourses();
	
	//设置行列数
	int courseCount = courses.size();
	int rowCount = students.size()+2;
	this->setRowCount(rowCount);
	int columnCount = 7 + 2 * courseCount;
	this->setColumnCount(columnCount);
	
	//设置表头
	this->setItem(0, 0, StrItem("排名"));
	this->setSpan(0, 0, 2, 1);
	this->setItem(0, 1, StrItem("学号"));
	this->setSpan(0, 1, 2, 1);
	this->setItem(0, 2, StrItem("姓名"));
	this->setSpan(0, 2, 2, 1);
	this->setItem(0, 3, StrItem("院系"));
	this->setSpan(0, 3, 2, 1);
	this->setItem(0, columnCount - 3, StrItem("GPA"));
	this->setSpan(0, columnCount - 3, 2, 1);
	this->setItem(0, columnCount - 2, StrItem("平均学分成绩"));
	this->setSpan(0, columnCount - 2, 2, 1);
	this->setItem(0, columnCount - 1, StrItem("总学分"));
	this->setSpan(0, columnCount - 1, 2, 1);

	for (int i = 0; i < courseCount; i++)
	{
		string s = courses[i].getCourseName() + "  " + courses[i].doubleToString(courses[i].getCredit(),1) + "学分";
		this->setItem(0, 4 + 2 * i, StrItem(s));
		this->setSpan(0, 4 + 2 * i, 1, 2);
		this->setItem(1, 4 + 2 * i, StrItem("得分"));
		this->setItem(1, 4 + 2 * i + 1, StrItem("绩点"));
	}
	vector<Student>::iterator it = students.begin();
	int row = 2;
	while (it != students.end())
	{
		this->setItem(row, 0, NumItem(row-1));
		this->setItem(row, 1, NumItem(it->getId()));
		this->setItem(row, 2, StrItem(it->getName()));
		this->setItem(row, 3, StrItem(it->getDepartment()));
		this->setItem(row, columnCount - 3, NumItem(it->getGPA()));
		this->setItem(row, columnCount - 2, NumItem(it->getAverageScore()));
		this->setItem(row, columnCount - 1, NumItem(it->getTotalCredit()));
		vector<CourseScore>::iterator course = it->getCourses().begin();
		int column = 4;
		while (course != it->getCourses().end())
		{
			this->setItem(row, column, NumItem(course->getScore()));
			this->setItem(row, column + 1, NumItem(course->getCreditPoint()));
			course++;
			column += 2;
		}
		it++;
		row++;
	}
}


QTableWidgetItem* Table::StrItem(string x)
{
	qtwi* item = new qtwi(QString::fromStdString(x));
	item->setTextAlignment(Qt::AlignCenter);
	return item;
}

template<typename T>
inline QTableWidgetItem* Table::NumItem(T x)
{
	qtwi* item = new qtwi(QString::number(x));
	item->setTextAlignment(Qt::AlignCenter);
	return item;
}
