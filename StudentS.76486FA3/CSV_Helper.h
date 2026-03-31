#pragma once
#include"Student.h"
#include<QMessageBox>
#include<fstream>


class CSV_Helper
{
	vector<Student> students;

public:
	CSV_Helper() = default;
	CSV_Helper(vector<Student> s) :students(s) {};

	void export_to_csv(string path);
	string WriteTableHeader();

	vector<Student> import_from_csv(const string& path);
};

