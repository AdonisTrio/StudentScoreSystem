#include "DatabaseHelper.h"

DatabaseHelper::DatabaseHelper()
{

}


vector<string> DatabaseHelper::split(const string& s, char delimiter)
{
	vector<string> tokens;
	string token;
	istringstream tokenStream(s);
	//getline函数从tokenStream中读取数据，直到遇到delimiter为止，并将读取到的数据存储在token中
	//下一次调用getline时会继续从tokenStream中读取数据，直到再次遇到delimiter为止
	//getline的返回值是tokenStream的引用，转换为bool类型时会调用tokenStream的operator bool()，当tokenStream处于有效状态时返回true，否则返回false
	while (getline(tokenStream, token, delimiter))	
		tokens.push_back(token);
	return tokens;
}

void DatabaseHelper::OpenDatabase(QString path)
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
	string  id = to_string(LiHua.getId());	
	string SQL = "CREATE TABLE IF NOT EXISTS \"" + id + "\" ( 课程名称 TEXT PRIMARY KEY, 课程得分 REAL, 课程学分 REAL, 课程绩点 REAL)";
	QSqlQuery q;
	if (!q.exec(SQL.c_str()))
		QMessageBox::critical(nullptr, " ", "创建课程成绩表失败！\n" + q.lastError().text());
}

void DatabaseHelper::getCourseScores( Student& LiHua)
{
	string  id = to_string(LiHua.getId());
	string SQL = "SELECT * FROM \"" + id + "\"";
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
	const char* SQL = "SELECT * FROM Students";
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
	string  id = to_string(LiHua.getId());
	while (course != courses.end())
	{
		string SQL = "INSERT INTO \"" + id + "\" (课程名称, 课程得分, 课程学分, 课程绩点) VALUES ('" +
			course->getCourseName() + "', ? , ? , ?)";
		QSqlQuery q;
		q.prepare(SQL.c_str());
		q.bindValue(0, course->getScore());
		q.bindValue(1, course->getCredit());
		q.bindValue(2, course->getCreditPoint());
		if (!q.exec()) {
			QMessageBox::critical(nullptr, "保存错误", "更新课程失败：\n" + q.lastError().text());
			return;
		}
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
		if (!q.exec()) {
			QMessageBox::critical(nullptr, "保存错误", "更新学生信息失败：\n" + q.lastError().text());
			return; 
		}
		
		string  id = to_string(x.getId());
		string SQL2 = "DELETE FROM \"" + id + "\""; 
		//delete仅删除表中的数据，不删除表结构，因此可以直接调用FillTableCourses函数将更新后的课程成绩数据填充到数据库中对应学生的课程成绩表中
		
		QSqlQuery q1;
		if (!q1.exec(SQL2.c_str())) {
			QMessageBox::critical(nullptr, "保存错误", "清空课程成绩失败：\n" + q.lastError().text());
			return;
		}
		FillTableCourses(x);
	}
}

