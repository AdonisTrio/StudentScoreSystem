#pragma once
#include"Student.h"

class ScoreManager
{
	vector<Student> students;
	vector<Student> pre_filteredStudents;
	vector<Student> filteredStudents;  // 过滤后的学生
	bool filtering;
	bool wholeFilter; // 是否已经全局过滤（如按全部及格等）
	//如果已经全局过滤，则再次按其他条件过滤时，应该在pre_filteredStudents上进行，而不是在filteredStudents上进行


public:
	ScoreManager(vector<Student> students) :students(students), pre_filteredStudents(students), filtering(false), wholeFilter(false){}

	vector<Student>& getStudents() 
	{ 
		if (filtering) return filteredStudents;
		else return students;
	}

	void clearFilter() { filtering = false; wholeFilter = false; filteredStudents.clear(); pre_filteredStudents = students;}

	void filter_by_name(const string& name);
	/*· 输入参数：name – 姓名关键字（子串）
· 输出参数：无
· 功能：按姓名筛选（包含子串）
· 算法：若filtering为真，则遍历filteredStudents，否则遍历students；使用string::find匹配姓名中包含关键字的学生，保留到filteredStudents中*/
	void filter_by_dep(const string& dept);
	void filter_by_CourseScoreMin(const string& courseName, double minScore); // 课程成绩>=某值
	void filter_by_CourseScoreMax(const string& courseName, double maxScore);
	void filter_by_excel();
	void filter_by_middle();
	void filter_by_Pass();
	void filter_by_Fail();

	void default_Sort();
	void Sort_by_id();
	void Sort_by_course(int);
	/*· 输入参数：n – 课程索引（第n门课程）
· 输出参数：无
· 功能：按第n门课程成绩降序排序
· 算法：使用std::sort，比较条件为A[n].getScore() > B[n].getScore()，若成绩相同则按学号升序*/
};