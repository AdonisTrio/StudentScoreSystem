#pragma once
#include"Student.h"
#include<QMessageBox>
#include<fstream>


class CSV_Helper
{
	vector<Student> students;
	bool imported = false;

public:

	CSV_Helper() {}
	CSV_Helper(vector<Student> s) :students(s) {}

	void export_to_csv(string path);
	/*· 输入参数：path – 输出路径
· 输出参数：无
· 功能：将students导出为CSV
· 算法：写入UTF-8 BOM；写入表头（课程名+学分）；写入数据行（学生基本信息、各科成绩和绩点、GPA、平均分、总学分）*/
	string WriteTableHeader();

	void import_from_csv(const string& path);
	/*· 输入参数：path – CSV文件路径
· 输出参数：无
· 功能：从CSV导入学生数据
· 算法：读取表头，解析课程名和学分；读取数据行，按列解析姓名、院系、学号、各科成绩和绩点；创建Student和CourseScore对象；计算汇总指标后存入students向量*/

	vector<Student>& get_students() { return students; }
	bool is_imported() { return imported; }
};

