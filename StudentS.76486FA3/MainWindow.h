#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include<QMainWindow>
#include<QWidget>
#include<QLabel>
#include<QLineEdit>
#include<QFileDialog>
#include<qfileinfo.h>
#include<QHBoxLayout>
#include<QGuiApplication>
#include<QPushButton>
#include<QVBoxLayout>
#include<QString>
#include<QMenuBar>
#include<QMenu>
#include<QWidgetAction>
#include <QScreen>
#include<QMessageBox>
#include "Table.h"
#include"CSV_Helper.h"

class InitialWidget;

class MainWindow :public QMainWindow
{
	Q_OBJECT;

private:
	InitialWidget* ParentWidget;
	Table* table;
	bool is_allowed_edited;
	bool is_sort_changed;
	int current_sort;

public:
	typedef MainWindow mw;
	typedef QMainWindow qmw;
	typedef QMenuBar qmb;
	typedef QWidget qw;
	typedef QMenu qm;
	typedef QAction qa;

	MainWindow(QWidget* parent = nullptr);

	
	void initialize_window();
	void menu_bar();
	//获得主窗口指针
	void setParent(InitialWidget *parent) { ParentWidget = parent; }
	bool connect_to_database() { return table->isOpen(); }
	bool isLocalDatabaseEmpty(QString path);
	void initable(vector<Student> s) { menu_bar(); table->initialize_table(s); }
	void connect_table_and_db(DatabaseHelper* db) { table->connect_db(db); }
	void keep_sort_measure();
	QWidget* searchWidget();

private slots:
	void On_save_menu_triggered();
	void On_editmenu_triggered();
	void On_cancelmenu_triggered();
	void On_exportmenu_triggered();
	void On_returnmenu_triggered();
	void On_searchedit_Changed(const QString& text);
};

#endif