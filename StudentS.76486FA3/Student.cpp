#include<iostream>
#include"Student.h"

Student::Student(string name, string dept, int studentId) : SName(name), department(dept), id(studentId)
{
	GPA = 0.0;
	AverageScore = 0.0;
	totalCredit = 0.0;
}


//Setters:
void Student::setGPA(double gpa) 
{ try{
	GPA = gpa;
	if (gpa > 4 || gpa < 0)
		throw "GPA 必须在0到4之间";
}
catch (const char* msg) {
	QMessageBox::critical(nullptr, " ", msg);
	}
}

void Student::setAverageScore(double as) 
{
	try {
		AverageScore = as;
		if (as > 100 || as < 0)
			throw "平均学分成绩必须在0到100之间";
	}
	catch (const char* msg) {
		QMessageBox::critical(nullptr, " ", msg);
	}
}

void Student::setTotalCredit(double tc) 
{ 
	try {
		totalCredit = tc;
		if (tc < 0)
			throw "总学分不能为负数";
	}
	catch (const char* msg) {
		QMessageBox::critical(nullptr, " ", msg);
	}
}




void Student::calculateTotalCredit()
{
	totalCredit = 0.0;
	vector<CourseScore>::iterator pd = Courses.begin();
	while (pd != Courses.end())
	{
		if(pd->score>=60)
			totalCredit += pd->credit;
		pd++;
	}
}

void Student::calculateAverageScore()
{
	double totalScore = 0.0;
	vector<CourseScore>::iterator pd = Courses.begin();
	while(pd != Courses.end())
	{
		if (pd->score >= 60)
			totalScore += ( pd->score * pd->credit);
		pd++;
	}
	AverageScore = totalScore / totalCredit;
}

void Student::calculateGPA()
{
	double totalCreditPoint = 0.0;
	vector<CourseScore>::iterator pd = Courses.begin();
	while (pd != Courses.end())
	{
		totalCreditPoint += (pd->CreditPoint * pd->credit);
		pd++;
	}
	GPA = totalCreditPoint / totalCredit;
}