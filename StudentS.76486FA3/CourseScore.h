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
	CourseScore(string name = " ", double s = 0.0, double c = 0.0, double cp = 0.0);

	//Getters:
	string getCourseName() { return CName; }
	double getScore() { return score; }
	double getCredit() { return credit; }
	double getCreditPoint() { return CreditPoint; }

	//Setters:
	void setCourseName(string name) { CName = name; }
	void setScore(double s);
	/*· 输入参数：s – 得分
· 输出参数：无
· 功能：设置课程得分，范围0~100
· 算法：若超出范围则弹出警告并返回*/
	void setCredit(double c);

	void evaluateCreditPoint();        //根据得分计算绩点
	/*· 输入参数：无
· 输出参数：无
· 功能：根据得分计算绩点
· 算法：分段线性映射：[90,100]→4.0，[85,90)→3.7，[82,85)→3.3，[78,82)→3.0，[75,78)→2.7，[72,75)→2.3，[68,72)→2.0，[64,68)→1.5，[60,64)→1.0，<60→0.0*/
	string doubleToString(double num, int precision = 4);
};