#pragma once
#include"Student.h"

class ScoreManager
{
	vector<Student> students;
	vector<Student> filteredStudents;  // 过滤后的学生
	bool filtering;
public:
	ScoreManager(vector<Student> students) :students(students), filtering(false) {}

	vector<Student>& getStudents() 
	{ 
		if (filtering) return filteredStudents;
		else return students;
	}

	void clearFilter() { filtering = false; }

	void filter_by_id(int id);
	void filter_by_name(const string& name);
	void filter_by_dep(const string& dept);
	void filter_by_CourseScoreMin(const string& courseName, double minScore); // 课程成绩>=某值
	void filter_by_CourseScoreMax(const string& courseName, double maxScore);

	void default_Sort();
	void Sort_by_id();
	void Sort_by_course(int);
};