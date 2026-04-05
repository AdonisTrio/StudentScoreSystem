#pragma once
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QSqlError>
#include<QMessageBox>
#include"Student.h"

class DatabaseHelper
{
	QSqlDatabase db;
public:
	DatabaseHelper();

	bool isOpen() { return db.isOpen(); }

	vector<string> split(const string& s, char delimiter);

	void OpenDatabase( QString );

	void CreateTableStudents();
	//创建学生表，表中包含姓名、院系、学号、GPA、平均学分成绩、总学分六个字段

	void CreateTableCourses(Student);
	//为每个学生创建一个课程成绩表，表名为学生学号，表中包含课程名称、课程得分、课程学分、课程绩点四个字段

	void getCourseScores(Student&);
	//从数据库中获取学生的课程成绩，并将其存储在学生对象的Courses成员变量的vector容器中

	vector<Student> get_All_Students();
	//从数据库中获取所有学生的信息，并将其存储在一个学生对象的vector容器中返回

	void FillTableCourses(Student);
	//将学生对象中的课程成绩数据填充到数据库中对应学生的课程成绩表中

	void FillTableStudents(vector<Student>);
	//将学生对象的vector中的数据填充到数据库的Students表中

	void update_Student(vector<Student>);

	void addStudent(Student&);

	void deleteStudent(int id);

	void creatTeacherTable();

	bool verifyTeacher(const QString& username, const QString& password);
};


