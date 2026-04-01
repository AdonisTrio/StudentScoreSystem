#pragma once
#include"CourseScore.h"
#include<vector>
#include<algorithm>

class Student
{
	string SName;                  //学生姓名
	string department;             //学生院系
	int id;						   //学生学号

	vector<CourseScore> Courses;   
	//学生的课程成绩列表(注：假设不同数据库中学生所选课程可能不同，而同一数据库中各学生所选课程相同）

	double GPA;					   //学生的平均绩点
	double AverageScore;           //学生的平均学分成绩
	double totalCredit;			   //学生的总学分
public:
	Student(string name = "", string dept = "", int studentId = 0);

	//Getters:
	string getName() { return SName; }
	string getDepartment() { return department; }
	int getId() { return id; }
	int getCourseCount() { return Courses.size(); }
	vector<CourseScore>& getCourses() { return Courses; }
	double getGPA() { return GPA; }
	double getAverageScore() { return AverageScore; }
	double getTotalCredit() { return totalCredit; }
	
	//Setters:
	void setName(string name) { SName = name; }
	void setDepartment(string dept) { department = dept; }
	void updateCourses(CourseScore course) { Courses.push_back(course); }
	void updateCourseScore(int i, double x);
	void updateCourseCredit(int i, double x);
	void setGPA(double);
	void setAverageScore(double);
	void setTotalCredit(double);


	//Calculaters:
	string doubleToString(double num, int precision = 4) { return Courses[0].doubleToString(num, precision); }
	void calculateTotalCredit();    //根据课程成绩计算总学分
	void calculateGPA();     //根据课程成绩计算GPA
	void calculateAverageScore();   //根据课程成绩计算平均分
};