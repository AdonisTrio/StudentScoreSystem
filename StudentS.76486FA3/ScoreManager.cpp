#include"ScoreManager.h"

void ScoreManager::default_Sort()
{
	if (filtering)
	{
		sort(filteredStudents.begin(), filteredStudents.end(), [](Student a, Student b)
			{
				if (a.getAverageScore() != b.getAverageScore())
					return a.getAverageScore() > b.getAverageScore();
				else
					return a.getId() < b.getId();
			});
	}
	else
	{
		sort(students.begin(), students.end(), [](Student a, Student b)
			{
				if (a.getAverageScore() != b.getAverageScore())
					return a.getAverageScore() > b.getAverageScore();
				else
					return a.getId() < b.getId();
			});
	}
}

void ScoreManager::Sort_by_id()
{
	if (filtering)
	{
		sort(filteredStudents.begin(), filteredStudents.end(), [](Student a, Student b)
			{
				return a.getId() < b.getId();
			});
	}
	else
	{
		sort(students.begin(), students.end(), [](Student a, Student b)
			{
				return a.getId() < b.getId();
			});
	}
}


void ScoreManager::Sort_by_course(int n)
{
	if(filtering)
		{
		sort(filteredStudents.begin(), filteredStudents.end(), [n](Student a, Student b)
			{
				vector<CourseScore> A = a.getCourses();
				vector<CourseScore> B = b.getCourses();
				if (A[n].getScore() != B[n].getScore())
					return A[n].getScore() > B[n].getScore();
				else
					return a.getId() < b.getId();
			});
	}
	else
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
}


void ScoreManager::filter_by_name(const string& name)
{
	if (filtering)
	{
		vector<Student> temp;
		for (int i = 0; i < filteredStudents.size(); ++i)
		{
			string n = filteredStudents[i].getName();
			if (name == n)
			{
				temp.push_back(filteredStudents[i]);
			}
		}
		filteredStudents = temp;
	}
	else
	{
		for (int i = 0; i < students.size(); ++i)
		{
			string n = students[i].getName();
			if (name == n)
			{
				filteredStudents.push_back(students[i]);
			}
		}
	}
	filtering = true;
}

void ScoreManager::filter_by_dep(const string& dept)
{
	if(filtering)
	{
		vector<Student> temp;
		for (int i = 0; i < filteredStudents.size(); ++i)
		{
			if (filteredStudents[i].getDepartment() == dept)
			{
				temp.push_back(filteredStudents[i]);
			}
		}
		filteredStudents = temp;
	}
	else
	{
		for (int i = 0; i < students.size(); ++i)
		{
			if (students[i].getDepartment() == dept)
			{
				filteredStudents.push_back(students[i]);
			}
		}
	}
	filtering = true;
}

void ScoreManager::filter_by_CourseScoreMin(const string& courseName, double minScore)
{
	if(filtering)
	{
		vector<Student> temp;
		for (int i = 0; i < filteredStudents.size(); ++i)
		{
			vector<CourseScore> courses = filteredStudents[i].getCourses();
			for (int j = 0; j < courses.size(); ++j)
			{
				if (courses[j].getCourseName() == courseName && courses[j].getScore() >= minScore)
				{
					temp.push_back(filteredStudents[i]);
					break;
				}
			}
		}
		filteredStudents = temp;
	}
	else
	{
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
	}
	filtering = true;
}

void ScoreManager::filter_by_CourseScoreMax(const string& courseName, double maxScore)
{
	if(filtering)
	{
		vector<Student> temp;
		for (int i = 0; i < filteredStudents.size(); ++i)
		{
			vector<CourseScore> courses = filteredStudents[i].getCourses();
			for (int j = 0; j < courses.size(); ++j)
			{
				if (courses[j].getCourseName() == courseName && courses[j].getScore() <= maxScore)
				{
					temp.push_back(filteredStudents[i]);
					break;
				}
			}
		}
		filteredStudents = temp;
	}
	else
	{
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
	}
	filtering = true;
}
