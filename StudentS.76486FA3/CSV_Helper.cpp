#include "CSV_Helper.h"

void CSV_Helper::export_to_csv(string path)
{
	ofstream file(path);
	if (!file.is_open())
	{
		QMessageBox::critical(nullptr, " ", "无法打开文件！\n");
		return;
	}

	// 写入UTF-8 BOM以确保Excel等程序正确识别编码
	file<<"\xEF\xBB\xBF";
	
	file << WriteTableHeader();
	
	for (auto &x : students)
	{
		
		file << x.getName() << "," << x.getDepartment() << "," << x.getId();
		vector<CourseScore> courses = x.getCourses();
		for (auto y : courses)
		{
			file << "," << y.getScore() << "," << y.getCreditPoint();
		}
		file << "," << x.getGPA() << "," << x.getAverageScore() << "," << x.getTotalCredit()<<'\n';
	}
	file.close();
}

//将表头转换为string
string CSV_Helper::WriteTableHeader( )
{
	string header = "姓名,院系,学号";
	vector<CourseScore> courses = students[0].getCourses();
	for (auto x : courses)
		header += "," + x.getCourseName() + "," + x.doubleToString(x.getCredit(), 1);
	header += ",GPA,平均学分成绩,总学分\n,,";
	for (auto x : courses)
		header += ",得分,绩点";
	header += ",,,\n";
	return header;
}
