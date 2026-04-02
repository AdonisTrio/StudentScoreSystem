#include<iostream>
#include"Student.h"
#include"CourseScore.h"

Student::Student(string name, string dept, int studentId) : SName(name), department(dept), id(studentId)
{
	GPA = 0.0;
	AverageScore = 0.0;
	totalCredit = 0.0;
}


void Student::updateCourseScore(int i, double x)
{
	Courses[i].setScore(x); 
	Courses[i].evaluateCreditPoint(); 
	calculateTotalCredit();
	calculateAverageScore();
	calculateGPA();
}

void Student::updateCourseCredit(int i, double x)
{
	Courses[i].setCredit(x); 
	calculateTotalCredit();
	calculateAverageScore();
	calculateGPA();
}

//Setters:
void Student::setGPA(double gpa) 
{ try{
	if (gpa > 4 || gpa < 0)
		throw "GPA 必须在0到4之间";
}
catch (const char* msg) {
	QMessageBox::critical(nullptr, " ", msg);
	return;
	}
GPA = gpa;
}

void Student::setAverageScore(double as) 
{
	try {
		if (as > 100 || as < 0)
			throw "平均学分成绩必须在0到100之间";
	}
	catch (const char* msg) {
		QMessageBox::critical(nullptr, " ", msg);
		return;
	}
	AverageScore = as;
}

void Student::setTotalCredit(double tc) 
{ 
	try {
		
		if (tc < 0)
			throw "总学分不能为负数";
	}
	catch (const char* msg) {
		QMessageBox::critical(nullptr, " ", msg);
		return;
	}
	totalCredit = tc;
}


void Student::calculateTotalCredit()
{
	totalCredit = 0.0;
	vector<CourseScore>::iterator pd = Courses.begin();
	while (pd != Courses.end())
	{
		if (pd->getScore() >= 60)
			totalCredit += pd->getCredit();
		++pd;
	}
}


void Student::calculateAverageScore()
{
	if(totalCredit == 0)
	{
		AverageScore = 0.0;
		return;
	}
	double totalScore = 0.0;
	vector<CourseScore>::iterator pd = Courses.begin();
	while(pd != Courses.end())
	{
		if (pd->getScore() >= 60)
			totalScore += ( pd->getScore() * pd->getCredit());
		pd++;
	}
	AverageScore = totalScore / totalCredit;
}

void Student::calculateGPA()
{
	if (totalCredit == 0)
	{
			GPA = 0.0;
		return;
	}
	double totalCreditPoint = 0.0;
	vector<CourseScore>::iterator pd = Courses.begin();
	while (pd != Courses.end())
	{
		totalCreditPoint += (pd->getCreditPoint() * pd->getCredit());
		pd++;
	}
	GPA = totalCreditPoint / totalCredit;
}