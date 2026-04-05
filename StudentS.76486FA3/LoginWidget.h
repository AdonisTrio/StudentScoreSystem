#pragma once
#include"DatabaseHelper.h"
#include "InitialWidget.h"

class LoginWidget :public QWidget
{
	Q_OBJECT;
public:
	typedef InitialWidget iw;
	typedef QPushButton bt;
	typedef QVBoxLayout vb;
	typedef QHBoxLayout hb;
	typedef QLabel lb;

	LoginWidget(QWidget* parent = nullptr);

	void initialize_widget();

private slots:
	void On_bt1_Clicked();
	void On_bt2_Clicked();
};