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

public:
	Table(QWidget* parent = nullptr) { DB = nullptr; };

	vector<Student> get_current_studentlist() { return DB->get_All_Students(); }
	void OpenLocalDatabase(QString );
	bool isOpen() { return DB->isOpen(); }
	bool isLocalDatabaseEmpty();
	void class_to_table(vector<Student>);

	qtwi* StrItem(string x);
	template<typename T>
	qtwi* NumItem(T x);
};

#endif

