#pragma once
#include"Student.h"

class ScoreManager
{
	vector<Student> students;
public:
	ScoreManager(vector<Student> students) :students(students) {}

	vector<Student>& getStudents() { return students; }

	void default_Sort();
	void Sort_by_course(int);
};