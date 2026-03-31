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
	string WriteTableHeader();

	void import_from_csv(const string& path);

	vector<Student>& get_students() { return students; }
	bool is_imported() { return imported; }
};

