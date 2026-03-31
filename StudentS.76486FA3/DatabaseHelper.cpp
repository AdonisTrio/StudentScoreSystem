#include "DatabaseHelper.h"

DatabaseHelper::DatabaseHelper()
{

}


vector<string> DatabaseHelper::split(const string& s, char delimiter)
{
	vector<string> tokens;
	string token;
	istringstream tokenStream(s);
	while (getline(tokenStream, token, delimiter))
		tokens.push_back(token);
	return tokens;
}

void DatabaseHelper::OpenLocalDatabase(QString path)
{
	db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName(path);
	db.open();
}

void DatabaseHelper::CreateTableStudents()
{
	const char* SQL = "CREATE TABLE IF NOT EXISTS Students ( 姓名 TEXT, 院系 TEXT,学号 INTEGER PRIMARY KEY, GPA REAL, 平均学分成绩 REAL, 总学分 REAL)";
	QSqlQuery q;
	if (!q.exec(SQL))
		QMessageBox::critical(nullptr, " ", "创建学生表失败！\n" + q.lastError().text());
}

void DatabaseHelper::CreateTableCourses(Student LiHua)
{
	string  name = LiHua.getName();
	string SQL = "CREATE TABLE IF NOT EXISTS " + name + " ( 课程名称 TEXT PRIMARY KEY, 课程得分 REAL, 课程学分 REAL, 课程绩点 REAL)";
	QSqlQuery q;
	if (!q.exec(SQL.c_str()))
		QMessageBox::critical(nullptr, " ", "创建课程成绩表失败！\n" + q.lastError().text());
}

void DatabaseHelper::getCourseScores( Student& LiHua)
{
	string  name = LiHua.getName();
	string SQL = "SELECT * FROM " + name;
	QSqlQuery q;
	q.exec(SQL.c_str());
	while (q.next())
	{
		CourseScore course(
			q.value(0).toString().toStdString(),
			q.value(1).toDouble(),
			q.value(2).toDouble(),
			q.value(3).toDouble()
		);
		course.evaluateCreditPoint();
		LiHua.updateCourses(course);
	}
}

vector<Student> DatabaseHelper::get_All_Students()
{
	vector<Student> students;
	const char* SQL = "SELECT * FROM Students ORDER BY 平均学分成绩 DESC, 学号 ASC ";
	QSqlQuery q;
	q.exec(SQL);
	while (q.next())
	{
		Student student(
			q.value(0).toString().toStdString(),
			q.value(1).toString().toStdString(),
			q.value(2).toInt()
		);
		getCourseScores(student);
		student.calculateTotalCredit();
		student.calculateGPA();
		student.calculateAverageScore();
		students.push_back(student);
	}
	return students;
}

void DatabaseHelper::FillTableCourses(Student LiHua)
{
	vector<CourseScore> courses = LiHua.getCourses();
	vector<CourseScore>::iterator course = courses.begin();
	string  name = LiHua.getName();
	while (course != courses.end())
	{
		string SQL = "INSERT INTO " + name + " (课程名称, 课程得分, 课程学分, 课程绩点) VALUES ('" +
			course->getCourseName() + "', ? , ? , ?)";
		QSqlQuery q;
		q.prepare(SQL.c_str());
		q.bindValue(0, course->getScore());
		q.bindValue(1, course->getCredit());
		q.bindValue(2, course->getCreditPoint());
		q.exec();
		course++;
	}
}

void DatabaseHelper::FillTableStudents(vector<Student> students)
{
	if (students.size() == 0)
		return;
	vector<Student>::iterator student = students.begin();
	while (student != students.end())
	{
		string SQL = "INSERT INTO Students (姓名, 院系, 学号, GPA, 平均学分成绩, 总学分) VALUES ('" +
			student->getName() + "', '" +
			student->getDepartment() + "', ? , ? ,? , ?)";
		QSqlQuery q;
		q.prepare(SQL.c_str());
		q.bindValue(0, student->getId());
		q.bindValue(1, student->getGPA());
		q.bindValue(2, student->getAverageScore());
		q.bindValue(3, student->getTotalCredit());
		q.exec();
		CreateTableCourses(*student);
		FillTableCourses(*student);
		student++;
	}
}

void DatabaseHelper::update_Student(vector<Student> students)
{
	if(students.size() == 0)
		return;
	for (auto x : students)
	{
		string SQL1 = "UPDATE Students SET 姓名 = ?, 院系 = ? , GPA = ? , 平均学分成绩 = ? , 总学分 = ? WHERE 学号 = ?";
		QSqlQuery q;
		q.prepare(SQL1.c_str());
		q.bindValue(0, x.getName().c_str());
		q.bindValue(1, x.getDepartment().c_str());
		q.bindValue(2, x.getGPA());
		q.bindValue(3, x.getAverageScore());
		q.bindValue(4, x.getTotalCredit());
		q.bindValue(5, x.getId());
		q.exec();
		
		string SQL2 = "DELETE FROM " + x.getName();
		q.exec(SQL2.c_str());
		FillTableCourses(x);
	}
}

