#ifndef TABLE_H
#define TABLE_H

#include<QTableWidget>
#include"DatabaseHelper.h"
#include"ScoreManager.h"

class Table :public QTableWidget
{
	Q_OBJECT;

	typedef QTableWidgetItem qtwi;

private:
	DatabaseHelper* DB;
	ScoreManager* manager;
	bool is_course_changed;
	bool isRefreshing;   //当正在刷新表格时，禁止触发itemChanged事件

public:
	Table(QWidget* parent = nullptr) ;
	~Table() { delete DB; delete manager; }

	vector<Student> get_saved_studentlist() { return DB->get_All_Students(); }
	vector<Student> get_current_studentlist() { return manager->getStudents(); }

	void updateStudent() { DB->update_Student(manager->getStudents()); }
	void class_to_table(vector<Student>);
	/*· 输入参数：students – 待显示的学生列表
· 输出参数：无
· 功能：将学生数据填充到表格
· 算法：动态设置表格行数列数；合并表头单元格（课程名跨两列）；逐行逐列创建QTableWidgetItem并设置内容*/

	bool course_changed() { return is_course_changed; }
	void set_course_changed() { is_course_changed = false; }

	void customed_sort(int n) { manager->Sort_by_course(n); }
	void default_sort() { manager->default_Sort(); }
	void id_sort() { manager->Sort_by_id(); }


	void reset() { delete manager; manager = new ScoreManager(DB->get_All_Students()); }
	void initialize_table();
	void connect_db(DatabaseHelper* db) { DB = db; }
	qtwi* StrItem(string x);
	qtwi* fixed_StrItem(string x);
	qtwi* fixed_NumItem(int x);

	void addEmptyStudent();
	void deleteSelectedStudent();

	void filter_by_name(const string& keyword) { manager->filter_by_name(keyword);  }
	void filter_by_dep(const string& dept) { manager->filter_by_dep(dept);  }
	void filter_by_CourseScoreMin(const string& courseName, double minScore) { manager->filter_by_CourseScoreMin(courseName, minScore); }
	void filter_by_CourseScoreMax(const string& courseName, double maxScore) { manager->filter_by_CourseScoreMax(courseName, maxScore);}
	void filter_by_Excel() { manager->filter_by_excel(); }
	void filter_by_Middle() { manager->filter_by_middle(); }
	void filter_by_Pass() { manager->filter_by_Pass(); }
	void filter_by_Fail() { manager->filter_by_Fail(); }
	void clearFilter() { manager->clearFilter(); }


private slots:
	void On_cell_changed(QTableWidgetItem* item);
	/*· 输入参数：item – 被编辑的单元格
· 输出参数：无
· 功能：处理单元格修改事件
· 算法：根据行列判断修改的是学生信息还是课程成绩；进行输入验证（正则表达式限制字符类型）；更新对应的Student对象，触发重新计算汇总值，并刷新表格中相关单元格*/

};

#endif

