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

void CSV_Helper::import_from_csv(const string& path)
{
	ifstream file(path);
	if (!file.is_open())
	{
		QMessageBox::critical(nullptr, " ", "无法打开文件！\n");
		return;
	}

	string header;
	if (!getline(file, header)) //此处getline调用省略了默认参数，默认以换行符为分隔符
	{
		QMessageBox::critical(nullptr, " ", "文件为空或缺少表头！\n");
		return;
	}

	// 去除 UTF-8 BOM（如果存在）
	if (header.size() >= 3 && header[0] == (char)0xEF && header[1] == (char)0xBB && header[2] == (char)0xBF) 
	{
		header = header.substr(3);
	}

	// 读取第二行（我们不需要解析内容，但需要读取以跳过）
	string line2;
	if (!getline(file, line2)) 
	{
		QMessageBox::critical(nullptr, "", "缺少第二行！");
		return;
	}

	// 解析表头，提取出课程名和学分
	vector<pair<string, double>> courseInfo;
	stringstream ss1(header); // 把表头字符串放入一个字符串流中，方便随后依次读取
	string cell;
	// 跳过前三个固定字段：姓名,院系,学号
	getline(ss1, cell, ',');
	getline(ss1, cell, ',');
	getline(ss1, cell, ',');
	vector<string> info;
	while(getline(ss1, cell, ','))
		info.push_back(cell);
	for (int i = 0; i<info.size()-3 ; i+=2 )
	{
		string courseName = info[i];
		double credit = stod(info[i+1]); // 字符串转换为double
		courseInfo.emplace_back(courseName, credit);
		//此处用emplace_back而不是push_back是因为我们直接传入了构造函数的参数，而不是一个pair类的对象
		//如果用push_back就需要先创建一个pair对象，再将其传入push_back，而emplace_back则直接在容器中构造对象，避免了不必要的复制和移动操作，提高了效率
		//其他位置使用push_back也是因为我们已经有了一个完整的对象，而不是需要传入构造函数参数的情况，所以直接使用push_back即可
	}

	string line;
	while (getline(file, line))
	{
		if (line.empty()) continue;

		vector<string> stuInfo;
		stringstream ss(line);
		string each_stuInfo;
		while (getline(ss, each_stuInfo, ','))
		{
			stuInfo.push_back(each_stuInfo);
		}

		string name = stuInfo[0];
		string dept = stuInfo[1];
		int id = stoi(stuInfo[2]); // 字符串转换为int

		vector<CourseScore> courses;
		int i = 3; // 从第4个字段开始是课程得分和绩点
		for (int j = 0; j < courseInfo.size(); j++)
		{
			double score = stod(stuInfo[i]);
			double creditPoint = stod(stuInfo[i + 1]);

			CourseScore cs(courseInfo[j].first, score, courseInfo[j].second, creditPoint);
			courses.push_back(cs);
			i += 2;
		}

		Student stu(name, dept, id);

		for (auto& c : courses)
		{
			stu.updateCourses(c);
		}
		stu.calculateAverageScore();
		stu.calculateGPA();
		stu.calculateTotalCredit();

		students.push_back(stu);
	}

	file.close();
	imported = true;
}
