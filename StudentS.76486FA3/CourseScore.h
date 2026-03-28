#pragma once
#include <string>
#include<iomanip>
#include<sstream>
#include<QMessageBox>
using namespace std;

class CourseScore
{
	string CName;//课程名称
	double score;//课程得分
	double credit;//课程学分
	double CreditPoint;//课程绩点
public:
	CourseScore(string name = NULL, double s = 0.0, double c = 0.0, double cp = 0.0);

	//Getters:
	string getCourseName() { return CName; }
	double getScore() { return score; }
	double getCredit() { return credit; }
	double getCreditPoint() { return CreditPoint; }

	//Setters:
	void setCourseName(string name) { CName = name; }
	void setScore(double s);
	void setCredit(double c);

	void evaluateCreditPoint();           //根据得分计算绩点
	string doubleToString(double num, int precision = 4);

	friend class Student;
};