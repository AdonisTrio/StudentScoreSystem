#include"ScoreManager.h"

void ScoreManager::default_Sort()
{
	vector<Student>* s;
	if (filtering)
		s = &filteredStudents;
	else
		s = &students;
	
	sort(s->begin(), s->end(), [](Student a, Student b)
		{
			if (a.getAverageScore() != b.getAverageScore())
				return a.getAverageScore() > b.getAverageScore();
			else
				return a.getId() < b.getId();
		});
	
}

void ScoreManager::Sort_by_id()
{
	vector<Student>* s;
	if (filtering)
		s = &filteredStudents;
	else
		s = &students;

	sort(s->begin(), s->end(), [](Student a, Student b)
		{
			return a.getId() < b.getId();
		});
	
}


void ScoreManager::Sort_by_course(int n)
{
	vector<Student>* s;
	if (filtering)
		s = &filteredStudents;
	else
		s = &students;

	sort(s->begin(), s->end(), [n](Student a, Student b)
		{
			vector<CourseScore> A = a.getCourses();
			vector<CourseScore> B = b.getCourses();
			if (A[n].getScore() != B[n].getScore())
				return A[n].getScore() > B[n].getScore();
			else
				return a.getId() < b.getId();
		});
}


void ScoreManager::filter_by_name(const string& name)
{
	if (wholeFilter)
		filteredStudents = pre_filteredStudents;

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
		filteredStudents.clear();
		for (int i = 0; i < students.size(); ++i)
		{
			string n = students[i].getName();
			if (n.find(name) != string::npos)
			{
				filteredStudents.push_back(students[i]);
			}
		}
	}
	filtering = true;
	pre_filteredStudents = filteredStudents;
}

void ScoreManager::filter_by_dep(const string& dept)
{
	if (wholeFilter)
		filteredStudents = pre_filteredStudents;

	if(filtering)
	{
		vector<Student> temp;
		for (int i = 0; i < filteredStudents.size(); ++i)
		{
			//find函数用于在字符串中查找子字符串，如果找到则返回子字符串的起始位置，否则返回string::npos
			if (filteredStudents[i].getDepartment().find(dept) != string::npos)
			{
				temp.push_back(filteredStudents[i]);
			}
		}
		filteredStudents = temp;
	}
	else
	{
		filteredStudents.clear();
		for (int i = 0; i < students.size(); ++i)
		{
			if (students[i].getDepartment().find(dept) != string::npos)
			{
				filteredStudents.push_back(students[i]);
			}
		}
	}
	filtering = true;
	pre_filteredStudents = filteredStudents;
}

void ScoreManager::filter_by_CourseScoreMin(const string& courseName, double minScore)
{
	if (wholeFilter)
		filteredStudents = pre_filteredStudents;

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
	}
	filtering = true;
	pre_filteredStudents = filteredStudents;
}

void ScoreManager::filter_by_CourseScoreMax(const string& courseName, double maxScore)
{
	if (wholeFilter)
		filteredStudents = pre_filteredStudents;

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
	}
	filtering = true;
	pre_filteredStudents = filteredStudents;
}

void ScoreManager::filter_by_excel()
{
	if (wholeFilter)
		filteredStudents = pre_filteredStudents;

	if (filtering)
	{
		vector<Student> temp;
		for (auto& x : filteredStudents)
		{
			vector<CourseScore> courses = x.getCourses();
			bool excel = true;
			for (auto& y : courses)
			{
				if (y.getScore() < 90)
				{
					excel = false;
					break;
				}
			}
			if (excel)
				temp.push_back(x);
		}
		filteredStudents = temp;
	}
	else
	{
		for (auto& x : students)
		{
			vector<CourseScore> courses = x.getCourses();
			bool excel = true;
			for (auto& y : courses)
			{
				if (y.getScore() < 90)
				{
					excel = false;
					break;
				}
			}
			if (excel)
				filteredStudents.push_back(x);
		}
	}
	filtering = true;
	wholeFilter = true;
}

void ScoreManager::filter_by_middle()
{
	if (wholeFilter)
		filteredStudents = pre_filteredStudents;

	if (filtering)
	{
		vector<Student> temp;
		for (auto& x : filteredStudents)
		{
			vector<CourseScore> courses = x.getCourses();
			bool middle = true;
			for (auto& y : courses)
			{
				if (y.getScore() < 80||y.getScore()>=90)
				{
					middle = false;
					break;
				}
			}
			if (middle)
				temp.push_back(x);
		}
		filteredStudents = temp;
	}
	else
	{
		for (auto& x : students)
		{
			vector<CourseScore> courses = x.getCourses();
			bool middle = true;
			for (auto& y : courses)
			{
				if (y.getScore() < 80 || y.getScore() >= 90)
				{
					middle = false;
					break;
				}
			}
			if (middle)
				filteredStudents.push_back(x);
		}
	}
	filtering = true;
	wholeFilter = true;
}

void ScoreManager::filter_by_Pass()
{
	if (wholeFilter)
		filteredStudents = pre_filteredStudents;

	if (filtering)
	{
		vector<Student> temp;
		for (auto &x :filteredStudents)
		{
			vector<CourseScore> courses = x.getCourses();
			bool pass = true;
			for (auto &y: courses)
			{
				if (y.getScore()<60)
				{
					pass = false;
					break;
				}
			}
			if (pass)
				temp.push_back(x);
		}
		filteredStudents = temp;
	}
	else
	{
		for (auto& x : students)
		{
			vector<CourseScore> courses = x.getCourses();
			bool pass = true;
			for (auto& y : courses)
			{
				if (y.getScore() < 60)
				{
					pass = false;
					break;
				}
			}
			if (pass)
				filteredStudents.push_back(x);
		}
	}
	filtering = true;
	wholeFilter = true;
}

void ScoreManager::filter_by_Fail()
{
	if (wholeFilter)
		filteredStudents = pre_filteredStudents;

	if (filtering)
	{
		vector<Student> temp;
		for (auto& x : filteredStudents)
		{
			vector<CourseScore> courses = x.getCourses();
			bool pass = true;
			for (auto& y : courses)
			{
				if (y.getScore() < 60)
				{
					pass = false;
					break;
				}
			}
			if (!pass)
				temp.push_back(x);
		}
		filteredStudents = temp;
	}
	else
	{
		filteredStudents.clear();
		for (auto& x : students)
		{
			vector<CourseScore> courses = x.getCourses();
			bool pass = true;
			for (auto& y : courses)
			{
				if (y.getScore() < 60)
				{
					pass = false;
					break;
				}
			}
			if (!pass)
				filteredStudents.push_back(x);
		}
	}
	filtering = true;
	wholeFilter = true;
}
