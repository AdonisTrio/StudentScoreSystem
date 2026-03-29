#include "InitialWidget.h"

InitialWidget::InitialWidget(QWidget* parent) 
{
	initialize_widget();
}

void InitialWidget::initialize_widget()
{
	this->setWindowTitle("NJUST学生成绩管理系统");
	this->setGeometry((myAvailableDesktop.width() - this->width()) / 2, (myAvailableDesktop.height() - this->height()) / 2, 500, 400);

	//按钮
	bt* b1 = new bt("打开本地数据库", this);
	connect(b1,& bt::clicked, this, & InitialWidget::On_bt1_Clicked);
	bt* b2 = new bt("导入外部CSV文件", this);
	b1->setFont(QFont("SimSun", 15));
	b2->setFont(QFont("SimSun", 15));
	b1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	b2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	
	//标签
	lb* l1 = new lb("欢迎使用学生成绩管理系统", this);
	l1->setFont(QFont("Microsoft YaHei", 25));
	l1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	l1->setAlignment(Qt::AlignCenter);
	
	//布局设计
	hb* layout1 = new hb();
	vb* layout2 = new vb(this);
	layout1->setSpacing(50);
	layout1->setContentsMargins(30, 30, 30, 60);
	layout2->setContentsMargins(50, 70, 50, 30);
	layout1->addWidget(b1);
	layout1->addWidget(b2);
	layout2->addWidget(l1);
	layout2->addLayout(layout1);
	layout2->setAlignment(l1, Qt::AlignHCenter);
}

//选择并打开本地数据库，隐藏当前窗口
void InitialWidget::On_bt1_Clicked()
{
	MainWindow* sub = new MainWindow(this);
	sub->setParent(this);
	sub->setWindowTitle("NJUST学生成绩管理系统");
	sub->resize(myWholeDesktop.width() * 0.85, myWholeDesktop.height() * 0.85);
	sub->move((myWholeDesktop.width() - sub->width()) / 2, (myWholeDesktop.height() - sub->height()) * 0.8 / 2);
	try
	{
		QString path = QFileDialog::getOpenFileName(
			this,
			"选择本地数据库",
			QDir::currentPath() + "/Database/students.db",
			"SQLite数据库 (*.db )"
		);
		if (path.isEmpty())
			throw "请选择数据库文件！";
		else if (sub->isLocalDatabaseEmpty(path))
			throw "数据库中没有学生数据！";
		else if (!sub->connect_to_database())
			throw "连接数据库失败！";
		else
		{
			this->hide();
			sub->show();
		}
	}
	catch (const char* msg)
	{
		QMessageBox::critical(this, " ", msg);
	}
}
