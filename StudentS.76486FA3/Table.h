#ifndef TABLE_H
#define TABLE_H

#include<QTableWidget>
#include"DatabaseHelper.h"

class Table :public QTableWidget
{
	Q_OBJECT;

	typedef QTableWidgetItem qtwi;

private:
	DatabaseHelper* DB;
	vector<Student> current_students;
	bool isRefreshing;   //当正在刷新表格时，禁止触发itemChanged事件

public:
	Table(QWidget* parent = nullptr) ;
	~Table() { delete DB; }

	vector<Student> get_saved_studentlist() { return DB->get_All_Students(); }
	vector<Student> get_current_studentlist() { return current_students; }
	void updateStudent() { DB->update_Student(current_students); }
	void OpenLocalDatabase(QString );
	bool isOpen() { return DB->isOpen(); }
	bool isLocalDatabaseEmpty();
	void class_to_table(vector<Student>);

	qtwi* StrItem(string x);
	qtwi* fixed_StrItem(string x);

	template<typename T>
	qtwi* NumItem(T x);
	template<typename T>
	qtwi* fixed_NumItem(T x);

private slots:
	void On_cell_changed(QTableWidgetItem* item);

};

#endif

