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
	bool isRefreshing;   //当正在刷新表格时，禁止触发itemChanged事件

public:
	Table(QWidget* parent = nullptr) ;
	~Table() { delete DB,manager; }

	vector<Student> get_saved_studentlist() { return DB->get_All_Students(); }
	vector<Student> get_current_studentlist() { return manager->getStudents(); }

	void updateStudent() { DB->update_Student(manager->getStudents()); }
	void OpenLocalDatabase(QString );
	bool isOpen() { return DB->isOpen(); }
	bool isLocalDatabaseEmpty();
	void class_to_table(vector<Student>);
	void customed_sort(int n) { manager->Sort_by_course(n); }
	void default_sort() { manager->default_Sort(); }
	void id_sort() { manager->Sort_by_id(); }
	void reset() { delete manager; manager = new ScoreManager(DB->get_All_Students()); }
	void initialize_table();
	void connect_db(DatabaseHelper* db) { DB = db; }
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

