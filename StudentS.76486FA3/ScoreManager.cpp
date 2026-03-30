#include"ScoreManager.h"

void ScoreManager::default_Sort()
{
	sort(students.begin(), students.end(), [](Student a, Student b) {
		if (a.getAverageScore() != b.getAverageScore())
			return a.getAverageScore() > b.getAverageScore();
		else
			return a.getId() < b.getId();
		});
}

void ScoreManager::Sort_by_id()
{
	sort(students.begin(), students.end(), [](Student a, Student b) {
		return a.getId() < b.getId();
		});
}


void ScoreManager::Sort_by_course(int n)
{
	sort(students.begin(), students.end(), [n](Student a, Student b) {

		vector<CourseScore> A = a.getCourses();
		vector<CourseScore> B = b.getCourses();
		if (A[n].getScore() != B[n].getScore())
			return A[n].getScore() > B[n].getScore();
		else
			return a.getId() < b.getId();
		});
}
