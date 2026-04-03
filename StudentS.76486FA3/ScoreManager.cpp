#include"ScoreManager.h"

void ScoreManager::default_Sort()
{
	sort(students.begin(), students.end(), [](Student a, Student b) 
		{
		if (a.getAverageScore() != b.getAverageScore())
			return a.getAverageScore() > b.getAverageScore();
		else
			return a.getId() < b.getId();
		});
}

void ScoreManager::Sort_by_id()
{
	sort(students.begin(), students.end(), [](Student a, Student b) 
		{
		return a.getId() < b.getId();
		});
}


void ScoreManager::Sort_by_course(int n)
{
	sort(students.begin(), students.end(), [n](Student a, Student b) 
		{

		vector<CourseScore> A = a.getCourses();
		vector<CourseScore> B = b.getCourses();
		if (A[n].getScore() != B[n].getScore())
			return A[n].getScore() > B[n].getScore();
		else
			return a.getId() < b.getId();
		});
}

void ScoreManager::filter_by_id(int id)
{
	filteredStudents.clear();
	for (int i = 0; i < students.size(); ++i) 
	{
		if (students[i].getId() == id) 
		{
			filteredStudents.push_back(students[i]);
			break;
		}
	}
	filtering = true;
}

void ScoreManager::filter_by_name(const string& name)
{
	filteredStudents.clear();
	for (int i = 0; i < students.size(); ++i) 
	{
		string n = students[i].getName();
		if (name == n) 
		{
			filteredStudents.push_back(students[i]);
		}
	}
	filtering = true;
}

void ScoreManager::filter_by_dep(const string& dept)
{
	filteredStudents.clear();
	for (int i = 0; i < students.size(); ++i) 
	{
		if (students[i].getDepartment() == dept) 
		{
			filteredStudents.push_back(students[i]);
		}
	}
	filtering = true;
}

void ScoreManager::filter_by_CourseScoreMin(const string& courseName, double minScore)
{
	filteredStudents.clear();
	for (int i = 0; i < students.size(); ++i) 
	{
		vector<CourseScore> courses = students[i].getCourses();
		for (int j = 0; j < courses.size(); ++j) 
		{
			if (courses[j].getCourseName() == courseName && courses[j].getScore() >= minScore) 
			{
				filteredStudents.push_back(students[i]);
				break;
			}
		}
	}
	filtering = true;
}

void ScoreManager::filter_by_CourseScoreMax(const string& courseName, double maxScore)
{
	filteredStudents.clear();
	for (int i = 0; i < students.size(); ++i) 
	{
		vector<CourseScore> courses = students[i].getCourses();
		for (int j = 0; j < courses.size(); ++j) 
		{
			if (courses[j].getCourseName() == courseName && courses[j].getScore() <= maxScore) 
			{
				filteredStudents.push_back(students[i]);
				break;
			}
		}
	}
	filtering = true;
}
