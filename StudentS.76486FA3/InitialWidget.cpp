#include "InitialWidget.h"

InitialWidget::InitialWidget(QWidget* parent) 
{
	initialize_widget();
}

void InitialWidget::initialize_widget()
{
	this->setWindowTitle("NJUST学生成绩管理系统");

	QScreen* myDesktop = QGuiApplication::primaryScreen();
	QRect myAvailableDesktop = myDesktop->availableGeometry();
	this->setGeometry((myAvailableDesktop.width() - this->width()) / 2, (myAvailableDesktop.height() - this->height()) / 2, 500, 400);

	//按钮
	bt* b1 = new bt("打开本地数据库", this);
	connect(b1,& bt::clicked, this, & InitialWidget::On_bt1_Clicked);
	bt* b2 = new bt("导入CSV文件建库", this);
	connect(b2, &bt::clicked, this, &InitialWidget::On_bt2_Clicked);
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

void InitialWidget::On_bt2_Clicked()
{
	QString path = QFileDialog::getOpenFileName(
		this,
		"导入CSV文件",
		QDir::currentPath() + "/CSV/students.csv",
		"CSV文件 (*.csv)"
	);
	QFileInfo info(QDir::cleanPath(path));

	if (!path.isEmpty())
	{
		CSV_Helper* csv = new CSV_Helper();
		csv->import_from_csv(path.toStdString());
		if (csv->is_imported())
		{
			QString newFilePath = QFileDialog::getSaveFileName(
				this,
				"选择新建库路径",
				QDir::currentPath() + "/Database/"+info.baseName()+".db", 
				// 默认路径为项目文件夹的Database文件夹，名称和csv一致
				"SQLite数据库 (*.db)"
			);
			if (newFilePath.isEmpty())
				return;
			MainWindow* sub = new MainWindow(this);
			sub->setParent(this);

			//建立数据库，将CSV数据写入数据库，并连接数据库和表格
			DatabaseHelper* db = new DatabaseHelper;
			db->OpenDatabase(newFilePath);
			db->CreateTableStudents();
			db->FillTableStudents(csv->get_students());	
			sub->connect_table_and_db(db);
			sub->initable();
			this->hide();
			sub->show();
			delete csv,db;
		}
	}
}