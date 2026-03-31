#ifndef INITIALWIDGET_H
#define INITIALWIDGET_H

#include "MainWindow.h"

class MainWindow;

class InitialWidget :public QWidget
{
	Q_OBJECT;

public:
	typedef InitialWidget iw;
	typedef QPushButton bt;
	typedef QVBoxLayout vb;
	typedef QHBoxLayout hb;
	typedef QLabel lb;

	InitialWidget(QWidget* parent = nullptr) ;

	void initialize_widget();

private slots:
	void On_bt1_Clicked();
	void On_bt2_Clicked();
};

#endif