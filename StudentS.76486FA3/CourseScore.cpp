#include<iostream>
#include"CourseScore.h"

CourseScore::CourseScore(string name, double s, double c, double cp) : CName(name), score(s), credit(c), CreditPoint(cp)
{
	try {
		if (s > 100 || s < 0 || c < 0 || cp < 0 || cp > 4)
			throw "输入的课程得分必须在0到100之间，课程学分不能为负数，课程绩点必须在0到4之间";
	}
	catch (const char* msg) {
		QMessageBox::critical(nullptr, " ", msg);
	}
}

//Setters:
void CourseScore::setScore(double s) 
{
	try {
		score = s;
		if (s > 100 || s < 0)
			throw "课程得分必须在0到100之间";
	}
	catch (const char* msg) {
		QMessageBox::critical(nullptr, " ", msg);
	}
}

void CourseScore::setCredit(double c) 
{
	try {
		credit = c;
		if (c < 0)
			throw "课程学分不能为负数";
	}
	catch (const char* msg) {
		QMessageBox::critical(nullptr, " ", msg);
	}
}

void CourseScore::evaluateCreditPoint()
{
	if (score >= 90)
		CreditPoint = 4.0;
	else if (score >= 85)
		CreditPoint = 3.7;
	else if (score >= 82)
		CreditPoint = 3.3;
	else if (score >= 78)
		CreditPoint = 3.0;
	else if (score >= 75)
		CreditPoint = 2.7;
	else if (score >= 72)
		CreditPoint = 2.3;
	else if (score >= 68)
		CreditPoint = 2.0;
	else if (score >= 64)
		CreditPoint = 1.5;
	else if (score >= 60)
		CreditPoint = 1.0;
	else
		CreditPoint = 0.0;
}

string CourseScore::doubleToString(double num, int precision)
{
	stringstream ss;
	ss << fixed << setprecision(precision) << num;
	return ss.str();
}
