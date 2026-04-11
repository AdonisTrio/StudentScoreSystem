#pragma once
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QSqlError>
#include<QFileDialog>
#include<QMessageBox>
#include <QCryptographicHash>
#include"Student.h"

class DatabaseHelper
{
	QSqlDatabase db;
public:
	DatabaseHelper();

	bool isOpen() { return db.isOpen(); }

	vector<string> split(const string& s, char delimiter);
	//将字符串s按照指定的分隔符delimiter进行分割，并将分割后的子字符串存储在一个vector<string>容器中返回

	void OpenDatabase( QString );

	void CreateTableStudents();
	//创建学生表，表中包含姓名、院系、学号、GPA、平均学分成绩、总学分六个字段

	void CreateTableCourses(Student);
	//为每个学生创建一个课程成绩表，表名为学生学号，表中包含课程名称、课程得分、课程学分、课程绩点四个字段

	void getCourseScores(Student&);
	//从数据库中获取学生的课程成绩，并将其存储在学生对象的Courses成员变量的vector容器中

	vector<Student> get_All_Students();
	//从数据库中获取所有学生的信息，并将其存储在一个学生对象的vector容器中返回
	/*· 输入参数：无
· 输出参数：vector<Student> – 所有学生对象的列表
· 功能：从数据库读取所有学生及其成绩
· 算法：执行“SELECT * FROM Students”，对每条记录创建Student对象，调用getCourseScores填充课程列表，再依次调用calculateTotalCredit、calculateAverageScore、calculateGPA计算汇总指标*/
	
	void FillTableCourses(Student);
	//将学生对象中的课程成绩数据填充到数据库中对应学生的课程成绩表中

	void FillTableStudents(vector<Student>);
	//将学生对象的vector中的数据填充到数据库的Students表中

	void update_Student(vector<Student>);
	//根据学生对象的vector中的数据更新数据库中对应学生的记录和课程成绩表
	/*· 输入参数：students – 需要更新的学生列表
· 输出参数：无
· 功能：更新学生信息及课程成绩
· 算法：对每个学生：执行UPDATE语句更新Students表；删除该学生原有的课程成绩表记录；再调用FillTableCourses重新插入当前课程数据*/
	void addStudent(Student&);
	//向数据库中添加一个学生记录和对应的课程成绩表

	void deleteStudent(int id);
	//根据学生学号删除学生记录和对应的课程成绩表

	QString getSha256Hash(const QString& password);
	//使用SHA-256算法对密码进行哈希处理，返回哈希值的十六进制字符串表示

	bool verifyTeacher(const QString& username, const QString& password);
	//验证教师登录信息，返回true表示验证成功，false表示验证失败
	/*· 输入参数：username – 用户名，password – 明文密码
· 输出参数：bool – 验证成功返回true，否则false
· 功能：验证教师登录信息
· 算法：打开教师数据库，查询username对应的password哈希值，与输入密码的SHA-256哈希值比对*/

};


